#include "L1TMuonSimulations/EMTFSimulation/interface/PatternGenerator.h"

#include "L1TMuonSimulations/EMTFSimulationIO/interface/MessageLogger.h"
#include "L1TMuonSimulations/EMTFSimulationIO/interface/CSCStubReader.h"
#include "L1TMuonSimulations/EMTFSimulationIO/interface/PatternBankReader.h"
using namespace phasetwoemtf;


// _____________________________________________________________________________
PatternGenerator::PatternGenerator(const ProgramOption& po)
: po_(po), nEvents_(po.maxEvents), firstEvent_(po.skipEvents), verbose_(po.verbose)
{
    bank_.reset(new PatternBankContainer());

    arbiter_.reset(new SuperstripArbiter());
    arbiter_ -> setDefinition(po_.superstrip);
}

PatternGenerator::~PatternGenerator() {}

void PatternGenerator::init() {}

void PatternGenerator::run() {
    makePatterns(po_.input);
    writePatterns(po_.output);
    analyzePatterns(po_.output);
}


// _____________________________________________________________________________
// Make pattern bank
void PatternGenerator::makePatterns(TString src) {

    LogInfo("PatternGenerator", verbose_) << "Using nEvents " << nEvents_ << " firstEvent " << firstEvent_ << std::endl;
    LogInfo("PatternGenerator", verbose_) << "Using superstrip " << arbiter_ -> str() << std::endl;

    // _________________________________________________________________________
    // For reading
    CSCStubReader reader(verbose_);
    reader.init(src);

    // _________________________________________________________________________
    // Loop over all events

    pattern_t patt;
    attrib_t attr;

    // Bookkeepers
    float coverage = 0.;
    long int bankSize = 0, bankSizeOld = -100000, nKeptOld = -100000;
    long int nRead = 0, nKept = 0;

    for (long long ievt=firstEvent_; ievt<nEvents_; ++ievt) {
        if (reader.loadTree(ievt) < 0)  break;
        reader.getEntry(ievt);

        // Running estimate of coverage
        if (ievt%100000==0) {
            bankSize = bank_->size();
            coverage = 1.0 - float(bankSize - bankSizeOld) / float(nKept - nKeptOld);

            LogInfo("PatternGenerator", verbose_) << Form("... Processing event: %7lld, keeping: %7ld, # patterns: %7ld, coverage: %7.5f", ievt, nKept, bankSize, coverage) << std::endl;

            bankSizeOld = bankSize;
            nKeptOld = nKept;
        }

        const unsigned nstubs = reader.vb_moduleId->size();
        LogDebug("PatternGenerator", verbose_) << "... evt: " << ievt << " # stubs: " << nstubs << std::endl;

        // Get sim info
        float simPt       = reader.vp_pt->front();
        float simEta      = reader.vp_eta->front();
        float simPhi      = reader.vp_phi->front();
        //float simVx       = reader.vp_vx->front();
        //float simVy       = reader.vp_vy->front();
        float simVz       = reader.vp_vz->front();
        int   simCharge   = reader.vp_charge->front();
        float simCotTheta = std::sinh(simEta);
        float simInvPt    = float(simCharge)/simPt;

        // Apply pt, eta, phi requirements
        bool sim = (po_.minPt  <= simPt  && simPt  <= po_.maxPt  &&
                    po_.minEta <= simEta && simEta <= po_.maxEta &&
                    po_.minPhi <= simPhi && simPhi <= po_.maxPhi &&
                    po_.minVz  <= simVz  && simVz  <= po_.maxVz);

        LogDebug("StubSelector", verbose_) << "... evt: " << ievt << " simPt: " << simPt << " simEta: " << simEta << " simPhi: " << simPhi << " simVz: " << simVz << " simInvPt: " << simInvPt << " keep? " << sim << std::endl;

        if (!sim) {
            ++nRead;
            continue;
        }

        // _____________________________________________________________________
        // Apply trigger tower acceptance

        assert(nstubs == 5);
        unsigned ngoodstubs = 0;
        unsigned ngoodstubs_ME11 = 0;
        unsigned ngoodstubs_ME12 = 0;
        unsigned ngoodstubs_ME2to4 = 0;

        for (unsigned istub=0; istub<nstubs; ++istub) {
            uint32_t moduleId    = reader.vb_moduleId   ->at(istub);
            int16_t  isector     = reader.vb_isector    ->at(istub);

            if (unsigned(isector) != po_.sector)  //FIXME: include neighbors
                continue;

            if (moduleId == 999999)  //FIXME: handle missing stubs
                continue;

            ++ngoodstubs;
            if (istub == 0) {
                ++ngoodstubs_ME11;
            } else if (istub == 1) {
                ++ngoodstubs_ME12;
            } else {
                ++ngoodstubs_ME2to4;
            }
        }

        // Categorize events
        unsigned category = 0;
        if (ngoodstubs_ME11 && ngoodstubs_ME2to4 == 3) {
            category = 0;
        } else if (ngoodstubs_ME11 && ngoodstubs_ME2to4 == 2) {
            category = 1;
        } else if (ngoodstubs_ME12 && ngoodstubs_ME2to4 == 3) {
            category = 2;
        } else if (ngoodstubs_ME12 && ngoodstubs_ME2to4 == 2) {
            category = 3;
        } else if (ngoodstubs_ME2to4 == 3) {
            category = 4;
        } else {
            category = 5;
        }

        LogDebug("PatternGenerator", verbose_) << "... evt: " << ievt << " ngoodstubs: " << ngoodstubs << " ngoodstubs_ME11: " << ngoodstubs_ME11 << " ngoodstubs_ME12: " << ngoodstubs_ME12 << " ngoodstubs_ME2to4: " << ngoodstubs_ME2to4 << " category: " << category << std::endl;

        if (category > 0) {
            ++nRead;
            continue;
        }


        // _____________________________________________________________________
        // Start generating patterns

        patt.fill(0);
        attr.reset();

        // Loop over reconstructed stubs
        for (unsigned istub=0; istub<nstubs; ++istub) {

            uint32_t moduleId    = reader.vb_moduleId   ->at(istub);
            int16_t  isector     = reader.vb_isector    ->at(istub);
            //int16_t  isubsector  = reader.vb_isubsector ->at(istub);
            uint16_t keywire     = reader.vb_keywire    ->at(istub);
            uint16_t strip       = reader.vb_strip      ->at(istub);
            uint16_t pattern     = reader.vb_pattern    ->at(istub);

            float    globalPhi   = reader.vb_globalPhi  ->at(istub);
            float    globalTheta = reader.vb_globalTheta->at(istub);
            //float    globalEta   = reader.vb_globalEta  ->at(istub);
            float    globalRho   = reader.vb_globalRho  ->at(istub);

            // _________________________________________________________________
            // Different strategy for different categories
            if (category == 0) {
                if (istub == 1)
                    continue;

            } else if (category == 1) {
                if (istub == 1)
                    continue;

                if (unsigned(isector) != po_.sector)
                    continue;
                if (moduleId == 999999)
                    continue;

            } else {
                // ?
            }

            // _________________________________________________________________
            // Find superstrip ID
            unsigned ssId = 0;
            if (arbiter_ -> getCoordType() == SuperstripCoordType::LOCAL) {
                ssId = arbiter_ -> superstripLocal(moduleId, strip, keywire, pattern);

            } else if (arbiter_ -> getCoordType() == SuperstripCoordType::GLOBAL) {
                ssId = arbiter_ -> superstripGlobal(moduleId, globalRho, globalPhi, globalTheta, pattern);
            }
            patt.at(istub) = ssId;

            LogDebug("PatternGenerator", verbose_) << "... ... stub: " << istub << " moduleId: " << moduleId << " strip: " << strip << " segment: " << keywire << " rho: " << globalRho << " phi: " << globalPhi << " theta: " << globalTheta << " ds: " << pattern << std::endl;
            LogDebug("PatternGenerator", verbose_) << "... ... stub: " << istub << " ssId: " << ssId << std::endl;
        }

        // Fill pattern
        bank_ -> fill(std::make_pair(patt, attr), simInvPt, simCotTheta, simPhi, simVz);

        LogDebug("PatternGenerator", verbose_) << "... evt: " << ievt << " patt: " << patt << std::endl;

        ++nKept;
        ++nRead;
    }

    if (nRead == 0) {
        throw std::runtime_error("Failed to read any event.");
    }

    LogInfo("PatternGenerator", verbose_) << Form("Read: %7ld, kept: %7ld, # patterns: %7lu, coverage: %7.5f", nRead, nKept, bank_->size(), coverage) << std::endl;

    // _________________________________________________________________________
    // Freeze the pattern bank

    bank_->freeze();

    bank_->getBankInfo().coverage      = coverage;
    bank_->getBankInfo().count         = nKept;
    bank_->getBankInfo().tower         = po_.tower;
    bank_->getBankInfo().sector        = po_.sector;
    bank_->getBankInfo().superstrip    = po_.superstrip;
    bank_->getBankInfo().superstrip_nx = arbiter_ -> nx();
    bank_->getBankInfo().superstrip_nz = arbiter_ -> nz();

    if (verbose_>2) {
        for (unsigned i=0; i<bank_->getBank().size(); ++i) {
            const pattern_pair& apair = bank_->getBank().at(i);
            LogDebug("PatternGenerator", verbose_) << "... patt: " << i << "  " << apair.first << " popularity: " << apair.second.n << std::endl;
        }
    }
}

// _____________________________________________________________________________
// Write pattern bank
void PatternGenerator::writePatterns(TString out) {

    // _________________________________________________________________________
    // For writing
    PatternBankWriter writer(verbose_);
    writer.init(out);

    // _________________________________________________________________________
    writer.fillPatternBankInfo(bank_->getBankInfo());

    writer.fillPatternBank(bank_->getBank());

    long long nentries = writer.writeTree();
    assert(nentries == (long long) bank_->size());

    bank_->clear();  // deallocate memory
}

// _____________________________________________________________________________
// Analyze pattern bank
void PatternGenerator::analyzePatterns(TString bank) {

    // _________________________________________________________________________
    // For reading
    PatternBankReader pbreader(verbose_);
    pbreader.init(bank);

    // _________________________________________________________________________
    // Get pattern bank info
    pbreader.getInfoTree();
    float coverage = pbreader.pb_coverage;
    unsigned long count = pbreader.pb_count;

    // _________________________________________________________________________
    // Get pattern bank

    long long npatterns = pbreader.getEntries();

    // Bookkeepers
    long int nKept = 0;
    unsigned pop = -1, oldPop = -1;
    long int n90 = 0, n95 = 0, n99 = 0;
    long int pop90 = 0, pop95 = 0, pop99 = 0;

    for (long long ipatt=0; ipatt<npatterns; ++ipatt) {
        pbreader.getEntry(ipatt);

        if (ipatt%1000==0) {
            float icov = float(nKept) * coverage / count;
            if (icov < 0.90 + 1e-5) {
                n90 = ipatt;
                pop90 = pop;
            }
            if (icov < 0.95 + 1e-5) {
                n95 = ipatt;
                pop95 = pop;
            }
            if (icov < 0.99 + 1e-5) {
                n99 = ipatt;
                pop99 = pop;
            }

            LogInfo("PatternGenerator", verbose_) << Form("... Writing event: %7lld, sorted coverage: %7.5f", ipatt, icov) << std::endl;
        }

        // Check whether patterns are indeed sorted by popularity
        pop = pbreader.pb_popularity;
        assert(oldPop >= pop);
        oldPop = pop;
        nKept += pop;
    }

    LogInfo("PatternGenerator", verbose_) << "After sorting by popularity, max coverage = " << coverage << std::endl;
    LogInfo("PatternGenerator", verbose_) << "  N(90% cov) = " << n90 << " popularity = " << pop90 << std::endl;
    LogInfo("PatternGenerator", verbose_) << "  N(95% cov) = " << n95 << " popularity = " << pop95 << std::endl;
    LogInfo("PatternGenerator", verbose_) << "  N(99% cov) = " << n99 << " popularity = " << pop99 << std::endl;
}

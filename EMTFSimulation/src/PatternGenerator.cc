#include "L1TMuonSimulations/EMTFSimulation/interface/PatternGenerator.h"

#include "L1TMuonSimulations/EMTFSimulationIO/interface/MessageLogger.h"
#include "L1TMuonSimulations/EMTFSimulationIO/interface/CSCStubReader.h"
#include "L1TMuonSimulations/EMTFSimulationIO/interface/PatternBankReader.h"


// _____________________________________________________________________________
PatternGenerator::PatternGenerator(const ProgramOption& po)
: po_(po), nEvents_(po.maxEvents), verbose_(po.verbose)
{
    bank_.reset(new PatternBankContainer());

    bankInfo_.reset(new PatternBankInfo());

    arbiter_.reset(new SuperstripArbiter());
}

PatternGenerator::~PatternGenerator() {}

void PatternGenerator::init() {

}

void PatternGenerator::run() {
    makePatterns(po_.input);
    writePatterns(po_.output);
}


// _____________________________________________________________________________
// Make pattern bank
void PatternGenerator::makePatterns(TString src) {

    // _________________________________________________________________________
    // For reading
    CSCStubReader reader(verbose_);
    reader.init(src);

    // _________________________________________________________________________
    // Loop over all events

    pattern_type patt;
    patt.fill(0);
    pattern_attr attr;

    // Bookkeepers
    float coverage = 0.;
    long int bankSize = 0, bankSizeOld = -100000, nKeptOld = -100000;
    long int nRead = 0, nKept = 0;

    for (long long ievt=0; ievt<nEvents_; ++ievt) {
        if (reader.loadTree(ievt) < 0)  break;
        reader.getEntry(ievt);

        // Running estimate of coverage
        if (ievt%100000==0) {
            bankSize = bank_->getMap().size();
            coverage = 1.0 - float(bankSize - bankSizeOld) / float(nKept - nKeptOld);

            LogInfo("PatternGenerator", verbose_) << Form("... Processing event: %7lld, keeping: %7ld, # patterns: %7ld, coverage: %7.5f", ievt, nKept, bankSize, coverage) << std::endl;

            bankSizeOld = bankSize;
            nKeptOld = nKept;
        }

        const unsigned nstubs = reader.vb_ichamber->size();
        LogDebug("PatternGenerator", verbose_) << "... evt: " << ievt << " # stubs: " << nstubs << std::endl;

        // Get sim info
        float simPt           = reader.vp_pt->front();
        float simEta          = reader.vp_eta->front();
        float simPhi          = reader.vp_phi->front();
        //float simVx           = reader.vp_vx->front();
        //float simVy           = reader.vp_vy->front();
        float simVz           = reader.vp_vz->front();
        int   simCharge       = reader.vp_charge->front();
        float simCotTheta     = std::sinh(simEta);
        float simChargeOverPt = float(simCharge)/simPt;

        // Apply pt, eta, phi requirements
        bool sim = (po_.minPt  <= simPt  && simPt  <= po_.maxPt  &&
                    po_.minEta <= simEta && simEta <= po_.maxEta &&
                    po_.minPhi <= simPhi && simPhi <= po_.maxPhi &&
                    po_.minVz  <= simVz  && simVz  <= po_.maxVz);
        if (!sim) {
            ++nRead;
            continue;
        }

        // Apply trigger tower acceptance
        assert(nstubs == 4);  // 4 endcap disks
        unsigned ngoodstubs = 0;
        for (unsigned istub=0; istub<nstubs; ++istub) {
            unsigned isector = reader.vb_isector  ->at(istub);

            if (isector == po_.sector)  //FIXME: include neighbors
                ++ngoodstubs;
        }

        if (ngoodstubs != 4) {  // 4 endcap disks
            ++nRead;
            continue;
        }


        // _____________________________________________________________________
        // Start generating patterns

        patt.fill(0);

        // Loop over reconstructed stubs
        for (unsigned istub=0; istub<nstubs; ++istub) {

            uint32_t ichamber    = reader.vb_ichamber   ->at(istub);
            //uint16_t isector     = reader.vb_isector    ->at(istub);
            uint16_t isubsector  = reader.vb_isubsector ->at(istub);
            uint16_t keywire     = reader.vb_keywire    ->at(istub);
            uint16_t strip       = reader.vb_strip      ->at(istub);
            uint16_t pattern     = reader.vb_pattern    ->at(istub);
            uint16_t cscID       = reader.vb_cscID      ->at(istub);

            float    globalPhi   = reader.vb_globalPhi  ->at(istub);
            float    globalTheta = reader.vb_globalTheta->at(istub);
            //float    globalEta   = reader.vb_globalEta  ->at(istub);
            float    globalRho   = reader.vb_globalRho  ->at(istub);

            // Customized
            unsigned lay = decodeLayer(ichamber);   // endcap*10 + station
            unsigned lad = decodeLadder(ichamber);  // ring
            //unsigned mod = decodeLayer(ichamber);   // chamber
            unsigned station = lay%10;
            unsigned ring = lad;

            if (station == 1) {  // station 1 has 2 subsectors
                cscID = (isubsector - 1)*10 + cscID;
            }

            //FIXME: check proper 'factor'
            if (station == 1 && ring == 4) {  // strip pitch is not constant in ME1/n
                strip *= 1707;
                strip >>= 10;
            } else if (station == 1 && ring == 3) {
                strip *= 947;
                strip >>= 10;
            } else if (station == 1 && ring == 1) {
                strip *= 1301;
                strip >>= 10;
            } else {
                strip *= 1024;
                strip >>= 10;
            }

            if (station == 1 || cscID > 3) {  // 10 degree chambers
                strip >>= 1;
            }

            strip >>= 0;  //FIXME

            // Find superstrip ID
            unsigned ssId = 0;
            if (!arbiter_ -> useGlobalCoord()) {  // local coordinates
                ssId = arbiter_ -> superstripLocal(cscID, strip, keywire, pattern);

            } else {                              // global coordinates
                ssId = arbiter_ -> superstripGlobal(cscID, globalRho, globalPhi, globalTheta, pattern);
            }
            patt.at(istub) = ssId;

            LogDebug("PatternGenerator", verbose_) << "... ... stub: " << istub << " moduleId: " << cscID << " strip: " << strip << " segment: " << keywire << " rho: " << globalRho << " phi: " << globalPhi << " theta: " << globalTheta << " ds: " << pattern << std::endl;
            LogDebug("PatternGenerator", verbose_) << "... ... stub: " << istub << " ssId: " << ssId << std::endl;
        }

        // Fill pattern
        bank_ -> fill(std::make_pair(patt, attr), simChargeOverPt, simCotTheta, simPhi, simVz);

        LogDebug("PatternGenerator", verbose_) << "... evt: " << ievt << " patt: " << patt << std::endl;

        ++nKept;
        ++nRead;
    }

    if (nRead == 0) {
        throw std::runtime_error("Failed to read any event.");
    }

    LogInfo("PatternGenerator", verbose_) << Form("Read: %7ld, kept: %7ld, # patterns: %7lu, coverage: %7.5f", nRead, nKept, bank_->getMap().size(), coverage) << std::endl;

    // _________________________________________________________________________
    // Freeze the pattern bank

    bank_ -> freeze();

    bankInfo_ -> coverage      = coverage;
    bankInfo_ -> count         = nKept;
    bankInfo_ -> tower         = po_.tower;
    bankInfo_ -> sector        = po_.sector;
    bankInfo_ -> superstrip    = po_.superstrip;
    bankInfo_ -> superstrip_nx = arbiter_ -> nx();
    bankInfo_ -> superstrip_nz = arbiter_ -> nz();

    if (verbose_>2) {
        for (unsigned i=0; i<bank_->getPairs().size(); ++i) {
            const pattern_pair& apair = bank_->getPairs().at(i);
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
    // Save pattern bank info
    *(writer.pb_coverage)      = bankInfo_ -> coverage;
    *(writer.pb_count)         = bankInfo_ -> count;
    *(writer.pb_tower)         = bankInfo_ -> tower;
    *(writer.pb_sector)        = bankInfo_ -> sector;
    *(writer.pb_superstrip)    = bankInfo_ -> superstrip;
    *(writer.pb_superstrip_nx) = bankInfo_ -> superstrip_nx;
    *(writer.pb_superstrip_nz) = bankInfo_ -> superstrip_nz;
    writer.fillPatternBankInfo();

    // _________________________________________________________________________
    // Save pattern bank
    const long long npatterns = bank_->getPairs().size();

    // Bookkeepers
    unsigned nKept = 0;
    unsigned pop = -1, oldPop = -1;
    long n90=0, n95=0, n99=0;

    for (long long ipatt=0; ipatt<npatterns; ++ipatt) {
        if (ipatt%1000==0) {
            float coverage = float(nKept) * (bankInfo_->coverage) / (bankInfo_->count);
            if (coverage < 0.90 + 1e-5)
                n90 = ipatt;
            if (coverage < 0.95 + 1e-5)
                n95 = ipatt;
            if (coverage < 0.99 + 1e-5)
                n99 = ipatt;

            LogInfo("PatternGenerator", verbose_) << Form("... Writing event: %7lld, sorted coverage: %7.5f", ipatt, coverage) << std::endl;
        }

        const pattern_type& patt = bank_->getPairs().at(ipatt).first;
        const pattern_attr& attr = bank_->getPairs().at(ipatt).second;

        // Check whether patterns are indeed sorted by popularity
        pop = attr.n;
        assert(oldPop >= pop);
        oldPop = pop;
        nKept += pop;

        if (pop < (unsigned) po_.minPopularity)  // cut off
            break;

        writer.pb_superstripIds->clear();
        for (unsigned i=0; i<4; ++i) {  // 4 endcap disks
            writer.pb_superstripIds->push_back(patt.at(i));
        }
        *(writer.pb_popularity)     = attr.n;
        *(writer.pb_invPt_mean)     = (attr.invPt_mean);
        *(writer.pb_invPt_sigma)    = std::sqrt(attr.invPt_variance);
        *(writer.pb_cotTheta_mean)  = (attr.cotTheta_mean);
        *(writer.pb_cotTheta_sigma) = std::sqrt(attr.cotTheta_variance);
        *(writer.pb_phi_mean)       = (attr.phi_mean);
        *(writer.pb_phi_sigma)      = std::sqrt(attr.phi_variance);
        *(writer.pb_z0_mean)        = (attr.z0_mean);
        *(writer.pb_z0_sigma)       = std::sqrt(attr.z0_variance);
        writer.fillPatternBank();
    }

    long long nentries = writer.writeTree();
    assert(npatterns == nentries);
    assert(nKept == bankInfo_->count);

    LogInfo("PatternGenerator", verbose_) << "After sorting by popularity, max coverage = " << bankInfo_->coverage << std::endl;
    LogInfo("PatternGenerator", verbose_) << "  N(90% cov) = " << n90 << " popularity = " << bank_->getPairs().at(n90).second.n << std::endl;
    LogInfo("PatternGenerator", verbose_) << "  N(95% cov) = " << n95 << " popularity = " << bank_->getPairs().at(n95).second.n << std::endl;
    LogInfo("PatternGenerator", verbose_) << "  N(99% cov) = " << n99 << " popularity = " << bank_->getPairs().at(n99).second.n << std::endl;
}

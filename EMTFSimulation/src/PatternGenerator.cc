#include "L1TMuonSimulations/EMTFSimulation/interface/PatternGenerator.h"

#include "L1TMuonSimulations/EMTFSimulationIO/interface/CSCStubReader.h"
#include "L1TMuonSimulations/EMTFSimulationIO/interface/PatternBankReader.h"

static const unsigned MAX_POPULARITY = 0xffffffff;  // unsigned

namespace {
    // Insert pattern
    void fill_statistics(const unsigned long n, float& mean, float& variance, float x) {
        //++n;
        mean += (x - mean)/n;
        if (n > 1)  variance += (x - mean)*(x - mean)/(n-1) - (variance/n);
    }

    // Insert pattern
    void fill_attribute(pattern_attr& attr, float invPt, float cotTheta, float phi, float z0) {
        ++ attr.n;
        fill_statistics(attr.n, attr.invPt_mean, attr.invPt_variance, invPt);
        fill_statistics(attr.n, attr.cotTheta_mean, attr.cotTheta_variance, cotTheta);
        fill_statistics(attr.n, attr.phi_mean, attr.phi_variance, phi);
        fill_statistics(attr.n, attr.z0_mean, attr.z0_variance, z0);
    }

    // Comparator
    bool sortByPopularity(const pattern_pair& lhs, const pattern_pair& rhs) {
        return lhs.second.n > rhs.second.n;
    }
}


// _____________________________________________________________________________
PatternGenerator::PatternGenerator(const ProgramOption& po)
: po_(po), nEvents_(po.maxEvents), verbose_(po.verbose)
{
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
    try {
        reader.init(src);
    } catch(const std::exception& e) {
        std::cout << e.what() << '\n';
        throw;
    }

    // _________________________________________________________________________
    // Loop over all events

    patternBank_map_.clear();
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
            bankSize = patternBank_map_.size();
            coverage = 1.0 - float(bankSize - bankSizeOld) / float(nKept - nKeptOld);

            if (verbose_>1)  std::cout << Form("... Processing event: %7lld, keeping: %7ld, # patterns: %7ld, coverage: %7.5f", ievt, nKept, bankSize, coverage) << std::endl;

            bankSizeOld = bankSize;
            nKeptOld = nKept;
        }

        const unsigned nstubs = reader.vb_ichamber->size();
        if (verbose_>2)  std::cout << "... evt: " << ievt << " # stubs: " << nstubs << std::endl;

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
            uint16_t isector     = reader.vb_isector    ->at(istub);
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

            if (verbose_>2) {
                std::cout << "... ... stub: " << istub << " moduleId: " << cscID << " strip: " << strip << " segment: " << keywire << " rho: " << globalRho << " phi: " << globalPhi << " theta: " << globalTheta << " ds: " << pattern << std::endl;
                std::cout << "... ... stub: " << istub << " ssId: " << ssId << std::endl;
            }
        }

        // Insert pattern into the bank
        typedef std::pair<std::map<pattern_type, pattern_attr>::iterator, bool> map_return_type;
        map_return_type ins = patternBank_map_.insert(std::make_pair(patt, attr));

        // Update the pattern attribute
        fill_attribute(ins.first->second, simChargeOverPt, simCotTheta, simPhi, simVz);
        //fill_attribute(ins.first->second, simChargeOverPt, simCotTheta, simPt, simCharge);  //FIXME

        if (verbose_>2)  std::cout << "... evt: " << ievt << " patt: " << patt << std::endl;

        ++nKept;
        ++nRead;
    }

    if (nRead == 0) {
        throw std::runtime_error("Failed to read any event.");
    }

    if (verbose_)  std::cout << Form("Read: %7ld, kept: %7ld, # patterns: %7lu, coverage: %7.5f", nRead, nKept, patternBank_map_.size(), coverage) << std::endl;

    // Save these numbers
    coverage_       = coverage;
    coverage_count_ = nKept;

    // _________________________________________________________________________
    // Sort by popularity

    // Convert map to vector of pairs
    const unsigned origSize = patternBank_map_.size();

    for (std::map<pattern_type, pattern_attr>::const_iterator it = patternBank_map_.begin();
         it != patternBank_map_.end(); ) {  // should not cause bad_alloc
        patternBank_pairs_.push_back(*it);
        it = patternBank_map_.erase(it);
    }
    assert(patternBank_pairs_.size() == origSize);

    // Clear map and release memory
    std::map<pattern_type, pattern_attr> emptyMap;
    patternBank_map_.clear();
    patternBank_map_.swap(emptyMap);

    // Sort by popularity
    std::stable_sort(patternBank_pairs_.begin(), patternBank_pairs_.end(), sortByPopularity);

    if (verbose_>2) {
        for (unsigned i=0; i<patternBank_pairs_.size(); ++i) {
            const std::pair<pattern_type, pattern_attr>& apair = patternBank_pairs_.at(i);
            std::cout << "... patt: " << i << "  " << apair.first << " popularity: " << apair.second.n << std::endl;
        }
    }

    unsigned max_popularity = patternBank_pairs_.size() ? patternBank_pairs_.front().second.n : 0;
    if (verbose_)  std::cout << "Generated " << patternBank_pairs_.size() << " patterns, max popularity: " << max_popularity << std::endl;
    assert(patternBank_pairs_.front().second.n <= MAX_POPULARITY);
}

// _____________________________________________________________________________
// Write pattern bank
void PatternGenerator::writePatterns(TString out) {

    // _________________________________________________________________________
    // For writing
    PatternBankWriter writer(verbose_);
    try {
        writer.init(out);
    } catch(const std::exception& e) {
        std::cout << e.what() << '\n';
        throw;
    }

    // _________________________________________________________________________
    // Save pattern bank info
    *(writer.pb_coverage)      = coverage_;
    *(writer.pb_count)         = coverage_count_;
    *(writer.pb_tower)         = po_.tower;
    *(writer.pb_sector)        = po_.sector;
    *(writer.pb_superstrip)    = po_.superstrip;
    *(writer.pb_superstrip_nx) = arbiter_->nx();
    *(writer.pb_superstrip_nz) = arbiter_->nz();
    writer.fillPatternBankInfo();

    // _________________________________________________________________________
    // Save pattern bank
    const long long npatterns = patternBank_pairs_.size();

    // Bookkeepers
    unsigned nKept = 0;
    unsigned pop = MAX_POPULARITY, oldPop = MAX_POPULARITY;
    int n90=0, n95=0, n99=0;

    for (long long ipatt=0; ipatt<npatterns; ++ipatt) {
        if (ipatt%1000==0) {
            float coverage = float(nKept) / coverage_count_ * coverage_;
            if (coverage < 0.90 + 1e-5)
                n90 = ipatt;
            if (coverage < 0.95 + 1e-5)
                n95 = ipatt;
            if (coverage < 0.99 + 1e-5)
                n99 = ipatt;

            if (verbose_>1)  std::cout << Form("... Writing event: %7lld, sorted coverage: %7.5f", ipatt, coverage) << std::endl;
        }

        const pattern_attr& attr = patternBank_pairs_.at(ipatt).second;
        pop = attr.n;

        // Check whether patterns are indeed sorted by popularity
        assert(oldPop >= pop);
        oldPop = pop;
        nKept += pop;

        if (pop < (unsigned) po_.minPopularity)  // cut off
            break;

        writer.pb_superstripIds->clear();
        const pattern_type& patt = patternBank_pairs_.at(ipatt).first;
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
    assert(coverage_count_ == nKept);

    if (verbose_)  {
        std::cout << "After sorting by popularity: " << std::endl;
        std::cout << "  max coverage = " << coverage_ << std::endl;
        std::cout << "  N(90% cov) = " << n90 << " popularity = " << patternBank_pairs_.at(n90).second.n << std::endl;
        std::cout << "  N(95% cov) = " << n95 << " popularity = " << patternBank_pairs_.at(n95).second.n << std::endl;
        std::cout << "  N(99% cov) = " << n99 << " popularity = " << patternBank_pairs_.at(n99).second.n << std::endl;
    }
}

#include "L1TMuonSimulations/EMTFSimulation/interface/PatternMatcher.h"

#include "L1TMuonSimulations/EMTFSimulationIO/interface/MessageLogger.h"
#include "L1TMuonSimulations/EMTFSimulationIO/interface/CSCStubReader.h"
#include "L1TMuonSimulations/EMTFSimulationIO/interface/PatternBankReader.h"
#include "L1TMuonSimulations/EMTFSimulationIO/interface/EMTFRoadReader.h"
using namespace phasetwoemtf;


// _____________________________________________________________________________
PatternMatcher::PatternMatcher(const ProgramOption& po)
: po_(po), nEvents_(po.maxEvents), firstEvent_(po.skipEvents), verbose_(po.verbose),
  prefixRoad_("EMTFRoads_"), suffix_("")
{
    arbiter_.reset(new SuperstripArbiter());
    arbiter_ -> setDefinition(po_.superstrip);

    associativeMemory_.reset(new AssociativeMemory());

    hitBuffer_.reset(new HitBuffer());
}

PatternMatcher::~PatternMatcher() {}

void PatternMatcher::init() {}

void PatternMatcher::run() {
    loadPatterns(po_.bankfile);
    matchPatterns(po_.input, po_.output);
}


// _____________________________________________________________________________
// Make pattern bank
void PatternMatcher::loadPatterns(TString bank) {

    LogInfo("PatternMatcher", verbose_) << "Loading patterns from " << bank << std::endl;

    // _________________________________________________________________________
    // For reading pattern bank
    PatternBankReader pbreader(verbose_);
    pbreader.init(bank);

    long long npatterns = pbreader.getEntries();
    if (npatterns > po_.maxPatterns)
        npatterns = po_.maxPatterns;
    assert(npatterns > 0);

    // Setup hit buffer
    const unsigned nss = arbiter_ -> nsuperstripsPerLayer();
    hitBuffer_ -> init(po_.nLayers, nss);

    // Setup associative memory
    associativeMemory_ -> init(npatterns);
    LogInfo("PatternMatcher", verbose_) << "Assume " << nss << " possible superstrips per layer, " << npatterns << " patterns." << std::endl;

    // _________________________________________________________________________
    // Load the patterns

    for (long long ipatt=0; ipatt<npatterns; ++ipatt) {
        pbreader.getEntry(ipatt);
        if (pbreader.pb_popularity < po_.minPopularity)
            break;

        assert(pbreader.pb_superstripIds->size() == po_.nLayers);

        if (verbose_>3) {
            LogDebug("PatternMatcher", verbose_) << "... patt: " << ipatt << "  " << pbreader.pb_superstripIds << std::endl;
        }

        // Fill the associative memory
        associativeMemory_ -> insert(pbreader.pb_superstripIds->begin(), pbreader.pb_superstripIds->end(), pbreader.pb_invPt_mean);
    }

    associativeMemory_ -> freeze();
    assert(associativeMemory_ -> size() == npatterns);

    LogInfo("PatternMatcher", verbose_) << "Successfully loaded " << npatterns << " patterns." << std::endl;
}

// _____________________________________________________________________________
void PatternMatcher::matchPatterns(TString src, TString out) {

    LogInfo("PatternMatcher", verbose_) << "Using nEvents " << nEvents_ << " firstEvent " << firstEvent_ << std::endl;
    LogInfo("PatternMatcher", verbose_) << "Using superstrip " << arbiter_ -> str() << std::endl;

    // _________________________________________________________________________
    // For reading
    CSCStubReader reader(verbose_);
    reader.init(src);

    // For writing
    EMTFRoadWriter writer(verbose_);
    writer.init(reader.getChain(), out, prefixRoad_, suffix_);


    // _________________________________________________________________________
    // Loop over all events

    // Containers
    std::vector<EMTFRoad> roads;
    roads.reserve(200);

    // Bookkeepers
    long int nRead = 0, nKept = 0;

    for (long long ievt=firstEvent_; ievt<nEvents_; ++ievt) {
        if (reader.loadTree(ievt) < 0)  break;
        reader.getEntry(ievt);

        if (ievt%100==0) {
            LogInfo("PatternMatcher", verbose_) << Form("... Processing event: %7lld, triggering: %7ld", ievt, nKept) << std::endl;
        }

        const unsigned nstubs = reader.vb_moduleId->size();
        LogDebug("PatternMatcher", verbose_) << "... evt: " << ievt << " # stubs: " << nstubs << std::endl;

        if (!nstubs) {  // skip if no stub
            ++nRead;
            writer.fill(std::vector<EMTFRoad>());
            continue;
        }


        // _____________________________________________________________________
        // Start pattern recognition
        hitBuffer_ -> reset();

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
            // Find superstrip ID
            unsigned ssId = 0;

            if (moduleId == 999999) {
                ssId = 0xffffffff;

            } else if (arbiter_ -> getCoordType() == SuperstripCoordType::LOCAL) {
                ssId = arbiter_ -> superstripLocal(moduleId, strip, keywire, pattern);

            } else if (arbiter_ -> getCoordType() == SuperstripCoordType::GLOBAL) {
                ssId = arbiter_ -> superstripGlobal(moduleId, globalRho, globalPhi, globalTheta, pattern);
            }

            unsigned layME = decodeLayerME(moduleId);

            // Push into hit buffer
            hitBuffer_ -> insert(layME, ssId, istub);

            if (verbose_>2) {
                LogDebug("PatternMatcher", verbose_) << "... ... stub: " << istub << " moduleId: " << moduleId << " strip: " << strip << " segment: " << keywire << " rho: " << globalRho << " phi: " << globalPhi << " theta: " << globalTheta << " ds: " << pattern << std::endl;
                LogDebug("PatternMatcher", verbose_) << "... ... stub: " << istub << " ssId: " << ssId << " layME: " << layME << std::endl;
            }
        }

        hitBuffer_ -> freeze(po_.maxStubs);

        // _____________________________________________________________________
        // Perform associative memory lookup
        const std::vector<unsigned>& firedPatterns = associativeMemory_ -> lookup(*hitBuffer_, po_.nLayers, po_.maxMisses);


        // _____________________________________________________________________
        // Create roads
        roads.clear();

        // Collect stubs
        for (std::vector<unsigned>::const_iterator it = firedPatterns.begin(); it != firedPatterns.end(); ++it) {
            // Create and set EMTFRoad
            EMTFRoad aroad;
            aroad.patternRef   = (*it);
            aroad.sector       = po_.sector;
            aroad.nstubs       = 0;
            aroad.patternInvPt = 0.;

            // Retrieve the superstripIds and other attributes
            pattern_t patt;
            associativeMemory_ -> retrieve(aroad.patternRef, patt, aroad.patternInvPt);

            aroad.superstripIds.clear();
            aroad.stubRefs.clear();
            aroad.superstripIds.resize(po_.nLayers);
            aroad.stubRefs.resize(po_.nLayers);

            for (unsigned layer=0; layer<po_.nLayers; ++layer) {
                const unsigned ssId = patt.at(layer);

                if (hitBuffer_ -> hasHits(layer, ssId)) {
                    const std::vector<unsigned>& stubRefs = hitBuffer_ -> getHits(layer, ssId);
                    aroad.superstripIds.at(layer) = ssId;
                    aroad.stubRefs     .at(layer) = stubRefs;
                    aroad.nstubs                 += stubRefs.size();

                } else {
                    aroad.superstripIds.at(layer) = ssId;
                }
            }

            roads.push_back(aroad);  // save aroad

            LogDebug("PatternMatcher", verbose_) << "... ... road: " << roads.size() - 1 << " " << aroad << std::endl;

            if (roads.size() >= (unsigned) po_.maxRoads)
                break;
        }

        if (! roads.empty())
            ++nKept;

        writer.fill(roads);
        ++nRead;
    }

    LogInfo("PatternMatcher", verbose_) << Form("Read: %7ld, triggered: %7ld", nRead, nKept) << std::endl;

    long long nentries = writer.writeTree();
    assert(nentries == nRead);
}

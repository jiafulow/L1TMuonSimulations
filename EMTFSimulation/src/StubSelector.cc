#include "L1TMuonSimulations/EMTFSimulation/interface/StubSelector.h"

#include "L1TMuonSimulations/EMTFSimulationIO/interface/MessageLogger.h"
#include "L1TMuonSimulations/EMTFSimulationIO/interface/CSCStubReader.h"
using namespace phasetwoemtf;


static const unsigned MIN_NGOODSTUBS = 3;
static const unsigned MAX_NGOODSTUBS = 8;

namespace {
    // Select elements in a vector according to indices
    template<typename T>
    void selectVectorElements(std::vector<T>& vec, const std::vector<unsigned>& indices) {
        std::vector<T> buffer;
        for (unsigned i=0; i<indices.size(); i++) {
            unsigned index = indices.at(i);

            if (index != 999999) {
                buffer.push_back(vec.at(index));
            } else {
                buffer.push_back(T());  // CUIDADO: missing stub
            }
        }
        vec = buffer;
    }

    template<typename T>
    void flagMissingVectorElements(std::vector<T>& vec, const std::vector<unsigned>& indices) {
        for (unsigned i=0; i<indices.size(); i++) {
            unsigned index = indices.at(i);

            if (index == 999999) {
                vec.at(i) = 999999;
            }
        }
    }
}  // namespace


// _____________________________________________________________________________
StubSelector::StubSelector(const ProgramOption& po)
: po_(po), nEvents_(po.maxEvents), firstEvent_(po.skipEvents), verbose_(po.verbose)
{
    algo_.reset(new StubSelectorAlgo());
}

StubSelector::~StubSelector() {}

void StubSelector::init() {}

void StubSelector::run() {
    selectStubs(po_.input, po_.output);
}


// _____________________________________________________________________________
// Select one unique stub per layer
void StubSelector::selectStubs(TString src, TString out) {
    LogInfo("StubSelector", verbose_) << "Using nEvents " << nEvents_ << " firstEvent " << firstEvent_ << std::endl;

    //const int good_tpId = 0;

    // _________________________________________________________________________
    // For reading
    CSCStubReader reader(verbose_);
    reader.init(src);

    // For writing
    CSCStubWriter writer(verbose_);
    writer.init(reader.getChain(), out);

    // _________________________________________________________________________
    // Loop over all events

    // Bookkeepers
    long int nRead1 = 0, nKept1 = 0;
    long int nRead2 = 0, nKept2 = 0;

    for (long long ievt=firstEvent_; ievt<nEvents_; ++ievt) {
        if (reader.loadTree(ievt) < 0)  break;
        reader.getEntry(ievt);

        const unsigned nstubs = reader.vb_moduleId->size();
        const unsigned nparts = reader.vp_pt->size();

        if (ievt%10000==0) {
            LogInfo("StubSelector", verbose_) << Form("... Processing event: %7lld, keeping: %7ld", ievt, nKept1) << std::endl;
        }

        LogDebug("StubSelector", verbose_) << "... evt: " << ievt << " # stubs: " << nstubs << " # particles: " << nparts << std::endl;

        if (nstubs >= 999999) {
            throw std::runtime_error("Way too many stubs.");
        }

        // _____________________________________________________________________
        // Loop over genParticles

        std::vector<std::vector<unsigned> > evt_selectedStubRefs;

        for (unsigned ipart=0; ipart<nparts; ++ipart) {

            // _________________________________________________________________
            // Start stub selection

            // Events that fail don't exit the loop immediately, so that event info
            // can still be printed when verbosity is turned on.
            bool keep = true;

            // Check min # of stubs
            bool require = (nstubs >= MIN_NGOODSTUBS);
            if (!require)
                keep = false;

            // Check sim info
            assert(reader.vp_pt->size() == 1);
            float simPt       = reader.vp_pt->at(ipart);
            float simEta      = reader.vp_eta->at(ipart);
            float simPhi      = reader.vp_phi->at(ipart);
            //float simVx       = reader.vp_vx->at(ipart);
            //float simVy       = reader.vp_vy->at(ipart);
            float simVz       = reader.vp_vz->at(ipart);
            int   simCharge   = reader.vp_charge->at(ipart);
            //float simCotTheta = std::sinh(simEta);
            float simInvPt    = float(simCharge)/simPt;

            // Apply pt, eta, phi requirements
            bool sim = (po_.minPt  <= simPt  && simPt  <= po_.maxPt  &&
                        po_.minEta <= simEta && simEta <= po_.maxEta &&
                        po_.minPhi <= simPhi && simPhi <= po_.maxPhi &&
                        po_.minVz  <= simVz  && simVz  <= po_.maxVz);
            if (!sim)
                keep = false;

            LogDebug("StubSelector", verbose_) << "... evt: " << ievt << " simPt: " << simPt << " simEta: " << simEta << " simPhi: " << simPhi << " simVz: " << simVz << " simInvPt: " << simInvPt << " keep? " << keep << std::endl;

            // _____________________________________________________________________
            // Filter multiple stubs in one layer

            if (keep) {
                // This assumes 5 endcap disks (ME1/1, ME1/2, ME2, ME3, ME4)
                assert(reader.vp_globalPhiME->at(ipart).size()   == 5);
                assert(reader.vp_globalThetaME->at(ipart).size() == 5);

                std::vector<std::vector<unsigned> > stubRefs(5, std::vector<unsigned>());
                std::vector<std::vector<unsigned> > moduleIds(5, std::vector<unsigned>());
                std::vector<std::vector<float> > globalPhiStubs(5, std::vector<float>());
                std::vector<std::vector<float> > globalThetaStubs(5, std::vector<float>());

                for (unsigned istub=0; istub<nstubs; ++istub) {
                    //int tpId = reader.vb_tpId->at(istub);  // check sim info
                    //if (tpId != good_tpId)
                    //    continue;

                    uint32_t moduleId    = reader.vb_moduleId     ->at(istub);
                    float    globalPhi   = reader.vb_globalPhi    ->at(istub);
                    float    globalTheta = reader.vb_globalTheta  ->at(istub);

                    unsigned layME = decodeLayerME(moduleId);
                    stubRefs.at(layME).push_back(istub);
                    moduleIds.at(layME).push_back(moduleId);
                    globalPhiStubs.at(layME).push_back(globalPhi);
                    globalThetaStubs.at(layME).push_back(globalTheta);

                    LogDebug("StubSelector", verbose_) << "... ... stub: " << istub << " moduleId: " << moduleId << " globalPhi: " << globalPhi << " globalTheta: " << globalTheta << std::endl;
                }

                const std::vector<unsigned>& selectedStubRefs = algo_->select(
                        stubRefs, moduleIds, globalPhiStubs, globalThetaStubs,
                        reader.vp_globalPhiME->at(ipart), reader.vp_globalThetaME->at(ipart) );
                assert(selectedStubRefs.size() == 5);  // 5 endcap disks
                LogDebug("StubSelector", verbose_) << "... evt: " << ievt << " selectedStubRefs: [" << selectedStubRefs << "]" << std::endl;

                evt_selectedStubRefs.push_back(selectedStubRefs);

            } else {
                evt_selectedStubRefs.push_back(std::vector<unsigned>());
            }
        }  // end loop over genParticles

        assert(evt_selectedStubRefs.size() == nparts);


        // Modify branches to keep only the selected stubs
        bool keepEvent = false;

        for (unsigned ipart=0; ipart<nparts; ++ipart) {
            const std::vector<unsigned>& selectedStubRefs = evt_selectedStubRefs.at(ipart);
            const std::vector<unsigned> selectedPartRefs(1, ipart);

            if (selectedStubRefs.empty()) {  // don't output if empty
                ++nRead2;
                continue;
            }

            reader.getEntry(ievt);  // always read from source before modifying

            selectVectorElements(*reader.vp_pt            , selectedPartRefs);
            selectVectorElements(*reader.vp_eta           , selectedPartRefs);
            selectVectorElements(*reader.vp_phi           , selectedPartRefs);
            selectVectorElements(*reader.vp_vx            , selectedPartRefs);
            selectVectorElements(*reader.vp_vy            , selectedPartRefs);
            selectVectorElements(*reader.vp_vz            , selectedPartRefs);
            selectVectorElements(*reader.vp_charge        , selectedPartRefs);
            selectVectorElements(*reader.vp_pdgId         , selectedPartRefs);
            selectVectorElements(*reader.vp_status        , selectedPartRefs);

            selectVectorElements(*reader.vp_globalPhiME   , selectedPartRefs);
            selectVectorElements(*reader.vp_globalThetaME , selectedPartRefs);
            selectVectorElements(*reader.vp_globalEtaME   , selectedPartRefs);
            selectVectorElements(*reader.vp_globalRhoME   , selectedPartRefs);
            selectVectorElements(*reader.vp_globalPxME    , selectedPartRefs);
            selectVectorElements(*reader.vp_globalPyME    , selectedPartRefs);
            selectVectorElements(*reader.vp_globalPzME    , selectedPartRefs);

            selectVectorElements(*reader.vb_globalPhi     , selectedStubRefs);
            selectVectorElements(*reader.vb_globalTheta   , selectedStubRefs);
            selectVectorElements(*reader.vb_globalEta     , selectedStubRefs);
            selectVectorElements(*reader.vb_globalRho     , selectedStubRefs);
            selectVectorElements(*reader.vb_globalX       , selectedStubRefs);
            selectVectorElements(*reader.vb_globalY       , selectedStubRefs);
            selectVectorElements(*reader.vb_globalZ       , selectedStubRefs);
            selectVectorElements(*reader.vb_geoId         , selectedStubRefs);
            selectVectorElements(*reader.vb_moduleId      , selectedStubRefs);
            selectVectorElements(*reader.vb_isector       , selectedStubRefs);
            selectVectorElements(*reader.vb_isubsector    , selectedStubRefs);
            selectVectorElements(*reader.vb_keywire       , selectedStubRefs);
            selectVectorElements(*reader.vb_strip         , selectedStubRefs);
            selectVectorElements(*reader.vb_pattern       , selectedStubRefs);
            selectVectorElements(*reader.vb_cscID         , selectedStubRefs);

            // Flag 999999
            flagMissingVectorElements(*reader.vb_moduleId      , selectedStubRefs);

            ++nKept2;
            keepEvent = true;
            ++nRead2;
            writer.fill();
        }  // end loop over evt_selectedStubRefs

        if (keepEvent)
            ++nKept1;
        ++nRead1;
    }  // end loop over events

    if (nRead1 == 0) {
        throw std::runtime_error("Failed to read any event.");
    }

    LogInfo("StubSelector", verbose_) << Form("Read event   : %7ld, kept: %7ld", nRead1, nKept1) << std::endl;
    LogInfo("StubSelector", verbose_) << Form("Read particle: %7ld, kept: %7ld", nRead2, nKept2) << std::endl;

    long long nentries = writer.writeTree();
    assert(nentries == nKept2);
}

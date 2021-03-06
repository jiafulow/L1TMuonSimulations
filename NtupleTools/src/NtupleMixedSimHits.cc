#include "L1TMuonSimulations/NtupleTools/interface/NtupleMixedSimHits.h"

#include "L1TMuonSimulations/MuonTools/interface/ModuleIdHelper.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleCollectionMap.h"


NtupleMixedSimHits::NtupleMixedSimHits(const edm::ParameterSet& iConfig) :
  inputTag_  (iConfig.getParameter<edm::InputTag>("inputTag")),
  trkPartTag_(iConfig.getParameter<edm::InputTag>("trkPartTag")),
  simHitCollectionConfig_(iConfig.getParameter<edm::ParameterSet>("simHitCollections")),
  prefix_    (iConfig.getParameter<std::string>("prefix")),
  suffix_    (iConfig.getParameter<std::string>("suffix")),
  selector_  (iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_      (iConfig.getParameter<unsigned>("maxN")) {

    trkPartToken_ = consumes<TrackingParticleCollection>(trkPartTag_);

    const std::vector<std::string>& parameterNames = simHitCollectionConfig_.getParameterNames();
    for (unsigned ipn=0; ipn<parameterNames.size(); ++ipn) {
        const std::string parameterName = parameterNames.at(ipn);
        const std::vector<edm::InputTag>& collectionTags = simHitCollectionConfig_.getParameter<std::vector<edm::InputTag> >(parameterName);
        for (unsigned ict=0; ict<collectionTags.size(); ++ict) {
            const edm::InputTag collectionTag = collectionTags.at(ict);
            simHitTags_.push_back(collectionTag);
            simHitTokens_.push_back(consumes<CrossingFrame<PSimHit> >(collectionTag));
        }
    }

    produces<std::vector<float> >    (prefix_ + "x"            + suffix_);
    produces<std::vector<float> >    (prefix_ + "y"            + suffix_);
    produces<std::vector<float> >    (prefix_ + "z"            + suffix_);
    produces<std::vector<float> >    (prefix_ + "r"            + suffix_);
    produces<std::vector<float> >    (prefix_ + "eta"          + suffix_);
    produces<std::vector<float> >    (prefix_ + "phi"          + suffix_);
    produces<std::vector<float> >    (prefix_ + "localx"       + suffix_);
    produces<std::vector<float> >    (prefix_ + "localy"       + suffix_);
    produces<std::vector<float> >    (prefix_ + "localz"       + suffix_);
    produces<std::vector<unsigned> > (prefix_ + "modId"        + suffix_);
    produces<std::vector<unsigned> > (prefix_ + "geoId"        + suffix_);
    produces<std::vector<unsigned> > (prefix_ + "subdet"       + suffix_);
    //produces<std::vector<float> >    (prefix_ + "pabs"         + suffix_);
    //produces<std::vector<float> >    (prefix_ + "energyLoss"   + suffix_);
    //produces<std::vector<float> >    (prefix_ + "thetaAtEntry" + suffix_);
    //produces<std::vector<float> >    (prefix_ + "phiAtEntry"   + suffix_);
    //produces<std::vector<float> >    (prefix_ + "tof"          + suffix_);
    produces<std::vector<int> >      (prefix_ + "particleType" + suffix_);
    produces<std::vector<int> >      (prefix_ + "processType"  + suffix_);
    produces<std::vector<int> >      (prefix_ + "pdgId"        + suffix_);
    produces<std::vector<unsigned> > (prefix_ + "trkId"        + suffix_);
    produces<std::vector<unsigned> > (prefix_ + "evtId"        + suffix_);
    produces<std::vector<int> >      (prefix_ + "tpId"         + suffix_);
    //produces<std::vector<bool> >     (prefix_ + "tofBin"       + suffix_);
    produces<unsigned>               (prefix_ + "size"         + suffix_);
}

NtupleMixedSimHits::~NtupleMixedSimHits() {}

void NtupleMixedSimHits::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    /// Geometry setup
    edm::ESHandle<CSCGeometry> cscGeometryHandle;
    iSetup.get<MuonGeometryRecord>().get(cscGeometryHandle);
    if (!cscGeometryHandle.isValid()) {
        edm::LogWarning("NtupleMixedSimHits") << "Unable to get MuonGeometryRecord!";
    } else {
        theCSCGeometry_ = cscGeometryHandle.product();
    }
}

void NtupleMixedSimHits::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<float> >    v_x           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_y           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_z           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_r           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_eta         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_phi         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_localx      (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_localy      (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_localz      (new std::vector<float>());
    std::auto_ptr<std::vector<unsigned> > v_modId       (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > v_geoId       (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > v_subdet      (new std::vector<unsigned>());
    //std::auto_ptr<std::vector<float> >    v_pabs        (new std::vector<float>());
    //std::auto_ptr<std::vector<float> >    v_energyLoss  (new std::vector<float>());
    //std::auto_ptr<std::vector<float> >    v_thetaAtEntry(new std::vector<float>());
    //std::auto_ptr<std::vector<float> >    v_phiAtEntry  (new std::vector<float>());
    //std::auto_ptr<std::vector<float> >    v_tof         (new std::vector<float>());
    std::auto_ptr<std::vector<int> >      v_particleType(new std::vector<int>());
    std::auto_ptr<std::vector<int> >      v_processType (new std::vector<int>());
    std::auto_ptr<std::vector<int> >      v_pdgId       (new std::vector<int>());
    std::auto_ptr<std::vector<unsigned> > v_trkId       (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> > v_evtId       (new std::vector<unsigned>());
    std::auto_ptr<std::vector<int> >      v_tpId        (new std::vector<int>());
    //std::auto_ptr<std::vector<bool> >     v_tofBin      (new std::vector<bool>());
    std::auto_ptr<unsigned>               v_size        (new unsigned(0));

    //__________________________________________________________________________
    if (!iEvent.isRealData()) {

        /// Prepare a map of simTrack -> trackingParticle
        edm::Handle<TrackingParticleCollection> trackingParticleHandle;
        //iEvent.getByLabel(trkPartTag_, trackingParticleHandle);
        if (!trkPartToken_.isUninitialized())
            iEvent.getByToken(trkPartToken_, trackingParticleHandle);
        TrackingParticleCollectionMap trkToTPMap;
        trkToTPMap.setup(trackingParticleHandle);

        unsigned n = 0;
        for (unsigned ict=0; ict<simHitTags_.size(); ++ict) {
            const edm::InputTag collectionTag = simHitTags_.at(ict);

            edm::Handle<CrossingFrame<PSimHit> > simHits;
            //iEvent.getByLabel(collectionTag, simHits);
            if (!simHitTokens_.at(ict).isUninitialized())
                iEvent.getByToken(simHitTokens_.at(ict), simHits);

            if (simHits.isValid()) {
                std::auto_ptr<MixCollection<PSimHit> > mixedSimHits(new MixCollection<PSimHit>(simHits.product()));
                edm::LogInfo("NtupleMixedSimHits") << "Size: " << mixedSimHits->size();

                for (MixCollection<PSimHit>::iterator it = mixedSimHits->begin(); it != mixedSimHits->end(); ++it) {
                    //if (n >= maxN_)
                    //    break;
                    //if (!selector_(*it))
                    //    continue;

                    const DetId geoId(it->detUnitId());
                    DetId::Detector det = geoId.det();  // Tracker=1,Muon=2,Ecal=3,Hcal=4,Calo=5,Forward=6
                    int subdet = geoId.subdetId();      // DT=1,CSC=2,RPC=3,GEM=4,ME0=5
                    bool isMuon = (det == DetId::Tracker);
                    bool isCSC = (subdet == (int) MuonSubdetId::CSC);
                    //bool tofBin = (collectionTag.instance().find(std::string("HighTof")) != std::string::npos);
                    if (!isMuon)
                        continue;  // only muon
                    if (!isCSC)
                        continue;  // only CSC

                    /// Module ID
                    const uint32_t moduleId = ModuleIdHelper::getModuleId(geoId);

                    const CSCLayer* cscLayer = theCSCGeometry_->layer(geoId);

                    const LocalPoint& localPosition = it->localPosition();
                    const GlobalPoint& globalPosition = cscLayer->surface().toGlobal(localPosition);

                    // Get the matching tracking particle
                    const TrackingParticleCollectionMap::identifier_type myTpId(it->eventId(), it->trackId());
                    int tpId = trkToTPMap.get_index(myTpId);
                    int pdgId = (tpId != -1) ? trackingParticleHandle->at(tpId).pdgId() : -99;


                    // Fill the vectors
                    v_x->push_back(globalPosition.x());
                    v_y->push_back(globalPosition.y());
                    v_z->push_back(globalPosition.z());
                    v_r->push_back(globalPosition.perp());
                    v_eta->push_back(globalPosition.eta());
                    v_phi->push_back(globalPosition.phi());
                    v_localx->push_back(localPosition.x());
                    v_localy->push_back(localPosition.y());
                    v_localz->push_back(localPosition.z());
                    v_modId->push_back(moduleId);
                    v_geoId->push_back(geoId.rawId());
                    v_subdet->push_back(subdet);
                    //v_pabs->push_back(it->pabs());
                    //v_energyLoss->push_back(it->energyLoss());
                    //v_thetaAtEntry->push_back(it->thetaAtEntry());
                    //v_phiAtEntry->push_back(it->phiAtEntry());
                    //v_tof->push_back(it->timeOfFlight());  // in nanoseconds
                    v_particleType->push_back(it->particleType());
                    v_processType->push_back(it->processType());
                    v_pdgId->push_back(pdgId);
                    v_trkId->push_back(it->trackId());
                    v_evtId->push_back(it->eventId().rawId());
                    v_tpId->push_back(tpId);
                    //v_tofBin->push_back(tofBin);  // 0=lowTof, 1=highTof

                    n++;
                }
                *v_size = v_localx->size();

            } else {
                edm::LogError("NtupleMixedSimHits") << "Cannot get the product: " << collectionTag;
            }
        }
    }

    //__________________________________________________________________________
    iEvent.put(v_x           , prefix_ + "x"            + suffix_);
    iEvent.put(v_y           , prefix_ + "y"            + suffix_);
    iEvent.put(v_z           , prefix_ + "z"            + suffix_);
    iEvent.put(v_r           , prefix_ + "r"            + suffix_);
    iEvent.put(v_eta         , prefix_ + "eta"          + suffix_);
    iEvent.put(v_phi         , prefix_ + "phi"          + suffix_);
    iEvent.put(v_localx      , prefix_ + "localx"       + suffix_);
    iEvent.put(v_localy      , prefix_ + "localy"       + suffix_);
    iEvent.put(v_localz      , prefix_ + "localz"       + suffix_);
    iEvent.put(v_modId       , prefix_ + "modId"        + suffix_);
    iEvent.put(v_geoId       , prefix_ + "geoId"        + suffix_);
    iEvent.put(v_subdet      , prefix_ + "subdet"       + suffix_);
    //iEvent.put(v_pabs        , prefix_ + "pabs"         + suffix_);
    //iEvent.put(v_energyLoss  , prefix_ + "energyLoss"   + suffix_);
    //iEvent.put(v_thetaAtEntry, prefix_ + "thetaAtEntry" + suffix_);
    //iEvent.put(v_phiAtEntry  , prefix_ + "phiAtEntry"   + suffix_);
    //iEvent.put(v_tof         , prefix_ + "tof"          + suffix_);
    iEvent.put(v_particleType, prefix_ + "particleType" + suffix_);
    iEvent.put(v_processType , prefix_ + "processType"  + suffix_);
    iEvent.put(v_pdgId       , prefix_ + "pdgId"        + suffix_);
    iEvent.put(v_trkId       , prefix_ + "trkId"        + suffix_);
    iEvent.put(v_evtId       , prefix_ + "evtId"        + suffix_);
    iEvent.put(v_tpId        , prefix_ + "tpId"         + suffix_);
    //iEvent.put(v_tofBin      , prefix_ + "tofBin"       + suffix_);
    iEvent.put(v_size        , prefix_ + "size"         + suffix_);
}

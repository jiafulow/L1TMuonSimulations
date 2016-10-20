#include "L1TMuonSimulations/NtupleTools/interface/NtupleEMTFTriggerTracks.h"


NtupleEMTFTriggerTracks::NtupleEMTFTriggerTracks(const edm::ParameterSet& iConfig) :
  unpStubTag_ (iConfig.getParameter<edm::InputTag>("unpStubTag")),
  unpTrackTag_(iConfig.getParameter<edm::InputTag>("unpTrackTag")),
  unpPrefix_  (iConfig.getParameter<std::string>("unpPrefix")),
  unpSuffix_  (iConfig.getParameter<std::string>("unpSuffix")),
  unpSelector_(iConfig.existsAs<std::string>("unpCut") ? iConfig.getParameter<std::string>("unpCut") : "", true),
  emuStubTag_ (iConfig.getParameter<edm::InputTag>("emuStubTag")),
  emuTrackTag_(iConfig.getParameter<edm::InputTag>("emuTrackTag")),
  emuPrefix_  (iConfig.getParameter<std::string>("emuPrefix")),
  emuSuffix_  (iConfig.getParameter<std::string>("emuSuffix")),
  emuSelector_(iConfig.existsAs<std::string>("emuCut") ? iConfig.getParameter<std::string>("emuCut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    unpStubToken_ = consumes<l1t::EMTFHitCollection>(unpStubTag_);
    unpTrackToken_ = consumes<l1t::EMTFTrackCollection>(unpTrackTag_);
    emuStubToken_ = consumes<l1t::EMTFHitExtraCollection>(emuStubTag_);
    emuTrackToken_ = consumes<l1t::EMTFTrackExtraCollection>(emuTrackTag_);

    produces<std::vector<int16_t> >           (unpPrefix_ + "iendcap"         + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "isector"         + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "isectorGMT"      + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "isectorIndex"    + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "mode"            + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "modeLUT"         + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "quality"         + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "bx"              + unpSuffix_);
    produces<std::vector<float> >             (unpPrefix_ + "pt"              + unpSuffix_);
    produces<std::vector<int> >               (unpPrefix_ + "ptGMT"           + unpSuffix_);
    produces<std::vector<uint64_t> >          (unpPrefix_ + "ptLUTAddr"       + unpSuffix_);
    produces<std::vector<float> >             (unpPrefix_ + "eta"             + unpSuffix_);
    produces<std::vector<int> >               (unpPrefix_ + "etaGMT"          + unpSuffix_);
    produces<std::vector<float> >             (unpPrefix_ + "etaLUT"          + unpSuffix_);
    produces<std::vector<int> >               (unpPrefix_ + "phiLocInt"       + unpSuffix_);
    produces<std::vector<float> >             (unpPrefix_ + "phiLocDeg"       + unpSuffix_);
    produces<std::vector<float> >             (unpPrefix_ + "phiLocRad"       + unpSuffix_);
    produces<std::vector<int> >               (unpPrefix_ + "phiGMT"          + unpSuffix_);
    produces<std::vector<float> >             (unpPrefix_ + "phiGlbDeg"       + unpSuffix_);
    produces<std::vector<float> >             (unpPrefix_ + "phiGlbRad"       + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "charge"          + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "chargeGMT"       + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "chargeValid"     + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "dPhi12"          + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "dPhi13"          + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "dPhi14"          + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "dPhi23"          + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "dPhi24"          + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "dPhi34"          + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "dTheta12"        + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "dTheta13"        + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "dTheta14"        + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "dTheta23"        + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "dTheta24"        + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "dTheta34"        + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "clct1"           + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "clct2"           + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "clct3"           + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "clct4"           + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "fr1"             + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "fr2"             + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "fr3"             + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "fr4"             + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "trackNum"        + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "hasNeighbor"     + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "allNeighbor"     + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "numHits"         + unpSuffix_);
    produces<std::vector<std::vector<unsigned> > > (unpPrefix_ + "stubRefs"        + unpSuffix_);
    produces<unsigned>                        (unpPrefix_ + "size"            + unpSuffix_);

    produces<std::vector<int16_t> >           (emuPrefix_ + "iendcap"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "isector"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "isectorGMT"      + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "isectorIndex"    + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "mode"            + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "modeLUT"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "quality"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "bx"              + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "pt"              + emuSuffix_);
    produces<std::vector<int> >               (emuPrefix_ + "ptGMT"           + emuSuffix_);
    produces<std::vector<uint64_t> >          (emuPrefix_ + "ptLUTAddr"       + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "eta"             + emuSuffix_);
    produces<std::vector<int> >               (emuPrefix_ + "etaGMT"          + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "etaLUT"          + emuSuffix_);
    produces<std::vector<int> >               (emuPrefix_ + "phiLocInt"       + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "phiLocDeg"       + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "phiLocRad"       + emuSuffix_);
    produces<std::vector<int> >               (emuPrefix_ + "phiGMT"          + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "phiGlbDeg"       + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "phiGlbRad"       + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "charge"          + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "chargeGMT"       + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "chargeValid"     + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "dPhi12"          + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "dPhi13"          + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "dPhi14"          + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "dPhi23"          + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "dPhi24"          + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "dPhi34"          + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "dTheta12"        + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "dTheta13"        + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "dTheta14"        + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "dTheta23"        + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "dTheta24"        + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "dTheta34"        + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "clct1"           + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "clct2"           + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "clct3"           + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "clct4"           + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "fr1"             + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "fr2"             + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "fr3"             + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "fr4"             + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "trackNum"        + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "hasNeighbor"     + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "allNeighbor"     + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "numHits"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "firstBX"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "secondBX"        + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "ptXML"           + emuSuffix_);
    produces<std::vector<int> >               (emuPrefix_ + "thetaInt"        + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "thetaDeg"        + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "thetaRad"        + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "type"            + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "rank"            + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "layer"           + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "straightness"    + emuSuffix_);
    produces<std::vector<int> >               (emuPrefix_ + "strip"           + emuSuffix_);
    produces<std::vector<bool> >              (emuPrefix_ + "isGMT"           + emuSuffix_);
    produces<std::vector<int> >               (emuPrefix_ + "numHitsExtra"    + emuSuffix_);
    //produces<std::vector<std::vector<int> > > (emuPrefix_ + "phis"            + emuSuffix_);
    //produces<std::vector<std::vector<int> > > (emuPrefix_ + "thetas"          + emuSuffix_);
    produces<std::vector<std::vector<unsigned> > > (emuPrefix_ + "stubRefs"        + emuSuffix_);
    produces<unsigned>                        (emuPrefix_ + "size"            + emuSuffix_);
}

NtupleEMTFTriggerTracks::~NtupleEMTFTriggerTracks() {}

void NtupleEMTFTriggerTracks::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<int16_t> >           v1_iendcap         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_isector         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_isectorGMT      (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_isectorIndex    (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_mode            (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_modeLUT         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_quality         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_bx              (new std::vector<int16_t>());
    std::auto_ptr<std::vector<float> >             v1_pt              (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v1_ptGMT           (new std::vector<int>());
    std::auto_ptr<std::vector<uint64_t> >          v1_ptLUTAddr       (new std::vector<uint64_t>());
    std::auto_ptr<std::vector<float> >             v1_eta             (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v1_etaGMT          (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v1_etaLUT          (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v1_phiLocInt       (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v1_phiLocDeg       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v1_phiLocRad       (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v1_phiGMT          (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v1_phiGlbDeg       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v1_phiGlbRad       (new std::vector<float>());
    std::auto_ptr<std::vector<int16_t> >           v1_charge          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_chargeGMT       (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_chargeValid     (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_dPhi12          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_dPhi13          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_dPhi14          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_dPhi23          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_dPhi24          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_dPhi34          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_dTheta12        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_dTheta13        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_dTheta14        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_dTheta23        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_dTheta24        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_dTheta34        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_clct1           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_clct2           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_clct3           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_clct4           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_fr1             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_fr2             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_fr3             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_fr4             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_trackNum        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_hasNeighbor     (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_allNeighbor     (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_numHits         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v1_stubRefs        (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<unsigned>                        v1_size            (new unsigned(0));

    std::auto_ptr<std::vector<int16_t> >           v2_iendcap         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_isector         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_isectorGMT      (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_isectorIndex    (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_mode            (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_modeLUT         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_quality         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_bx              (new std::vector<int16_t>());
    std::auto_ptr<std::vector<float> >             v2_pt              (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v2_ptGMT           (new std::vector<int>());
    std::auto_ptr<std::vector<uint64_t> >          v2_ptLUTAddr       (new std::vector<uint64_t>());
    std::auto_ptr<std::vector<float> >             v2_eta             (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v2_etaGMT          (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v2_etaLUT          (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v2_phiLocInt       (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v2_phiLocDeg       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v2_phiLocRad       (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v2_phiGMT          (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v2_phiGlbDeg       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v2_phiGlbRad       (new std::vector<float>());
    std::auto_ptr<std::vector<int16_t> >           v2_charge          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_chargeGMT       (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_chargeValid     (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_dPhi12          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_dPhi13          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_dPhi14          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_dPhi23          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_dPhi24          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_dPhi34          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_dTheta12        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_dTheta13        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_dTheta14        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_dTheta23        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_dTheta24        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_dTheta34        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_clct1           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_clct2           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_clct3           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_clct4           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_fr1             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_fr2             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_fr3             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_fr4             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_trackNum        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_hasNeighbor     (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_allNeighbor     (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_numHits         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_firstBX         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_secondBX        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<float> >             v2_ptXML           (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v2_thetaInt        (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v2_thetaDeg        (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v2_thetaRad        (new std::vector<float>());
    std::auto_ptr<std::vector<int16_t> >           v2_type            (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_rank            (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_layer           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_straightness    (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int> >               v2_strip           (new std::vector<int>());
    std::auto_ptr<std::vector<bool> >              v2_isGMT           (new std::vector<bool>());
    std::auto_ptr<std::vector<int> >               v2_numHitsExtra    (new std::vector<int>());
    //std::auto_ptr<std::vector<std::vector<int> > > v2_phis            (new std::vector<std::vector<int> >());
    //std::auto_ptr<std::vector<std::vector<int> > > v2_thetas          (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v2_stubRefs        (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<unsigned>                        v2_size            (new unsigned(0));


    //__________________________________________________________________________
    edm::Handle<l1t::EMTFHitCollection> unpStubs;
    //iEvent.getByLabel(unpStubTag_, unpStubs);
    if (!unpStubToken_.isUninitialized())
        iEvent.getByToken(unpStubToken_, unpStubs);

    edm::Handle<l1t::EMTFTrackCollection> unpTracks;
    //iEvent.getByLabel(unpTrackTag_, unpTracks);
    if (!unpTrackToken_.isUninitialized())
        iEvent.getByToken(unpTrackToken_, unpTracks);

    if (unpTracks.isValid()) {
        edm::LogInfo("NtupleUnpackedEMTFTriggerTracks") << "Size: " << unpTracks->size();

        unsigned n = 0;
        for (l1t::EMTFTrackCollection::const_iterator it = unpTracks->begin(); it != unpTracks->end(); ++it) {
            if (n >= maxN_)
                break;
            if (!unpSelector_(*it))
                continue;

            // Get stubRefs
            std::vector<unsigned> myStubRefs;
            for (std::vector<unsigned>::const_iterator itb = it->PtrHitIndices()->begin(); itb != it->PtrHitIndices()->end(); ++itb) {
                unsigned myStubRef = *itb;
                myStubRefs.push_back(myStubRef);
            }

            // Fill the vectors
            v1_iendcap         ->push_back(it->Endcap());
            v1_isector         ->push_back(it->Sector());
            v1_isectorGMT      ->push_back(it->Sector_GMT());
            v1_isectorIndex    ->push_back(it->Sector_index());
            v1_mode            ->push_back(it->Mode());
            v1_modeLUT         ->push_back(it->Mode_LUT());
            v1_quality         ->push_back(it->Quality());
            v1_bx              ->push_back(it->BX());
            v1_pt              ->push_back(it->Pt());
            v1_ptGMT           ->push_back(it->Pt_GMT());
            v1_ptLUTAddr       ->push_back(it->Pt_LUT_addr());
            v1_eta             ->push_back(it->Eta());
            v1_etaGMT          ->push_back(it->Eta_GMT());
            v1_etaLUT          ->push_back(it->Eta_LUT());
            v1_phiLocInt       ->push_back(it->Phi_loc_int());
            v1_phiLocDeg       ->push_back(it->Phi_loc_deg());
            v1_phiLocRad       ->push_back(it->Phi_loc_rad());
            v1_phiGMT          ->push_back(it->Phi_GMT());
            v1_phiGlbDeg       ->push_back(it->Phi_glob_deg());
            v1_phiGlbRad       ->push_back(it->Phi_glob_rad());
            v1_charge          ->push_back(it->Charge());
            v1_chargeGMT       ->push_back(it->Charge_GMT());
            v1_chargeValid     ->push_back(it->Charge_valid());
            v1_dPhi12          ->push_back(it->DPhi_12());
            v1_dPhi13          ->push_back(it->DPhi_13());
            v1_dPhi14          ->push_back(it->DPhi_14());
            v1_dPhi23          ->push_back(it->DPhi_23());
            v1_dPhi24          ->push_back(it->DPhi_24());
            v1_dPhi34          ->push_back(it->DPhi_34());
            v1_dTheta12        ->push_back(it->DTheta_12());
            v1_dTheta13        ->push_back(it->DTheta_13());
            v1_dTheta14        ->push_back(it->DTheta_14());
            v1_dTheta23        ->push_back(it->DTheta_23());
            v1_dTheta24        ->push_back(it->DTheta_24());
            v1_dTheta34        ->push_back(it->DTheta_34());
            v1_clct1           ->push_back(it->CLCT_1());
            v1_clct2           ->push_back(it->CLCT_2());
            v1_clct3           ->push_back(it->CLCT_3());
            v1_clct4           ->push_back(it->CLCT_4());
            v1_fr1             ->push_back(it->FR_1());
            v1_fr2             ->push_back(it->FR_2());
            v1_fr3             ->push_back(it->FR_3());
            v1_fr4             ->push_back(it->FR_4());
            v1_trackNum        ->push_back(it->Track_num());
            v1_hasNeighbor     ->push_back(it->Has_neighbor());
            v1_allNeighbor     ->push_back(it->All_neighbor());
            v1_numHits         ->push_back(it->NumHits());
            v1_stubRefs        ->push_back(myStubRefs);
        }
        *v1_size = v1_iendcap->size();

    } else {
        edm::LogError("NtupleUnpackedEMTFTriggerTracks") << "Cannot get the product: " << unpTrackTag_;
    }


    //__________________________________________________________________________
    edm::Handle<l1t::EMTFHitExtraCollection> emuStubs;
    //iEvent.getByLabel(emuStubTag_, emuStubs);
    if (!emuStubToken_.isUninitialized())
        iEvent.getByToken(emuStubToken_, emuStubs);

    edm::Handle<l1t::EMTFTrackExtraCollection> emuTracks;
    //iEvent.getByLabel(emuTrackTag_, emuTracks);
    if (!emuTrackToken_.isUninitialized())
        iEvent.getByToken(emuTrackToken_, emuTracks);

    if (emuTracks.isValid()) {
        edm::LogInfo("NtupleEMTFTriggerTracks") << "Size: " << emuTracks->size();

        unsigned n = 0;
        for (l1t::EMTFTrackExtraCollection::const_iterator it = emuTracks->begin(); it != emuTracks->end(); ++it) {
            if (n >= maxN_)
                break;
            if (!emuSelector_(*it))
                continue;

            // Get stubRefs
            std::vector<unsigned> myStubRefs;
            for (std::vector<unsigned>::const_iterator itb = it->PtrHitExtraIndices()->begin(); itb != it->PtrHitExtraIndices()->end(); ++itb) {
                unsigned myStubRef = *itb;
                myStubRefs.push_back(myStubRef);
            }

            // Fill the vectors
            v2_iendcap         ->push_back(it->Endcap());
            v2_isector         ->push_back(it->Sector());
            v2_isectorGMT      ->push_back(it->Sector_GMT());
            v2_isectorIndex    ->push_back(it->Sector_index());
            v2_mode            ->push_back(it->Mode());
            v2_modeLUT         ->push_back(it->Mode_LUT());
            v2_quality         ->push_back(it->Quality());
            v2_bx              ->push_back(it->BX());
            v2_pt              ->push_back(it->Pt());
            v2_ptGMT           ->push_back(it->Pt_GMT());
            v2_ptLUTAddr       ->push_back(it->Pt_LUT_addr());
            v2_eta             ->push_back(it->Eta());
            v2_etaGMT          ->push_back(it->Eta_GMT());
            v2_etaLUT          ->push_back(it->Eta_LUT());
            v2_phiLocInt       ->push_back(it->Phi_loc_int());
            v2_phiLocDeg       ->push_back(it->Phi_loc_deg());
            v2_phiLocRad       ->push_back(it->Phi_loc_rad());
            v2_phiGMT          ->push_back(it->Phi_GMT());
            v2_phiGlbDeg       ->push_back(it->Phi_glob_deg());
            v2_phiGlbRad       ->push_back(it->Phi_glob_rad());
            v2_charge          ->push_back(it->Charge());
            v2_chargeGMT       ->push_back(it->Charge_GMT());
            v2_chargeValid     ->push_back(it->Charge_valid());
            v2_dPhi12          ->push_back(it->DPhi_12());
            v2_dPhi13          ->push_back(it->DPhi_13());
            v2_dPhi14          ->push_back(it->DPhi_14());
            v2_dPhi23          ->push_back(it->DPhi_23());
            v2_dPhi24          ->push_back(it->DPhi_24());
            v2_dPhi34          ->push_back(it->DPhi_34());
            v2_dTheta12        ->push_back(it->DTheta_12());
            v2_dTheta13        ->push_back(it->DTheta_13());
            v2_dTheta14        ->push_back(it->DTheta_14());
            v2_dTheta23        ->push_back(it->DTheta_23());
            v2_dTheta24        ->push_back(it->DTheta_24());
            v2_dTheta34        ->push_back(it->DTheta_34());
            v2_clct1           ->push_back(it->CLCT_1());
            v2_clct2           ->push_back(it->CLCT_2());
            v2_clct3           ->push_back(it->CLCT_3());
            v2_clct4           ->push_back(it->CLCT_4());
            v2_fr1             ->push_back(it->FR_1());
            v2_fr2             ->push_back(it->FR_2());
            v2_fr3             ->push_back(it->FR_3());
            v2_fr4             ->push_back(it->FR_4());
            v2_trackNum        ->push_back(it->Track_num());
            v2_hasNeighbor     ->push_back(it->Has_neighbor());
            v2_allNeighbor     ->push_back(it->All_neighbor());
            v2_numHits         ->push_back(it->NumHits());
            v2_firstBX         ->push_back(it->First_BX());
            v2_secondBX        ->push_back(it->Second_BX());
            v2_ptXML           ->push_back(it->Pt_XML());
            v2_thetaInt        ->push_back(it->Theta_int());
            v2_thetaDeg        ->push_back(it->Theta_deg());
            v2_thetaRad        ->push_back(it->Theta_rad());
            v2_type            ->push_back(it->Type());
            v2_rank            ->push_back(it->Rank());
            v2_layer           ->push_back(it->Layer());
            v2_straightness    ->push_back(it->Straightness());
            v2_strip           ->push_back(it->Strip());
            v2_isGMT           ->push_back(it->IsGMT());
            v2_numHitsExtra    ->push_back(it->NumHitsExtra());
            //v2_phis            ->push_back(it->Phis());
            //v2_thetas          ->push_back(it->Thetas());
            v2_stubRefs        ->push_back(myStubRefs);
        }
        *v2_size = v2_iendcap->size();

    } else {
        edm::LogError("NtupleEMTFTriggerTracks") << "Cannot get the product: " << emuTrackTag_;
    }


    //__________________________________________________________________________
    iEvent.put(v1_iendcap         , unpPrefix_ + "iendcap"         + unpSuffix_);
    iEvent.put(v1_isector         , unpPrefix_ + "isector"         + unpSuffix_);
    iEvent.put(v1_isectorGMT      , unpPrefix_ + "isectorGMT"      + unpSuffix_);
    iEvent.put(v1_isectorIndex    , unpPrefix_ + "isectorIndex"    + unpSuffix_);
    iEvent.put(v1_mode            , unpPrefix_ + "mode"            + unpSuffix_);
    iEvent.put(v1_modeLUT         , unpPrefix_ + "modeLUT"         + unpSuffix_);
    iEvent.put(v1_quality         , unpPrefix_ + "quality"         + unpSuffix_);
    iEvent.put(v1_bx              , unpPrefix_ + "bx"              + unpSuffix_);
    iEvent.put(v1_pt              , unpPrefix_ + "pt"              + unpSuffix_);
    iEvent.put(v1_ptGMT           , unpPrefix_ + "ptGMT"           + unpSuffix_);
    iEvent.put(v1_ptLUTAddr       , unpPrefix_ + "ptLUTAddr"       + unpSuffix_);
    iEvent.put(v1_eta             , unpPrefix_ + "eta"             + unpSuffix_);
    iEvent.put(v1_etaGMT          , unpPrefix_ + "etaGMT"          + unpSuffix_);
    iEvent.put(v1_etaLUT          , unpPrefix_ + "etaLUT"          + unpSuffix_);
    iEvent.put(v1_phiLocInt       , unpPrefix_ + "phiLocInt"       + unpSuffix_);
    iEvent.put(v1_phiLocDeg       , unpPrefix_ + "phiLocDeg"       + unpSuffix_);
    iEvent.put(v1_phiLocRad       , unpPrefix_ + "phiLocRad"       + unpSuffix_);
    iEvent.put(v1_phiGMT          , unpPrefix_ + "phiGMT"          + unpSuffix_);
    iEvent.put(v1_phiGlbDeg       , unpPrefix_ + "phiGlbDeg"       + unpSuffix_);
    iEvent.put(v1_phiGlbRad       , unpPrefix_ + "phiGlbRad"       + unpSuffix_);
    iEvent.put(v1_charge          , unpPrefix_ + "charge"          + unpSuffix_);
    iEvent.put(v1_chargeGMT       , unpPrefix_ + "chargeGMT"       + unpSuffix_);
    iEvent.put(v1_chargeValid     , unpPrefix_ + "chargeValid"     + unpSuffix_);
    iEvent.put(v1_dPhi12          , unpPrefix_ + "dPhi12"          + unpSuffix_);
    iEvent.put(v1_dPhi13          , unpPrefix_ + "dPhi13"          + unpSuffix_);
    iEvent.put(v1_dPhi14          , unpPrefix_ + "dPhi14"          + unpSuffix_);
    iEvent.put(v1_dPhi23          , unpPrefix_ + "dPhi23"          + unpSuffix_);
    iEvent.put(v1_dPhi24          , unpPrefix_ + "dPhi24"          + unpSuffix_);
    iEvent.put(v1_dPhi34          , unpPrefix_ + "dPhi34"          + unpSuffix_);
    iEvent.put(v1_dTheta12        , unpPrefix_ + "dTheta12"        + unpSuffix_);
    iEvent.put(v1_dTheta13        , unpPrefix_ + "dTheta13"        + unpSuffix_);
    iEvent.put(v1_dTheta14        , unpPrefix_ + "dTheta14"        + unpSuffix_);
    iEvent.put(v1_dTheta23        , unpPrefix_ + "dTheta23"        + unpSuffix_);
    iEvent.put(v1_dTheta24        , unpPrefix_ + "dTheta24"        + unpSuffix_);
    iEvent.put(v1_dTheta34        , unpPrefix_ + "dTheta34"        + unpSuffix_);
    iEvent.put(v1_clct1           , unpPrefix_ + "clct1"           + unpSuffix_);
    iEvent.put(v1_clct2           , unpPrefix_ + "clct2"           + unpSuffix_);
    iEvent.put(v1_clct3           , unpPrefix_ + "clct3"           + unpSuffix_);
    iEvent.put(v1_clct4           , unpPrefix_ + "clct4"           + unpSuffix_);
    iEvent.put(v1_fr1             , unpPrefix_ + "fr1"             + unpSuffix_);
    iEvent.put(v1_fr2             , unpPrefix_ + "fr2"             + unpSuffix_);
    iEvent.put(v1_fr3             , unpPrefix_ + "fr3"             + unpSuffix_);
    iEvent.put(v1_fr4             , unpPrefix_ + "fr4"             + unpSuffix_);
    iEvent.put(v1_trackNum        , unpPrefix_ + "trackNum"        + unpSuffix_);
    iEvent.put(v1_hasNeighbor     , unpPrefix_ + "hasNeighbor"     + unpSuffix_);
    iEvent.put(v1_allNeighbor     , unpPrefix_ + "allNeighbor"     + unpSuffix_);
    iEvent.put(v1_numHits         , unpPrefix_ + "numHits"         + unpSuffix_);
    iEvent.put(v1_stubRefs        , unpPrefix_ + "stubRefs"        + unpSuffix_);
    iEvent.put(v1_size            , unpPrefix_ + "size"            + unpSuffix_);

    iEvent.put(v2_iendcap         , emuPrefix_ + "iendcap"         + emuSuffix_);
    iEvent.put(v2_isector         , emuPrefix_ + "isector"         + emuSuffix_);
    iEvent.put(v2_isectorGMT      , emuPrefix_ + "isectorGMT"      + emuSuffix_);
    iEvent.put(v2_isectorIndex    , emuPrefix_ + "isectorIndex"    + emuSuffix_);
    iEvent.put(v2_mode            , emuPrefix_ + "mode"            + emuSuffix_);
    iEvent.put(v2_modeLUT         , emuPrefix_ + "modeLUT"         + emuSuffix_);
    iEvent.put(v2_quality         , emuPrefix_ + "quality"         + emuSuffix_);
    iEvent.put(v2_bx              , emuPrefix_ + "bx"              + emuSuffix_);
    iEvent.put(v2_pt              , emuPrefix_ + "pt"              + emuSuffix_);
    iEvent.put(v2_ptGMT           , emuPrefix_ + "ptGMT"           + emuSuffix_);
    iEvent.put(v2_ptLUTAddr       , emuPrefix_ + "ptLUTAddr"       + emuSuffix_);
    iEvent.put(v2_eta             , emuPrefix_ + "eta"             + emuSuffix_);
    iEvent.put(v2_etaGMT          , emuPrefix_ + "etaGMT"          + emuSuffix_);
    iEvent.put(v2_etaLUT          , emuPrefix_ + "etaLUT"          + emuSuffix_);
    iEvent.put(v2_phiLocInt       , emuPrefix_ + "phiLocInt"       + emuSuffix_);
    iEvent.put(v2_phiLocDeg       , emuPrefix_ + "phiLocDeg"       + emuSuffix_);
    iEvent.put(v2_phiLocRad       , emuPrefix_ + "phiLocRad"       + emuSuffix_);
    iEvent.put(v2_phiGMT          , emuPrefix_ + "phiGMT"          + emuSuffix_);
    iEvent.put(v2_phiGlbDeg       , emuPrefix_ + "phiGlbDeg"       + emuSuffix_);
    iEvent.put(v2_phiGlbRad       , emuPrefix_ + "phiGlbRad"       + emuSuffix_);
    iEvent.put(v2_charge          , emuPrefix_ + "charge"          + emuSuffix_);
    iEvent.put(v2_chargeGMT       , emuPrefix_ + "chargeGMT"       + emuSuffix_);
    iEvent.put(v2_chargeValid     , emuPrefix_ + "chargeValid"     + emuSuffix_);
    iEvent.put(v2_dPhi12          , emuPrefix_ + "dPhi12"          + emuSuffix_);
    iEvent.put(v2_dPhi13          , emuPrefix_ + "dPhi13"          + emuSuffix_);
    iEvent.put(v2_dPhi14          , emuPrefix_ + "dPhi14"          + emuSuffix_);
    iEvent.put(v2_dPhi23          , emuPrefix_ + "dPhi23"          + emuSuffix_);
    iEvent.put(v2_dPhi24          , emuPrefix_ + "dPhi24"          + emuSuffix_);
    iEvent.put(v2_dPhi34          , emuPrefix_ + "dPhi34"          + emuSuffix_);
    iEvent.put(v2_dTheta12        , emuPrefix_ + "dTheta12"        + emuSuffix_);
    iEvent.put(v2_dTheta13        , emuPrefix_ + "dTheta13"        + emuSuffix_);
    iEvent.put(v2_dTheta14        , emuPrefix_ + "dTheta14"        + emuSuffix_);
    iEvent.put(v2_dTheta23        , emuPrefix_ + "dTheta23"        + emuSuffix_);
    iEvent.put(v2_dTheta24        , emuPrefix_ + "dTheta24"        + emuSuffix_);
    iEvent.put(v2_dTheta34        , emuPrefix_ + "dTheta34"        + emuSuffix_);
    iEvent.put(v2_clct1           , emuPrefix_ + "clct1"           + emuSuffix_);
    iEvent.put(v2_clct2           , emuPrefix_ + "clct2"           + emuSuffix_);
    iEvent.put(v2_clct3           , emuPrefix_ + "clct3"           + emuSuffix_);
    iEvent.put(v2_clct4           , emuPrefix_ + "clct4"           + emuSuffix_);
    iEvent.put(v2_fr1             , emuPrefix_ + "fr1"             + emuSuffix_);
    iEvent.put(v2_fr2             , emuPrefix_ + "fr2"             + emuSuffix_);
    iEvent.put(v2_fr3             , emuPrefix_ + "fr3"             + emuSuffix_);
    iEvent.put(v2_fr4             , emuPrefix_ + "fr4"             + emuSuffix_);
    iEvent.put(v2_trackNum        , emuPrefix_ + "trackNum"        + emuSuffix_);
    iEvent.put(v2_hasNeighbor     , emuPrefix_ + "hasNeighbor"     + emuSuffix_);
    iEvent.put(v2_allNeighbor     , emuPrefix_ + "allNeighbor"     + emuSuffix_);
    iEvent.put(v2_numHits         , emuPrefix_ + "numHits"         + emuSuffix_);
    iEvent.put(v2_firstBX         , emuPrefix_ + "firstBX"         + emuSuffix_);
    iEvent.put(v2_secondBX        , emuPrefix_ + "secondBX"        + emuSuffix_);
    iEvent.put(v2_ptXML           , emuPrefix_ + "ptXML"           + emuSuffix_);
    iEvent.put(v2_thetaInt        , emuPrefix_ + "thetaInt"        + emuSuffix_);
    iEvent.put(v2_thetaDeg        , emuPrefix_ + "thetaDeg"        + emuSuffix_);
    iEvent.put(v2_thetaRad        , emuPrefix_ + "thetaRad"        + emuSuffix_);
    iEvent.put(v2_type            , emuPrefix_ + "type"            + emuSuffix_);
    iEvent.put(v2_rank            , emuPrefix_ + "rank"            + emuSuffix_);
    iEvent.put(v2_layer           , emuPrefix_ + "layer"           + emuSuffix_);
    iEvent.put(v2_straightness    , emuPrefix_ + "straightness"    + emuSuffix_);
    iEvent.put(v2_strip           , emuPrefix_ + "strip"           + emuSuffix_);
    iEvent.put(v2_isGMT           , emuPrefix_ + "isGMT"           + emuSuffix_);
    iEvent.put(v2_numHitsExtra    , emuPrefix_ + "numHitsExtra"    + emuSuffix_);
    //iEvent.put(v2_phis            , emuPrefix_ + "phis"            + emuSuffix_);
    //iEvent.put(v2_thetas          , emuPrefix_ + "thetas"          + emuSuffix_);
    iEvent.put(v2_stubRefs        , emuPrefix_ + "stubRefs"        + emuSuffix_);
    iEvent.put(v2_size            , emuPrefix_ + "size"            + emuSuffix_);
}

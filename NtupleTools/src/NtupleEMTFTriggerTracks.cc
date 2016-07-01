#include "L1TMuonSimulations/NtupleTools/interface/NtupleEMTFTriggerTracks.h"

#include "L1TMuonSimulations/NtupleTools/interface/MapEMTFTriggerPrimitives.h"


NtupleEMTFTriggerTracks::NtupleEMTFTriggerTracks(const edm::ParameterSet& iConfig) :
  stubTag_ (iConfig.getParameter<edm::InputTag>("stubTag")),
  trackTag_(iConfig.getParameter<edm::InputTag>("trackTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    stubToken_ = consumes<l1t::EMTFHitExtraCollection>(stubTag_);
    trackToken_ = consumes<l1t::EMTFTrackExtraCollection>(trackTag_);

    produces<std::vector<int16_t> >           (prefix_ + "iendcap"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "isector"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "isectorGMT"      + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "isectorIndex"    + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "mode"            + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "modeLUT"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "quality"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "bx"              + suffix_);
    produces<std::vector<float> >             (prefix_ + "pt"              + suffix_);
    produces<std::vector<int> >               (prefix_ + "ptGMT"           + suffix_);
    produces<std::vector<uint64_t> >          (prefix_ + "ptLUTAddr"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "eta"             + suffix_);
    produces<std::vector<int> >               (prefix_ + "etaGMT"          + suffix_);
    produces<std::vector<float> >             (prefix_ + "etaLUT"          + suffix_);
    produces<std::vector<int> >               (prefix_ + "phiLocInt"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "phiLocDeg"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "phiLocRad"       + suffix_);
    produces<std::vector<int> >               (prefix_ + "phiGMT"          + suffix_);
    produces<std::vector<float> >             (prefix_ + "phiGlbDeg"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "phiGlbRad"       + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "charge"          + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "chargeGMT"       + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "chargeValid"     + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "dPhi12"          + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "dPhi13"          + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "dPhi14"          + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "dPhi23"          + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "dPhi24"          + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "dPhi34"          + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "dTheta12"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "dTheta13"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "dTheta14"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "dTheta23"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "dTheta24"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "dTheta34"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "clct1"           + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "clct2"           + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "clct3"           + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "clct4"           + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "fr1"             + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "fr2"             + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "fr3"             + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "fr4"             + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "trackNum"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "hasNeighbor"     + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "allNeighbor"     + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "numHits"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "firstBX"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "secondBX"        + suffix_);
    produces<std::vector<float> >             (prefix_ + "ptXML"           + suffix_);
    produces<std::vector<int> >               (prefix_ + "thetaInt"        + suffix_);
    produces<std::vector<float> >             (prefix_ + "thetaDeg"        + suffix_);
    produces<std::vector<float> >             (prefix_ + "thetaRad"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "type"            + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "rank"            + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "layer"           + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "straightness"    + suffix_);
    produces<std::vector<int> >               (prefix_ + "strip"           + suffix_);
    produces<std::vector<bool> >              (prefix_ + "isGMT"           + suffix_);
    produces<std::vector<int> >               (prefix_ + "numHitsExtra"    + suffix_);
    //produces<std::vector<std::vector<int> > > (prefix_ + "phis"            + suffix_);
    //produces<std::vector<std::vector<int> > > (prefix_ + "thetas"          + suffix_);
    produces<std::vector<std::vector<unsigned> > > (prefix_ + "stubRefs"        + suffix_);
    produces<unsigned>                        (prefix_ + "size"            + suffix_);
}

NtupleEMTFTriggerTracks::~NtupleEMTFTriggerTracks() {}

void NtupleEMTFTriggerTracks::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<int16_t> >           v_iendcap         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_isector         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_isectorGMT      (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_isectorIndex    (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_mode            (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_modeLUT         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_quality         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_bx              (new std::vector<int16_t>());
    std::auto_ptr<std::vector<float> >             v_pt              (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v_ptGMT           (new std::vector<int>());
    std::auto_ptr<std::vector<uint64_t> >          v_ptLUTAddr       (new std::vector<uint64_t>());
    std::auto_ptr<std::vector<float> >             v_eta             (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v_etaGMT          (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v_etaLUT          (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v_phiLocInt       (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v_phiLocDeg       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_phiLocRad       (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v_phiGMT          (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v_phiGlbDeg       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_phiGlbRad       (new std::vector<float>());
    std::auto_ptr<std::vector<int16_t> >           v_charge          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_chargeGMT       (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_chargeValid     (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_dPhi12          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_dPhi13          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_dPhi14          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_dPhi23          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_dPhi24          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_dPhi34          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_dTheta12        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_dTheta13        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_dTheta14        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_dTheta23        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_dTheta24        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_dTheta34        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_clct1           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_clct2           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_clct3           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_clct4           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_fr1             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_fr2             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_fr3             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_fr4             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_trackNum        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_hasNeighbor     (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_allNeighbor     (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_numHits         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_firstBX         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_secondBX        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<float> >             v_ptXML           (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v_thetaInt        (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v_thetaDeg        (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_thetaRad        (new std::vector<float>());
    std::auto_ptr<std::vector<int16_t> >           v_type            (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_rank            (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_layer           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_straightness    (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int> >               v_strip           (new std::vector<int>());
    std::auto_ptr<std::vector<bool> >              v_isGMT           (new std::vector<bool>());
    std::auto_ptr<std::vector<int> >               v_numHitsExtra    (new std::vector<int>());
    //std::auto_ptr<std::vector<std::vector<int> > > v_phis            (new std::vector<std::vector<int> >());
    //std::auto_ptr<std::vector<std::vector<int> > > v_thetas          (new std::vector<std::vector<int> >());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v_stubRefs        (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<unsigned>                        v_size            (new unsigned(0));


    //__________________________________________________________________________
    edm::Handle<l1t::EMTFHitExtraCollection> stubs;
    //iEvent.getByLabel(stubTag_, stubs);
    if (!stubToken_.isUninitialized())
        iEvent.getByToken(stubToken_, stubs);

    MapEMTFTriggerPrimitives stubMap;
    stubMap.setup(stubs);

    //__________________________________________________________________________
    edm::Handle<l1t::EMTFTrackExtraCollection> tracks;
    //iEvent.getByLabel(trackTag_, tracks);
    if (!trackToken_.isUninitialized())
        iEvent.getByToken(trackToken_, tracks);

    if (tracks.isValid()) {
        edm::LogInfo("NtupleEMTFTriggerTracks") << "Size: " << tracks->size();

        unsigned n = 0;
        for (l1t::EMTFTrackExtraCollection::const_iterator it = tracks->begin(); it != tracks->end(); ++it) {
            if (n >= maxN_)
                break;
            if (!selector_(*it))
                continue;

            // Get stubRefs
            std::vector<unsigned> myStubRefs;
            for (std::vector<unsigned>::const_iterator itb = it->PtrHitExtraIndices()->begin(); itb != it->PtrHitExtraIndices()->end(); ++itb) {
                edm::Ref<l1t::EMTFHitExtraCollection> stubRef(stubs, *itb);
                unsigned myStubRef = stubMap.get(stubRef);
                assert(myStubRef == *itb);  // at the moment, MapEMTFTriggerPrimitives doesn't do anything

                myStubRefs.push_back(myStubRef);
            }

            // Fill the vectors
            v_iendcap         ->push_back(it->Endcap());
            v_isector         ->push_back(it->Sector());
            v_isectorGMT      ->push_back(it->Sector_GMT());
            v_isectorIndex    ->push_back(it->Sector_index());
            v_mode            ->push_back(it->Mode());
            v_modeLUT         ->push_back(it->Mode_LUT());
            v_quality         ->push_back(it->Quality());
            v_bx              ->push_back(it->BX());
            v_pt              ->push_back(it->Pt());
            v_ptGMT           ->push_back(it->Pt_GMT());
            v_ptLUTAddr       ->push_back(it->Pt_LUT_addr());
            v_eta             ->push_back(it->Eta());
            v_etaGMT          ->push_back(it->Eta_GMT());
            v_etaLUT          ->push_back(it->Eta_LUT());
            v_phiLocInt       ->push_back(it->Phi_loc_int());
            v_phiLocDeg       ->push_back(it->Phi_loc_deg());
            v_phiLocRad       ->push_back(it->Phi_loc_rad());
            v_phiGMT          ->push_back(it->Phi_GMT());
            v_phiGlbDeg       ->push_back(it->Phi_glob_deg());
            v_phiGlbRad       ->push_back(it->Phi_glob_rad());
            v_charge          ->push_back(it->Charge());
            v_chargeGMT       ->push_back(it->Charge_GMT());
            v_chargeValid     ->push_back(it->Charge_valid());
            v_dPhi12          ->push_back(it->DPhi_12());
            v_dPhi13          ->push_back(it->DPhi_13());
            v_dPhi14          ->push_back(it->DPhi_14());
            v_dPhi23          ->push_back(it->DPhi_23());
            v_dPhi24          ->push_back(it->DPhi_24());
            v_dPhi34          ->push_back(it->DPhi_34());
            v_dTheta12        ->push_back(it->DTheta_12());
            v_dTheta13        ->push_back(it->DTheta_13());
            v_dTheta14        ->push_back(it->DTheta_14());
            v_dTheta23        ->push_back(it->DTheta_23());
            v_dTheta24        ->push_back(it->DTheta_24());
            v_dTheta34        ->push_back(it->DTheta_34());
            v_clct1           ->push_back(it->CLCT_1());
            v_clct2           ->push_back(it->CLCT_2());
            v_clct3           ->push_back(it->CLCT_3());
            v_clct4           ->push_back(it->CLCT_4());
            v_fr1             ->push_back(it->FR_1());
            v_fr2             ->push_back(it->FR_2());
            v_fr3             ->push_back(it->FR_3());
            v_fr4             ->push_back(it->FR_4());
            v_trackNum        ->push_back(it->Track_num());
            v_hasNeighbor     ->push_back(it->Has_neighbor());
            v_allNeighbor     ->push_back(it->All_neighbor());
            v_numHits         ->push_back(it->NumHits());
            v_firstBX         ->push_back(it->First_BX());
            v_secondBX        ->push_back(it->Second_BX());
            v_ptXML           ->push_back(it->Pt_XML());
            v_thetaInt        ->push_back(it->Theta_int());
            v_thetaDeg        ->push_back(it->Theta_deg());
            v_thetaRad        ->push_back(it->Theta_rad());
            v_type            ->push_back(it->Type());
            v_rank            ->push_back(it->Rank());
            v_layer           ->push_back(it->Layer());
            v_straightness    ->push_back(it->Straightness());
            v_strip           ->push_back(it->Strip());
            v_isGMT           ->push_back(it->IsGMT());
            v_numHitsExtra    ->push_back(it->NumHitsExtra());
            //v_phis            ->push_back(it->Phis());
            //v_thetas          ->push_back(it->Thetas());
            v_stubRefs        ->push_back(myStubRefs);
        }
        *v_size = v_iendcap->size();

    } else {
        edm::LogError("NtupleEMTFTriggerTracks") << "Cannot get the product: " << trackTag_;
    }


    //__________________________________________________________________________
    iEvent.put(v_iendcap         , prefix_ + "iendcap"         + suffix_);
    iEvent.put(v_isector         , prefix_ + "isector"         + suffix_);
    iEvent.put(v_isectorGMT      , prefix_ + "isectorGMT"      + suffix_);
    iEvent.put(v_isectorIndex    , prefix_ + "isectorIndex"    + suffix_);
    iEvent.put(v_mode            , prefix_ + "mode"            + suffix_);
    iEvent.put(v_modeLUT         , prefix_ + "modeLUT"         + suffix_);
    iEvent.put(v_quality         , prefix_ + "quality"         + suffix_);
    iEvent.put(v_bx              , prefix_ + "bx"              + suffix_);
    iEvent.put(v_pt              , prefix_ + "pt"              + suffix_);
    iEvent.put(v_ptGMT           , prefix_ + "ptGMT"           + suffix_);
    iEvent.put(v_ptLUTAddr       , prefix_ + "ptLUTAddr"       + suffix_);
    iEvent.put(v_eta             , prefix_ + "eta"             + suffix_);
    iEvent.put(v_etaGMT          , prefix_ + "etaGMT"          + suffix_);
    iEvent.put(v_etaLUT          , prefix_ + "etaLUT"          + suffix_);
    iEvent.put(v_phiLocInt       , prefix_ + "phiLocInt"       + suffix_);
    iEvent.put(v_phiLocDeg       , prefix_ + "phiLocDeg"       + suffix_);
    iEvent.put(v_phiLocRad       , prefix_ + "phiLocRad"       + suffix_);
    iEvent.put(v_phiGMT          , prefix_ + "phiGMT"          + suffix_);
    iEvent.put(v_phiGlbDeg       , prefix_ + "phiGlbDeg"       + suffix_);
    iEvent.put(v_phiGlbRad       , prefix_ + "phiGlbRad"       + suffix_);
    iEvent.put(v_charge          , prefix_ + "charge"          + suffix_);
    iEvent.put(v_chargeGMT       , prefix_ + "chargeGMT"       + suffix_);
    iEvent.put(v_chargeValid     , prefix_ + "chargeValid"     + suffix_);
    iEvent.put(v_dPhi12          , prefix_ + "dPhi12"          + suffix_);
    iEvent.put(v_dPhi13          , prefix_ + "dPhi13"          + suffix_);
    iEvent.put(v_dPhi14          , prefix_ + "dPhi14"          + suffix_);
    iEvent.put(v_dPhi23          , prefix_ + "dPhi23"          + suffix_);
    iEvent.put(v_dPhi24          , prefix_ + "dPhi24"          + suffix_);
    iEvent.put(v_dPhi34          , prefix_ + "dPhi34"          + suffix_);
    iEvent.put(v_dTheta12        , prefix_ + "dTheta12"        + suffix_);
    iEvent.put(v_dTheta13        , prefix_ + "dTheta13"        + suffix_);
    iEvent.put(v_dTheta14        , prefix_ + "dTheta14"        + suffix_);
    iEvent.put(v_dTheta23        , prefix_ + "dTheta23"        + suffix_);
    iEvent.put(v_dTheta24        , prefix_ + "dTheta24"        + suffix_);
    iEvent.put(v_dTheta34        , prefix_ + "dTheta34"        + suffix_);
    iEvent.put(v_clct1           , prefix_ + "clct1"           + suffix_);
    iEvent.put(v_clct2           , prefix_ + "clct2"           + suffix_);
    iEvent.put(v_clct3           , prefix_ + "clct3"           + suffix_);
    iEvent.put(v_clct4           , prefix_ + "clct4"           + suffix_);
    iEvent.put(v_fr1             , prefix_ + "fr1"             + suffix_);
    iEvent.put(v_fr2             , prefix_ + "fr2"             + suffix_);
    iEvent.put(v_fr3             , prefix_ + "fr3"             + suffix_);
    iEvent.put(v_fr4             , prefix_ + "fr4"             + suffix_);
    iEvent.put(v_trackNum        , prefix_ + "trackNum"        + suffix_);
    iEvent.put(v_hasNeighbor     , prefix_ + "hasNeighbor"     + suffix_);
    iEvent.put(v_allNeighbor     , prefix_ + "allNeighbor"     + suffix_);
    iEvent.put(v_numHits         , prefix_ + "numHits"         + suffix_);
    iEvent.put(v_firstBX         , prefix_ + "firstBX"         + suffix_);
    iEvent.put(v_secondBX        , prefix_ + "secondBX"        + suffix_);
    iEvent.put(v_ptXML           , prefix_ + "ptXML"           + suffix_);
    iEvent.put(v_thetaInt        , prefix_ + "thetaInt"        + suffix_);
    iEvent.put(v_thetaDeg        , prefix_ + "thetaDeg"        + suffix_);
    iEvent.put(v_thetaRad        , prefix_ + "thetaRad"        + suffix_);
    iEvent.put(v_type            , prefix_ + "type"            + suffix_);
    iEvent.put(v_rank            , prefix_ + "rank"            + suffix_);
    iEvent.put(v_layer           , prefix_ + "layer"           + suffix_);
    iEvent.put(v_straightness    , prefix_ + "straightness"    + suffix_);
    iEvent.put(v_strip           , prefix_ + "strip"           + suffix_);
    iEvent.put(v_isGMT           , prefix_ + "isGMT"           + suffix_);
    iEvent.put(v_numHitsExtra    , prefix_ + "numHitsExtra"    + suffix_);
    //iEvent.put(v_phis            , prefix_ + "phis"            + suffix_);
    //iEvent.put(v_thetas          , prefix_ + "thetas"          + suffix_);
    iEvent.put(v_stubRefs        , prefix_ + "stubRefs"        + suffix_);
    iEvent.put(v_size            , prefix_ + "size"            + suffix_);
}

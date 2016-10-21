#include "L1TMuonSimulations/NtupleTools/interface/NtupleEMUTFTriggerTracks.h"

namespace {
    inline float calc_phi_loc_rad(int bits) {
        return (bits * M_PI / 10800) - (22 * M_PI / 180);
    }

    inline float calc_phi_glob_rad(float loc, int sect) {
        float tmp = loc + (M_PI/12) + (sect - 1)*(M_PI/3);
        return (tmp < M_PI) ? tmp : tmp - 2*M_PI;
    }

    inline float calc_theta_rad_from_int (int _theta_int) {
        return (_theta_int * 0.2851562 + 8.5) * (M_PI / 180);
    }

    inline float calc_eta_from_theta_rad (float _theta_rad) {
        return -1 * log( tan( _theta_rad / 2 ) );
    }
}  // namespace


NtupleEMUTFTriggerTracks::NtupleEMUTFTriggerTracks(const edm::ParameterSet& iConfig) :
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

    unpStubToken_ = consumes<L1TMuonEndCap::EMTFHitCollection>(unpStubTag_);
    unpTrackToken_ = consumes<L1TMuonEndCap::EMTFTrackCollection>(unpTrackTag_);
    emuStubToken_ = consumes<L1TMuonEndCap::EMTFHitExtraCollection>(emuStubTag_);
    emuTrackToken_ = consumes<L1TMuonEndCap::EMTFTrackExtraCollection>(emuTrackTag_);

    produces<std::vector<int16_t> >           (emuPrefix_ + "iendcap"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "isector"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "bx"              + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "firstBX"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "secondBX"        + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "zone"            + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "rank"            + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "winner"          + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "mode"            + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "modeInv"         + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "phiInt"          + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "thetaInt"        + emuSuffix_);
    produces<std::vector<uint64_t> >          (emuPrefix_ + "ptAddress"       + emuSuffix_);
    produces<std::vector<int32_t> >           (emuPrefix_ + "ptGMT"           + emuSuffix_);
    produces<std::vector<int32_t> >           (emuPrefix_ + "phiGMT"          + emuSuffix_);
    produces<std::vector<int32_t> >           (emuPrefix_ + "etaGMT"          + emuSuffix_);
    produces<std::vector<int32_t> >           (emuPrefix_ + "qualityGMT"      + emuSuffix_);
    produces<std::vector<int32_t> >           (emuPrefix_ + "chargeGMT"       + emuSuffix_);
    produces<std::vector<int32_t> >           (emuPrefix_ + "chargeValidGMT"  + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "pt"              + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "xmlpt"           + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "phi"             + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "theta"           + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "eta"             + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "quality"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "charge"          + emuSuffix_);
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
    produces<std::vector<uint16_t> >          (emuPrefix_ + "clct1"           + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "clct2"           + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "clct3"           + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "clct4"           + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "fr1"             + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "fr2"             + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "fr3"             + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "fr4"             + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "straightness"    + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "layerCode"       + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "qualityCode"     + emuSuffix_);
    produces<std::vector<std::vector<unsigned> > > (emuPrefix_ + "stubRefs"        + emuSuffix_);
    produces<unsigned>                        (emuPrefix_ + "size"            + emuSuffix_);
}

NtupleEMUTFTriggerTracks::~NtupleEMUTFTriggerTracks() {}

void NtupleEMUTFTriggerTracks::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<int16_t> >           v2_iendcap         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_isector         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_bx              (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_firstBX         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_secondBX        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_zone            (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_rank            (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_winner          (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_mode            (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_modeInv         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_phiInt          (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_thetaInt        (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint64_t> >          v2_ptAddress       (new std::vector<uint64_t>());
    std::auto_ptr<std::vector<int32_t> >           v2_ptGMT           (new std::vector<int32_t>());
    std::auto_ptr<std::vector<int32_t> >           v2_phiGMT          (new std::vector<int32_t>());
    std::auto_ptr<std::vector<int32_t> >           v2_etaGMT          (new std::vector<int32_t>());
    std::auto_ptr<std::vector<int32_t> >           v2_qualityGMT      (new std::vector<int32_t>());
    std::auto_ptr<std::vector<int32_t> >           v2_chargeGMT       (new std::vector<int32_t>());
    std::auto_ptr<std::vector<int32_t> >           v2_chargeValidGMT  (new std::vector<int32_t>());
    std::auto_ptr<std::vector<float> >             v2_pt              (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v2_xmlpt           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v2_phi             (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v2_theta           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v2_eta             (new std::vector<float>());
    std::auto_ptr<std::vector<int16_t> >           v2_quality         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_charge          (new std::vector<int16_t>());
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
    std::auto_ptr<std::vector<uint16_t> >          v2_clct1           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_clct2           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_clct3           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_clct4           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_fr1             (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_fr2             (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_fr3             (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_fr4             (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_straightness    (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_layerCode       (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_qualityCode     (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<std::vector<unsigned> > > v2_stubRefs        (new std::vector<std::vector<unsigned> >());
    std::auto_ptr<unsigned>                        v2_size            (new unsigned(0));


    //__________________________________________________________________________
    edm::Handle<L1TMuonEndCap::EMTFHitExtraCollection> emuStubs;
    //iEvent.getByLabel(emuStubTag_, emuStubs);
    if (!emuStubToken_.isUninitialized())
        iEvent.getByToken(emuStubToken_, emuStubs);

    edm::Handle<L1TMuonEndCap::EMTFTrackExtraCollection> emuTracks;
    //iEvent.getByLabel(emuTrackTag_, emuTracks);
    if (!emuTrackToken_.isUninitialized())
        iEvent.getByToken(emuTrackToken_, emuTracks);

    if (emuTracks.isValid()) {
        edm::LogInfo("NtupleEMUTFTriggerTracks") << "Size: " << emuTracks->size();

        unsigned n = 0;
        for (L1TMuonEndCap::EMTFTrackExtraCollection::const_iterator it = emuTracks->begin(); it != emuTracks->end(); ++it) {
            if (n >= maxN_)
                break;
            if (!emuSelector_(*it))
                continue;

            float phi       = calc_phi_glob_rad(calc_phi_loc_rad(it->phi_int), it->sector);
            float theta     = calc_theta_rad_from_int(it->theta_int);
            float eta       = (it->endcap == 2) ? -calc_eta_from_theta_rad(theta) : calc_eta_from_theta_rad(theta);
            int16_t quality = it->gmt_quality;
            int16_t charge  = (it->gmt_charge == 1) ? -1 : 1;

            const L1TMuonEndCap::EMTFPtLUTData& ptlut_data = it->ptlut_data;

            auto get_signed_int = [](int var, int sign) {
              return (sign == 1) ? (var * 1) : (var * -1);
            };

            auto adapt_dphi = [](int x) {
              return (x == 8191) ? -999 : x;
            };

            auto adapt_dtheta = [](int x) {
              return (x == 127) ? -999 : x;
            };

            const L1TMuonEndCap::EMTFRoadExtra& road = it->xroad;

            // Get stubRefs
            std::vector<unsigned> myStubRefs;  //FIXME


            // Fill the vectors
            v2_iendcap         ->push_back(it->endcap);
            v2_isector         ->push_back(it->sector);
            v2_bx              ->push_back(it->bx);
            v2_firstBX         ->push_back(it->first_bx);
            v2_secondBX        ->push_back(it->second_bx);
            v2_zone            ->push_back(it->zone);
            v2_rank            ->push_back(it->rank);
            v2_winner          ->push_back(it->winner);
            v2_mode            ->push_back(it->mode);
            v2_modeInv         ->push_back(it->mode_inv);
            v2_phiInt          ->push_back(it->phi_int);
            v2_thetaInt        ->push_back(it->theta_int);
            v2_ptAddress       ->push_back(it->ptlut_address);
            v2_ptGMT           ->push_back(it->gmt_pt);
            v2_phiGMT          ->push_back(it->gmt_phi);
            v2_etaGMT          ->push_back(it->gmt_eta);
            v2_qualityGMT      ->push_back(it->gmt_quality);
            v2_chargeGMT       ->push_back(it->gmt_charge);
            v2_chargeValidGMT  ->push_back(it->gmt_charge_valid);
            v2_pt              ->push_back(it->pt);
            v2_xmlpt           ->push_back(it->xmlpt);
            v2_phi             ->push_back(phi);
            v2_theta           ->push_back(theta);
            v2_eta             ->push_back(eta);
            v2_quality         ->push_back(quality);
            v2_charge          ->push_back(charge);
            v2_dPhi12          ->push_back(adapt_dphi(get_signed_int(ptlut_data.delta_ph[0], ptlut_data.sign_ph[0]) ) );
            v2_dPhi13          ->push_back(adapt_dphi(get_signed_int(ptlut_data.delta_ph[1], ptlut_data.sign_ph[1]) ) );
            v2_dPhi14          ->push_back(adapt_dphi(get_signed_int(ptlut_data.delta_ph[2], ptlut_data.sign_ph[2]) ) );
            v2_dPhi23          ->push_back(adapt_dphi(get_signed_int(ptlut_data.delta_ph[3], ptlut_data.sign_ph[3]) ) );
            v2_dPhi24          ->push_back(adapt_dphi(get_signed_int(ptlut_data.delta_ph[4], ptlut_data.sign_ph[4]) ) );
            v2_dPhi34          ->push_back(adapt_dphi(get_signed_int(ptlut_data.delta_ph[5], ptlut_data.sign_ph[5]) ) );
            v2_dTheta12        ->push_back(adapt_dtheta(get_signed_int(ptlut_data.delta_th[0], 1-ptlut_data.sign_th[0]) ) );
            v2_dTheta13        ->push_back(adapt_dtheta(get_signed_int(ptlut_data.delta_th[1], 1-ptlut_data.sign_th[1]) ) );
            v2_dTheta14        ->push_back(adapt_dtheta(get_signed_int(ptlut_data.delta_th[2], 1-ptlut_data.sign_th[2]) ) );
            v2_dTheta23        ->push_back(adapt_dtheta(get_signed_int(ptlut_data.delta_th[3], 1-ptlut_data.sign_th[3]) ) );
            v2_dTheta24        ->push_back(adapt_dtheta(get_signed_int(ptlut_data.delta_th[4], 1-ptlut_data.sign_th[4]) ) );
            v2_dTheta34        ->push_back(adapt_dtheta(get_signed_int(ptlut_data.delta_th[5], 1-ptlut_data.sign_th[5]) ) );
            v2_clct1           ->push_back(ptlut_data.cpattern[0]  );
            v2_clct2           ->push_back(ptlut_data.cpattern[1]  );
            v2_clct3           ->push_back(ptlut_data.cpattern[2]  );
            v2_clct4           ->push_back(ptlut_data.cpattern[3]  );
            v2_fr1             ->push_back(ptlut_data.fr[0]        );
            v2_fr2             ->push_back(ptlut_data.fr[1]        );
            v2_fr3             ->push_back(ptlut_data.fr[2]        );
            v2_fr4             ->push_back(ptlut_data.fr[3]        );
            v2_straightness    ->push_back(road.straightness);
            v2_layerCode       ->push_back(road.layer_code);
            v2_qualityCode     ->push_back(road.quality_code);
            v2_stubRefs        ->push_back(myStubRefs);
        }
        *v2_size = v2_iendcap->size();

    } else {
        edm::LogError("NtupleEMUTFTriggerTracks") << "Cannot get the product: " << emuTrackTag_;
    }


    //__________________________________________________________________________
    iEvent.put(v2_iendcap         , emuPrefix_ + "iendcap"          + emuSuffix_);
    iEvent.put(v2_isector         , emuPrefix_ + "isector"          + emuSuffix_);
    iEvent.put(v2_bx              , emuPrefix_ + "bx"               + emuSuffix_);
    iEvent.put(v2_firstBX         , emuPrefix_ + "firstBX"          + emuSuffix_);
    iEvent.put(v2_secondBX        , emuPrefix_ + "secondBX"         + emuSuffix_);
    iEvent.put(v2_zone            , emuPrefix_ + "zone"             + emuSuffix_);
    iEvent.put(v2_rank            , emuPrefix_ + "rank"             + emuSuffix_);
    iEvent.put(v2_winner          , emuPrefix_ + "winner"           + emuSuffix_);
    iEvent.put(v2_mode            , emuPrefix_ + "mode"             + emuSuffix_);
    iEvent.put(v2_modeInv         , emuPrefix_ + "modeInv"          + emuSuffix_);
    iEvent.put(v2_phiInt          , emuPrefix_ + "phiInt"           + emuSuffix_);
    iEvent.put(v2_thetaInt        , emuPrefix_ + "thetaInt"         + emuSuffix_);
    iEvent.put(v2_ptAddress       , emuPrefix_ + "ptAddress"        + emuSuffix_);
    iEvent.put(v2_ptGMT           , emuPrefix_ + "ptGMT"            + emuSuffix_);
    iEvent.put(v2_phiGMT          , emuPrefix_ + "phiGMT"           + emuSuffix_);
    iEvent.put(v2_etaGMT          , emuPrefix_ + "etaGMT"           + emuSuffix_);
    iEvent.put(v2_qualityGMT      , emuPrefix_ + "qualityGMT"       + emuSuffix_);
    iEvent.put(v2_chargeGMT       , emuPrefix_ + "chargeGMT"        + emuSuffix_);
    iEvent.put(v2_chargeValidGMT  , emuPrefix_ + "chargeValidGMT"   + emuSuffix_);
    iEvent.put(v2_pt              , emuPrefix_ + "pt"               + emuSuffix_);
    iEvent.put(v2_xmlpt           , emuPrefix_ + "xmlpt"            + emuSuffix_);
    iEvent.put(v2_phi             , emuPrefix_ + "phi"              + emuSuffix_);
    iEvent.put(v2_theta           , emuPrefix_ + "theta"            + emuSuffix_);
    iEvent.put(v2_eta             , emuPrefix_ + "eta"              + emuSuffix_);
    iEvent.put(v2_quality         , emuPrefix_ + "quality"          + emuSuffix_);
    iEvent.put(v2_charge          , emuPrefix_ + "charge"           + emuSuffix_);
    iEvent.put(v2_dPhi12          , emuPrefix_ + "dPhi12"           + emuSuffix_);
    iEvent.put(v2_dPhi13          , emuPrefix_ + "dPhi13"           + emuSuffix_);
    iEvent.put(v2_dPhi14          , emuPrefix_ + "dPhi14"           + emuSuffix_);
    iEvent.put(v2_dPhi23          , emuPrefix_ + "dPhi23"           + emuSuffix_);
    iEvent.put(v2_dPhi24          , emuPrefix_ + "dPhi24"           + emuSuffix_);
    iEvent.put(v2_dPhi34          , emuPrefix_ + "dPhi34"           + emuSuffix_);
    iEvent.put(v2_dTheta12        , emuPrefix_ + "dTheta12"         + emuSuffix_);
    iEvent.put(v2_dTheta13        , emuPrefix_ + "dTheta13"         + emuSuffix_);
    iEvent.put(v2_dTheta14        , emuPrefix_ + "dTheta14"         + emuSuffix_);
    iEvent.put(v2_dTheta23        , emuPrefix_ + "dTheta23"         + emuSuffix_);
    iEvent.put(v2_dTheta24        , emuPrefix_ + "dTheta24"         + emuSuffix_);
    iEvent.put(v2_dTheta34        , emuPrefix_ + "dTheta34"         + emuSuffix_);
    iEvent.put(v2_clct1           , emuPrefix_ + "clct1"            + emuSuffix_);
    iEvent.put(v2_clct2           , emuPrefix_ + "clct2"            + emuSuffix_);
    iEvent.put(v2_clct3           , emuPrefix_ + "clct3"            + emuSuffix_);
    iEvent.put(v2_clct4           , emuPrefix_ + "clct4"            + emuSuffix_);
    iEvent.put(v2_fr1             , emuPrefix_ + "fr1"              + emuSuffix_);
    iEvent.put(v2_fr2             , emuPrefix_ + "fr2"              + emuSuffix_);
    iEvent.put(v2_fr3             , emuPrefix_ + "fr3"              + emuSuffix_);
    iEvent.put(v2_fr4             , emuPrefix_ + "fr4"              + emuSuffix_);
    iEvent.put(v2_straightness    , emuPrefix_ + "straightness"     + emuSuffix_);
    iEvent.put(v2_layerCode       , emuPrefix_ + "layerCode"        + emuSuffix_);
    iEvent.put(v2_qualityCode     , emuPrefix_ + "qualityCode"      + emuSuffix_);
    iEvent.put(v2_stubRefs        , emuPrefix_ + "stubRefs"         + emuSuffix_);
    iEvent.put(v2_size            , emuPrefix_ + "size"             + emuSuffix_);
}

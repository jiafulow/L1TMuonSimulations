#include "L1TMuonSimulations/NtupleTools/interface/NtupleEMTFTriggerPrimitives.h"

#include "L1TMuonSimulations/MuonTools/interface/ModuleIdHelper.h"


NtupleEMTFTriggerPrimitives::NtupleEMTFTriggerPrimitives(const edm::ParameterSet& iConfig) :
  stubTag_ (iConfig.getParameter<edm::InputTag>("stubTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    stubToken_ = consumes<l1t::EMTFHitExtraCollection>(stubTag_);

    produces<std::vector<uint32_t> >          (prefix_ + "geoId"           + suffix_);
    produces<std::vector<uint32_t> >          (prefix_ + "moduleId"        + suffix_);
    produces<std::vector<bool> >              (prefix_ + "evenBit"         + suffix_);
    produces<std::vector<bool> >              (prefix_ + "frBit"           + suffix_);
    produces<std::vector<bool> >              (prefix_ + "ccwBit"          + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "iendcap"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "istation"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "iring"           + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "isector"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "isectorIndex"    + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "isubsector"      + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "ichamber"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "cscID"           + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "neighbor"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "mpcLink"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "wire"            + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "strip"           + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "trackNum"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "quality"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "pattern"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "bend"            + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "valid"           + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "syncErr"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "bc0"             + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "bx"              + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "stubNum"         + suffix_);
    produces<std::vector<bool> >              (prefix_ + "isCSCHit"        + suffix_);
    produces<std::vector<bool> >              (prefix_ + "isRPCHit"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "bx0"             + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "layer"           + suffix_);
    produces<std::vector<int> >               (prefix_ + "zoneHit"         + suffix_);
    produces<std::vector<int> >               (prefix_ + "phiHit"          + suffix_);
    produces<std::vector<int> >               (prefix_ + "phiZVal"         + suffix_);
    produces<std::vector<int> >               (prefix_ + "phiLocInt"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "phiLocDeg"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "phiLocRad"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "phiGlbDeg"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "phiGlbRad"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "phiGeomRad"      + suffix_);
    produces<std::vector<int> >               (prefix_ + "thetaInt"        + suffix_);
    produces<std::vector<float> >             (prefix_ + "thetaLoc"        + suffix_);
    produces<std::vector<float> >             (prefix_ + "thetaDeg"        + suffix_);
    produces<std::vector<float> >             (prefix_ + "thetaRad"        + suffix_);
    produces<std::vector<float> >             (prefix_ + "eta"             + suffix_);
    produces<std::vector<std::vector<int> > > (prefix_ + "zones"           + suffix_);
    produces<unsigned>                        (prefix_ + "size"            + suffix_);
}

NtupleEMTFTriggerPrimitives::~NtupleEMTFTriggerPrimitives() {}

void NtupleEMTFTriggerPrimitives::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<uint32_t> >          v_geoId           (new std::vector<uint32_t>());
    std::auto_ptr<std::vector<uint32_t> >          v_moduleId        (new std::vector<uint32_t>());
    std::auto_ptr<std::vector<bool> >              v_evenBit         (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >              v_frBit           (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >              v_ccwBit          (new std::vector<bool>());
    std::auto_ptr<std::vector<int16_t> >           v_iendcap         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_istation        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_iring           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_isector         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_isectorIndex    (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_isubsector      (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_ichamber        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_cscID           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_neighbor        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_mpcLink         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_wire            (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_strip           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_trackNum        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_quality         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_pattern         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_bend            (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_valid           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_syncErr         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_bc0             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_bx              (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_stubNum         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<bool> >              v_isCSCHit        (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >              v_isRPCHit        (new std::vector<bool>());
    std::auto_ptr<std::vector<int16_t> >           v_bx0             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_layer           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int> >               v_zoneHit         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_phiHit          (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_phiZVal         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_phiLocInt       (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v_phiLocDeg       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_phiLocRad       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_phiGlbDeg       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_phiGlbRad       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_phiGeomRad      (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v_thetaInt        (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v_thetaLoc        (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_thetaDeg        (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_thetaRad        (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_eta             (new std::vector<float>());
    std::auto_ptr<std::vector<std::vector<int> > > v_zones           (new std::vector<std::vector<int> >());
    std::auto_ptr<unsigned>                        v_size            (new unsigned(0));


    //__________________________________________________________________________
    edm::Handle<l1t::EMTFHitExtraCollection> stubs;
    //iEvent.getByLabel(stubTag_, stubs);
    if (!stubToken_.isUninitialized())
        iEvent.getByToken(stubToken_, stubs);

    if (stubs.isValid()) {
        edm::LogInfo("NtupleEMTFTriggerPrimitives") << "Size: " << stubs->size();

        unsigned n = 0;
        for (l1t::EMTFHitExtraCollection::const_iterator it = stubs->begin(); it != stubs->end(); ++it) {
            if (n >= maxN_)
                break;
            if (!selector_(*it))
                continue;

            //if (it->Neighbor() != 0)  continue;

            const CSCDetId cscDet = it->CSC_DetId();
            const uint32_t moduleId = ModuleIdHelper::getModuleId(cscDet);
            const bool evenBit      = ModuleIdHelper::isEven(cscDet);
            const bool frBit        = ModuleIdHelper::isFront(cscDet);
            const bool ccwBit       = ModuleIdHelper::isCounterClockwise(cscDet);

            // Fill the vectors
            v_geoId           ->push_back(cscDet.rawId());
            v_moduleId        ->push_back(moduleId);
            v_evenBit         ->push_back(evenBit);
            v_frBit           ->push_back(frBit);
            v_ccwBit          ->push_back(ccwBit);
            v_iendcap         ->push_back(it->Endcap());
            v_istation        ->push_back(it->Station());
            v_iring           ->push_back(it->Ring());
            v_isector         ->push_back(it->Sector());
            v_isectorIndex    ->push_back(it->Sector_index());
            v_isubsector      ->push_back(it->Subsector());
            v_ichamber        ->push_back(it->Chamber());
            v_cscID           ->push_back(it->CSC_ID());
            v_neighbor        ->push_back(it->Neighbor());
            v_mpcLink         ->push_back(it->MPC_link());
            v_wire            ->push_back(it->Wire());
            v_strip           ->push_back(it->Strip());
            v_trackNum        ->push_back(it->Track_num());
            v_quality         ->push_back(it->Quality());
            v_pattern         ->push_back(it->Pattern());
            v_bend            ->push_back(it->Bend());
            v_valid           ->push_back(it->Valid());
            v_syncErr         ->push_back(it->Sync_err());
            v_bc0             ->push_back(it->BC0());
            v_bx              ->push_back(it->BX());
            v_stubNum         ->push_back(it->Stub_num());
            v_isCSCHit        ->push_back(it->Is_CSC_hit());
            v_isRPCHit        ->push_back(it->Is_RPC_hit());
            v_bx0             ->push_back(it->BX0());
            v_layer           ->push_back(it->Layer());
            v_zoneHit         ->push_back(0);  //FIXME: v_zoneHit         ->push_back(it->Zone_hit());
            v_phiHit          ->push_back(it->Phi_hit());
            v_phiZVal         ->push_back(0);  //FIXME: v_phiZVal         ->push_back(it->Phi_Z_val());
            v_phiLocInt       ->push_back(it->Phi_loc_int());
            v_phiLocDeg       ->push_back(it->Phi_loc_deg());
            v_phiLocRad       ->push_back(it->Phi_loc_rad());
            v_phiGlbDeg       ->push_back(it->Phi_glob_deg());
            v_phiGlbRad       ->push_back(it->Phi_glob_rad());
            v_phiGeomRad      ->push_back(it->Phi_geom_rad());
            v_thetaInt        ->push_back(it->Theta_int());
            v_thetaLoc        ->push_back(it->Theta_loc());
            v_thetaDeg        ->push_back(it->Theta_deg());
            v_thetaRad        ->push_back(it->Theta_rad());
            v_eta             ->push_back(it->Eta());
            v_zones           ->push_back(it->Zone_contribution());
        }

        *v_size = v_geoId->size();

    } else {
        edm::LogError("NtupleEMTFTriggerPrimitives") << "Cannot get the product: " << stubTag_;
    }


    //__________________________________________________________________________
    iEvent.put(v_geoId           , prefix_ + "geoId"           + suffix_);
    iEvent.put(v_moduleId        , prefix_ + "moduleId"        + suffix_);
    iEvent.put(v_evenBit         , prefix_ + "evenBit"         + suffix_);
    iEvent.put(v_frBit           , prefix_ + "frBit"           + suffix_);
    iEvent.put(v_ccwBit          , prefix_ + "ccwBit"          + suffix_);
    iEvent.put(v_iendcap         , prefix_ + "iendcap"         + suffix_);
    iEvent.put(v_istation        , prefix_ + "istation"        + suffix_);
    iEvent.put(v_iring           , prefix_ + "iring"           + suffix_);
    iEvent.put(v_isector         , prefix_ + "isector"         + suffix_);
    iEvent.put(v_isectorIndex    , prefix_ + "isectorIndex"    + suffix_);
    iEvent.put(v_isubsector      , prefix_ + "isubsector"      + suffix_);
    iEvent.put(v_ichamber        , prefix_ + "ichamber"        + suffix_);
    iEvent.put(v_cscID           , prefix_ + "cscID"           + suffix_);
    iEvent.put(v_neighbor        , prefix_ + "neighbor"        + suffix_);
    iEvent.put(v_mpcLink         , prefix_ + "mpcLink"         + suffix_);
    iEvent.put(v_wire            , prefix_ + "wire"            + suffix_);
    iEvent.put(v_strip           , prefix_ + "strip"           + suffix_);
    iEvent.put(v_trackNum        , prefix_ + "trackNum"        + suffix_);
    iEvent.put(v_quality         , prefix_ + "quality"         + suffix_);
    iEvent.put(v_pattern         , prefix_ + "pattern"         + suffix_);
    iEvent.put(v_bend            , prefix_ + "bend"            + suffix_);
    iEvent.put(v_valid           , prefix_ + "valid"           + suffix_);
    iEvent.put(v_syncErr         , prefix_ + "syncErr"         + suffix_);
    iEvent.put(v_bc0             , prefix_ + "bc0"             + suffix_);
    iEvent.put(v_bx              , prefix_ + "bx"              + suffix_);
    iEvent.put(v_stubNum         , prefix_ + "stubNum"         + suffix_);
    iEvent.put(v_isCSCHit        , prefix_ + "isCSCHit"        + suffix_);
    iEvent.put(v_isRPCHit        , prefix_ + "isRPCHit"        + suffix_);
    iEvent.put(v_bx0             , prefix_ + "bx0"             + suffix_);
    iEvent.put(v_layer           , prefix_ + "layer"           + suffix_);
    iEvent.put(v_zoneHit         , prefix_ + "zoneHit"         + suffix_);
    iEvent.put(v_phiHit          , prefix_ + "phiHit"          + suffix_);
    iEvent.put(v_phiZVal         , prefix_ + "phiZVal"         + suffix_);
    iEvent.put(v_phiLocInt       , prefix_ + "phiLocInt"       + suffix_);
    iEvent.put(v_phiLocDeg       , prefix_ + "phiLocDeg"       + suffix_);
    iEvent.put(v_phiLocRad       , prefix_ + "phiLocRad"       + suffix_);
    iEvent.put(v_phiGlbDeg       , prefix_ + "phiGlbDeg"       + suffix_);
    iEvent.put(v_phiGlbRad       , prefix_ + "phiGlbRad"       + suffix_);
    iEvent.put(v_phiGeomRad      , prefix_ + "phiGeomRad"      + suffix_);
    iEvent.put(v_thetaInt        , prefix_ + "thetaInt"        + suffix_);
    iEvent.put(v_thetaLoc        , prefix_ + "thetaLoc"        + suffix_);
    iEvent.put(v_thetaDeg        , prefix_ + "thetaDeg"        + suffix_);
    iEvent.put(v_thetaRad        , prefix_ + "thetaRad"        + suffix_);
    iEvent.put(v_eta             , prefix_ + "eta"             + suffix_);
    iEvent.put(v_zones           , prefix_ + "zones"           + suffix_);
    iEvent.put(v_size            , prefix_ + "size"            + suffix_);
}

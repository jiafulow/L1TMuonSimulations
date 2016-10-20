#include "L1TMuonSimulations/NtupleTools/interface/NtupleEMTFTriggerPrimitives.h"

#include "L1TMuonSimulations/MuonTools/interface/ModuleIdHelper.h"


NtupleEMTFTriggerPrimitives::NtupleEMTFTriggerPrimitives(const edm::ParameterSet& iConfig) :
  unpStubTag_ (iConfig.getParameter<edm::InputTag>("unpStubTag")),
  unpPrefix_  (iConfig.getParameter<std::string>("unpPrefix")),
  unpSuffix_  (iConfig.getParameter<std::string>("unpSuffix")),
  unpSelector_(iConfig.existsAs<std::string>("unpCut") ? iConfig.getParameter<std::string>("unpCut") : "", true),
  emuStubTag_ (iConfig.getParameter<edm::InputTag>("emuStubTag")),
  emuPrefix_  (iConfig.getParameter<std::string>("emuPrefix")),
  emuSuffix_  (iConfig.getParameter<std::string>("emuSuffix")),
  emuSelector_(iConfig.existsAs<std::string>("emuCut") ? iConfig.getParameter<std::string>("emuCut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    unpStubToken_ = consumes<l1t::EMTFHitCollection>(unpStubTag_);
    emuStubToken_ = consumes<l1t::EMTFHitExtraCollection>(emuStubTag_);

    produces<std::vector<uint32_t> >          (unpPrefix_ + "geoId"           + unpSuffix_);
    produces<std::vector<uint32_t> >          (unpPrefix_ + "moduleId"        + unpSuffix_);
    produces<std::vector<bool> >              (unpPrefix_ + "evenBit"         + unpSuffix_);
    produces<std::vector<bool> >              (unpPrefix_ + "frBit"           + unpSuffix_);
    produces<std::vector<bool> >              (unpPrefix_ + "ccwBit"          + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "iendcap"         + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "istation"        + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "iring"           + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "isector"         + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "isectorIndex"    + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "isubsector"      + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "ichamber"        + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "cscID"           + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "neighbor"        + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "mpcLink"         + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "wire"            + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "strip"           + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "trackNum"        + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "quality"         + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "pattern"         + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "bend"            + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "valid"           + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "syncErr"         + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "bc0"             + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "bx"              + unpSuffix_);
    produces<std::vector<int16_t> >           (unpPrefix_ + "stubNum"         + unpSuffix_);
    produces<unsigned>                        (unpPrefix_ + "size"            + unpSuffix_);

    produces<std::vector<uint32_t> >          (emuPrefix_ + "geoId"           + emuSuffix_);
    produces<std::vector<uint32_t> >          (emuPrefix_ + "moduleId"        + emuSuffix_);
    produces<std::vector<bool> >              (emuPrefix_ + "evenBit"         + emuSuffix_);
    produces<std::vector<bool> >              (emuPrefix_ + "frBit"           + emuSuffix_);
    produces<std::vector<bool> >              (emuPrefix_ + "ccwBit"          + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "iendcap"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "istation"        + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "iring"           + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "isector"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "isectorIndex"    + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "isubsector"      + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "ichamber"        + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "cscID"           + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "neighbor"        + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "mpcLink"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "wire"            + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "strip"           + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "trackNum"        + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "quality"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "pattern"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "bend"            + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "valid"           + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "syncErr"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "bc0"             + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "bx"              + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "stubNum"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "bx0"             + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "layer"           + emuSuffix_);
    produces<std::vector<int> >               (emuPrefix_ + "zone"            + emuSuffix_);
    produces<std::vector<int> >               (emuPrefix_ + "phiHit"          + emuSuffix_);
    produces<std::vector<int> >               (emuPrefix_ + "phiZone"         + emuSuffix_);
    produces<std::vector<int> >               (emuPrefix_ + "phiLocInt"       + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "phiLocDeg"       + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "phiLocRad"       + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "phiGlbDeg"       + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "phiGlbRad"       + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "phiGeomRad"      + emuSuffix_);
    produces<std::vector<int> >               (emuPrefix_ + "thetaInt"        + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "thetaLoc"        + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "thetaDeg"        + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "thetaRad"        + emuSuffix_);
    produces<std::vector<float> >             (emuPrefix_ + "eta"             + emuSuffix_);
    produces<unsigned>                        (emuPrefix_ + "size"            + emuSuffix_);
}

NtupleEMTFTriggerPrimitives::~NtupleEMTFTriggerPrimitives() {}

void NtupleEMTFTriggerPrimitives::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<uint32_t> >          v1_geoId           (new std::vector<uint32_t>());
    std::auto_ptr<std::vector<uint32_t> >          v1_moduleId        (new std::vector<uint32_t>());
    std::auto_ptr<std::vector<bool> >              v1_evenBit         (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >              v1_frBit           (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >              v1_ccwBit          (new std::vector<bool>());
    std::auto_ptr<std::vector<int16_t> >           v1_iendcap         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_istation        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_iring           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_isector         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_isectorIndex    (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_isubsector      (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_ichamber        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_cscID           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_neighbor        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_mpcLink         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_wire            (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_strip           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_trackNum        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_quality         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_pattern         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_bend            (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_valid           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_syncErr         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_bc0             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_bx              (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v1_stubNum         (new std::vector<int16_t>());
    std::auto_ptr<unsigned>                        v1_size            (new unsigned(0));

    std::auto_ptr<std::vector<uint32_t> >          v2_geoId           (new std::vector<uint32_t>());
    std::auto_ptr<std::vector<uint32_t> >          v2_moduleId        (new std::vector<uint32_t>());
    std::auto_ptr<std::vector<bool> >              v2_evenBit         (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >              v2_frBit           (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >              v2_ccwBit          (new std::vector<bool>());
    std::auto_ptr<std::vector<int16_t> >           v2_iendcap         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_istation        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_iring           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_isector         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_isectorIndex    (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_isubsector      (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_ichamber        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_cscID           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_neighbor        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_mpcLink         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_wire            (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_strip           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_trackNum        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_quality         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_pattern         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_bend            (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_valid           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_syncErr         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_bc0             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_bx              (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_stubNum         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_bx0             (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_layer           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int> >               v2_zone            (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v2_phiHit          (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v2_phiZone         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v2_phiLocInt       (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v2_phiLocDeg       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v2_phiLocRad       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v2_phiGlbDeg       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v2_phiGlbRad       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v2_phiGeomRad      (new std::vector<float>());
    std::auto_ptr<std::vector<int> >               v2_thetaInt        (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v2_thetaLoc        (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v2_thetaDeg        (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v2_thetaRad        (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v2_eta             (new std::vector<float>());
    std::auto_ptr<unsigned>                        v2_size            (new unsigned(0));

    edm::Handle<l1t::EMTFHitCollection> unpStubs;
    //iEvent.getByLabel(unpStubTag_, unpStubs);
    if (!unpStubToken_.isUninitialized())
        iEvent.getByToken(unpStubToken_, unpStubs);

    if (unpStubs.isValid()) {
        edm::LogInfo("NtupleUnpackedEMTFTriggerPrimitives") << "Unpacker size: " << unpStubs->size();

        unsigned n = 0;
        for (l1t::EMTFHitCollection::const_iterator it = unpStubs->begin(); it != unpStubs->end(); ++it) {
            if (n >= maxN_)
                break;
            if (!unpSelector_(*it))
                continue;

            //if (it->Neighbor() != 0)  continue;
            if (!it->Is_CSC_hit())  continue;

            const CSCDetId cscDet = it->CSC_DetId();
            const uint32_t moduleId = ModuleIdHelper::getModuleId(cscDet);
            const bool evenBit      = ModuleIdHelper::isEven(cscDet);
            const bool frBit        = ModuleIdHelper::isFront(cscDet);
            const bool ccwBit       = ModuleIdHelper::isCounterClockwise(cscDet);

            // Fill the vectors
            v1_geoId           ->push_back(cscDet.rawId());
            v1_moduleId        ->push_back(moduleId);
            v1_evenBit         ->push_back(evenBit);
            v1_frBit           ->push_back(frBit);
            v1_ccwBit          ->push_back(ccwBit);
            v1_iendcap         ->push_back(it->Endcap());
            v1_istation        ->push_back(it->Station());
            v1_iring           ->push_back(it->Ring());
            v1_isector         ->push_back(it->Sector());
            v1_isectorIndex    ->push_back(it->Sector_index());
            v1_isubsector      ->push_back(it->Subsector());
            v1_ichamber        ->push_back(it->Chamber());
            v1_cscID           ->push_back(it->CSC_ID());
            v1_neighbor        ->push_back(it->Neighbor());
            v1_mpcLink         ->push_back(it->MPC_link());
            v1_wire            ->push_back(it->Wire());
            v1_strip           ->push_back(it->Strip());
            v1_trackNum        ->push_back(it->Track_num());
            v1_quality         ->push_back(it->Quality());
            v1_pattern         ->push_back(it->Pattern());
            v1_bend            ->push_back(it->Bend());
            v1_valid           ->push_back(it->Valid());
            v1_syncErr         ->push_back(it->Sync_err());
            v1_bc0             ->push_back(it->BC0());
            v1_bx              ->push_back(it->BX());
            v1_stubNum         ->push_back(it->Stub_num());
        }

        *v1_size = v1_geoId->size();

    } else {
        edm::LogError("NtupleUnpackedEMTFTriggerPrimitives") << "Cannot get the product: " << unpStubTag_;
    }

    //__________________________________________________________________________
    edm::Handle<l1t::EMTFHitExtraCollection> emuStubs;
    //iEvent.getByLabel(emuStubTag_, emuStubs);
    if (!emuStubToken_.isUninitialized())
        iEvent.getByToken(emuStubToken_, emuStubs);

    if (emuStubs.isValid()) {
        edm::LogInfo("NtupleEMTFTriggerPrimitives") << "Emulator size: " << emuStubs->size();

        unsigned n = 0;
        for (l1t::EMTFHitExtraCollection::const_iterator it = emuStubs->begin(); it != emuStubs->end(); ++it) {
            if (n >= maxN_)
                break;
            if (!emuSelector_(*it))
                continue;

            //if (it->Neighbor() != 0)  continue;
            if (!it->Is_CSC_hit())  continue;

            const CSCDetId cscDet = it->CSC_DetId();
            const uint32_t moduleId = ModuleIdHelper::getModuleId(cscDet);
            const bool evenBit      = ModuleIdHelper::isEven(cscDet);
            const bool frBit        = ModuleIdHelper::isFront(cscDet);
            const bool ccwBit       = ModuleIdHelper::isCounterClockwise(cscDet);

            // Fill the vectors
            v2_geoId           ->push_back(cscDet.rawId());
            v2_moduleId        ->push_back(moduleId);
            v2_evenBit         ->push_back(evenBit);
            v2_frBit           ->push_back(frBit);
            v2_ccwBit          ->push_back(ccwBit);
            v2_iendcap         ->push_back(it->Endcap());
            v2_istation        ->push_back(it->Station());
            v2_iring           ->push_back(it->Ring());
            v2_isector         ->push_back(it->Sector());
            v2_isectorIndex    ->push_back(it->Sector_index());
            v2_isubsector      ->push_back(it->Subsector());
            v2_ichamber        ->push_back(it->Chamber());
            v2_cscID           ->push_back(it->CSC_ID());
            v2_neighbor        ->push_back(it->Neighbor());
            v2_mpcLink         ->push_back(it->MPC_link());
            v2_wire            ->push_back(it->Wire());
            v2_strip           ->push_back(it->Strip());
            v2_trackNum        ->push_back(it->Track_num());
            v2_quality         ->push_back(it->Quality());
            v2_pattern         ->push_back(it->Pattern());
            v2_bend            ->push_back(it->Bend());
            v2_valid           ->push_back(it->Valid());
            v2_syncErr         ->push_back(it->Sync_err());
            v2_bc0             ->push_back(it->BC0());
            v2_bx              ->push_back(it->BX());
            v2_stubNum         ->push_back(it->Stub_num());
            v2_bx0             ->push_back(it->BX0());
            v2_layer           ->push_back(it->Layer());
            v2_zone            ->push_back(it->Zone());
            v2_phiHit          ->push_back(it->Phi_hit());
            v2_phiZone         ->push_back(it->Phi_zone());
            v2_phiLocInt       ->push_back(it->Phi_loc_int());
            v2_phiLocDeg       ->push_back(it->Phi_loc_deg());
            v2_phiLocRad       ->push_back(it->Phi_loc_rad());
            v2_phiGlbDeg       ->push_back(it->Phi_glob_deg());
            v2_phiGlbRad       ->push_back(it->Phi_glob_rad());
            v2_phiGeomRad      ->push_back(it->Phi_geom_rad());
            v2_thetaInt        ->push_back(it->Theta_int());
            v2_thetaLoc        ->push_back(it->Theta_loc());
            v2_thetaDeg        ->push_back(it->Theta_deg());
            v2_thetaRad        ->push_back(it->Theta_rad());
            v2_eta             ->push_back(it->Eta());
        }

        *v2_size = v2_geoId->size();

    } else {
        edm::LogError("NtupleEMTFTriggerPrimitives") << "Cannot get the product: " << emuStubTag_;
    }


    //__________________________________________________________________________
    iEvent.put(v1_geoId           , unpPrefix_ + "geoId"           + unpSuffix_);
    iEvent.put(v1_moduleId        , unpPrefix_ + "moduleId"        + unpSuffix_);
    iEvent.put(v1_evenBit         , unpPrefix_ + "evenBit"         + unpSuffix_);
    iEvent.put(v1_frBit           , unpPrefix_ + "frBit"           + unpSuffix_);
    iEvent.put(v1_ccwBit          , unpPrefix_ + "ccwBit"          + unpSuffix_);
    iEvent.put(v1_iendcap         , unpPrefix_ + "iendcap"         + unpSuffix_);
    iEvent.put(v1_istation        , unpPrefix_ + "istation"        + unpSuffix_);
    iEvent.put(v1_iring           , unpPrefix_ + "iring"           + unpSuffix_);
    iEvent.put(v1_isector         , unpPrefix_ + "isector"         + unpSuffix_);
    iEvent.put(v1_isectorIndex    , unpPrefix_ + "isectorIndex"    + unpSuffix_);
    iEvent.put(v1_isubsector      , unpPrefix_ + "isubsector"      + unpSuffix_);
    iEvent.put(v1_ichamber        , unpPrefix_ + "ichamber"        + unpSuffix_);
    iEvent.put(v1_cscID           , unpPrefix_ + "cscID"           + unpSuffix_);
    iEvent.put(v1_neighbor        , unpPrefix_ + "neighbor"        + unpSuffix_);
    iEvent.put(v1_mpcLink         , unpPrefix_ + "mpcLink"         + unpSuffix_);
    iEvent.put(v1_wire            , unpPrefix_ + "wire"            + unpSuffix_);
    iEvent.put(v1_strip           , unpPrefix_ + "strip"           + unpSuffix_);
    iEvent.put(v1_trackNum        , unpPrefix_ + "trackNum"        + unpSuffix_);
    iEvent.put(v1_quality         , unpPrefix_ + "quality"         + unpSuffix_);
    iEvent.put(v1_pattern         , unpPrefix_ + "pattern"         + unpSuffix_);
    iEvent.put(v1_bend            , unpPrefix_ + "bend"            + unpSuffix_);
    iEvent.put(v1_valid           , unpPrefix_ + "valid"           + unpSuffix_);
    iEvent.put(v1_syncErr         , unpPrefix_ + "syncErr"         + unpSuffix_);
    iEvent.put(v1_bc0             , unpPrefix_ + "bc0"             + unpSuffix_);
    iEvent.put(v1_bx              , unpPrefix_ + "bx"              + unpSuffix_);
    iEvent.put(v1_stubNum         , unpPrefix_ + "stubNum"         + unpSuffix_);
    iEvent.put(v1_size            , unpPrefix_ + "size"            + unpSuffix_);

    iEvent.put(v2_geoId           , emuPrefix_ + "geoId"           + emuSuffix_);
    iEvent.put(v2_moduleId        , emuPrefix_ + "moduleId"        + emuSuffix_);
    iEvent.put(v2_evenBit         , emuPrefix_ + "evenBit"         + emuSuffix_);
    iEvent.put(v2_frBit           , emuPrefix_ + "frBit"           + emuSuffix_);
    iEvent.put(v2_ccwBit          , emuPrefix_ + "ccwBit"          + emuSuffix_);
    iEvent.put(v2_iendcap         , emuPrefix_ + "iendcap"         + emuSuffix_);
    iEvent.put(v2_istation        , emuPrefix_ + "istation"        + emuSuffix_);
    iEvent.put(v2_iring           , emuPrefix_ + "iring"           + emuSuffix_);
    iEvent.put(v2_isector         , emuPrefix_ + "isector"         + emuSuffix_);
    iEvent.put(v2_isectorIndex    , emuPrefix_ + "isectorIndex"    + emuSuffix_);
    iEvent.put(v2_isubsector      , emuPrefix_ + "isubsector"      + emuSuffix_);
    iEvent.put(v2_ichamber        , emuPrefix_ + "ichamber"        + emuSuffix_);
    iEvent.put(v2_cscID           , emuPrefix_ + "cscID"           + emuSuffix_);
    iEvent.put(v2_neighbor        , emuPrefix_ + "neighbor"        + emuSuffix_);
    iEvent.put(v2_mpcLink         , emuPrefix_ + "mpcLink"         + emuSuffix_);
    iEvent.put(v2_wire            , emuPrefix_ + "wire"            + emuSuffix_);
    iEvent.put(v2_strip           , emuPrefix_ + "strip"           + emuSuffix_);
    iEvent.put(v2_trackNum        , emuPrefix_ + "trackNum"        + emuSuffix_);
    iEvent.put(v2_quality         , emuPrefix_ + "quality"         + emuSuffix_);
    iEvent.put(v2_pattern         , emuPrefix_ + "pattern"         + emuSuffix_);
    iEvent.put(v2_bend            , emuPrefix_ + "bend"            + emuSuffix_);
    iEvent.put(v2_valid           , emuPrefix_ + "valid"           + emuSuffix_);
    iEvent.put(v2_syncErr         , emuPrefix_ + "syncErr"         + emuSuffix_);
    iEvent.put(v2_bc0             , emuPrefix_ + "bc0"             + emuSuffix_);
    iEvent.put(v2_bx              , emuPrefix_ + "bx"              + emuSuffix_);
    iEvent.put(v2_stubNum         , emuPrefix_ + "stubNum"         + emuSuffix_);
    iEvent.put(v2_bx0             , emuPrefix_ + "bx0"             + emuSuffix_);
    iEvent.put(v2_layer           , emuPrefix_ + "layer"           + emuSuffix_);
    iEvent.put(v2_zone            , emuPrefix_ + "zone"            + emuSuffix_);
    iEvent.put(v2_phiHit          , emuPrefix_ + "phiHit"          + emuSuffix_);
    iEvent.put(v2_phiZone         , emuPrefix_ + "phiZone"         + emuSuffix_);
    iEvent.put(v2_phiLocInt       , emuPrefix_ + "phiLocInt"       + emuSuffix_);
    iEvent.put(v2_phiLocDeg       , emuPrefix_ + "phiLocDeg"       + emuSuffix_);
    iEvent.put(v2_phiLocRad       , emuPrefix_ + "phiLocRad"       + emuSuffix_);
    iEvent.put(v2_phiGlbDeg       , emuPrefix_ + "phiGlbDeg"       + emuSuffix_);
    iEvent.put(v2_phiGlbRad       , emuPrefix_ + "phiGlbRad"       + emuSuffix_);
    iEvent.put(v2_phiGeomRad      , emuPrefix_ + "phiGeomRad"      + emuSuffix_);
    iEvent.put(v2_thetaInt        , emuPrefix_ + "thetaInt"        + emuSuffix_);
    iEvent.put(v2_thetaLoc        , emuPrefix_ + "thetaLoc"        + emuSuffix_);
    iEvent.put(v2_thetaDeg        , emuPrefix_ + "thetaDeg"        + emuSuffix_);
    iEvent.put(v2_thetaRad        , emuPrefix_ + "thetaRad"        + emuSuffix_);
    iEvent.put(v2_eta             , emuPrefix_ + "eta"             + emuSuffix_);
    iEvent.put(v2_size            , emuPrefix_ + "size"            + emuSuffix_);
}

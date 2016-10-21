#include "L1TMuonSimulations/NtupleTools/interface/NtupleEMUTFTriggerPrimitives.h"

#include "L1TMuonSimulations/MuonTools/interface/ModuleIdHelper.h"


NtupleEMUTFTriggerPrimitives::NtupleEMUTFTriggerPrimitives(const edm::ParameterSet& iConfig) :
  unpStubTag_ (iConfig.getParameter<edm::InputTag>("unpStubTag")),
  unpPrefix_  (iConfig.getParameter<std::string>("unpPrefix")),
  unpSuffix_  (iConfig.getParameter<std::string>("unpSuffix")),
  unpSelector_(iConfig.existsAs<std::string>("unpCut") ? iConfig.getParameter<std::string>("unpCut") : "", true),
  emuStubTag_ (iConfig.getParameter<edm::InputTag>("emuStubTag")),
  emuPrefix_  (iConfig.getParameter<std::string>("emuPrefix")),
  emuSuffix_  (iConfig.getParameter<std::string>("emuSuffix")),
  emuSelector_(iConfig.existsAs<std::string>("emuCut") ? iConfig.getParameter<std::string>("emuCut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    unpStubToken_ = consumes<L1TMuonEndCap::EMTFHitCollection>(unpStubTag_);
    emuStubToken_ = consumes<L1TMuonEndCap::EMTFHitExtraCollection>(emuStubTag_);

    produces<std::vector<uint32_t> >          (emuPrefix_ + "geoId"           + emuSuffix_);
    produces<std::vector<uint32_t> >          (emuPrefix_ + "moduleId"        + emuSuffix_);
    produces<std::vector<bool> >              (emuPrefix_ + "evenBit"         + emuSuffix_);
    produces<std::vector<bool> >              (emuPrefix_ + "frBit"           + emuSuffix_);
    produces<std::vector<bool> >              (emuPrefix_ + "ccwBit"          + emuSuffix_);
    produces<std::vector<bool> >              (emuPrefix_ + "neighBit"        + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "iendcap"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "istation"        + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "iring"           + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "ichamber"        + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "isector"         + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "isubsector"      + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "cscID"           + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "cscnID"          + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "bx"              + emuSuffix_);
    produces<std::vector<int16_t> >           (emuPrefix_ + "subsystem"       + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "pcSector"        + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "pcStation"       + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "pcChamber"       + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "pcSegment"       + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "valid"           + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "strip"           + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "wire"            + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "quality"         + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "pattern"         + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "bend"            + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "phi"             + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "theta"           + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "phzvl"           + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "phhit"           + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "zoneHit"         + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "zoneCode"        + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "fsSegment"       + emuSuffix_);
    produces<std::vector<uint16_t> >          (emuPrefix_ + "fsZoneCode"      + emuSuffix_);
    produces<unsigned>                        (emuPrefix_ + "size"            + emuSuffix_);
}

NtupleEMUTFTriggerPrimitives::~NtupleEMUTFTriggerPrimitives() {}

void NtupleEMUTFTriggerPrimitives::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<uint32_t> >          v2_geoId           (new std::vector<uint32_t>());
    std::auto_ptr<std::vector<uint32_t> >          v2_moduleId        (new std::vector<uint32_t>());
    std::auto_ptr<std::vector<bool> >              v2_evenBit         (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >              v2_frBit           (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >              v2_ccwBit          (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >              v2_neighBit        (new std::vector<bool>());
    std::auto_ptr<std::vector<int16_t> >           v2_iendcap         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_istation        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_iring           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_ichamber        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_isector         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_isubsector      (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_cscID           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_cscnID          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_bx              (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v2_subsystem       (new std::vector<int16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_pcSector        (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_pcStation       (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_pcChamber       (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_pcSegment       (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_valid           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_strip           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_wire            (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_quality         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_pattern         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_bend            (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_phi             (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_theta           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_phzvl           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_phhit           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_zoneHit         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_zoneCode        (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_fsSegment       (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v2_fsZoneCode      (new std::vector<uint16_t>());
    std::auto_ptr<unsigned>                        v2_size            (new unsigned(0));


    //__________________________________________________________________________
    edm::Handle<L1TMuonEndCap::EMTFHitExtraCollection> emuStubs;
    //iEvent.getByLabel(emuStubTag_, emuStubs);
    if (!emuStubToken_.isUninitialized())
        iEvent.getByToken(emuStubToken_, emuStubs);

    if (emuStubs.isValid()) {
        edm::LogInfo("NtupleEMUTFTriggerPrimitives") << "Emulator size: " << emuStubs->size();

        unsigned n = 0;
        for (L1TMuonEndCap::EMTFHitExtraCollection::const_iterator it = emuStubs->begin(); it != emuStubs->end(); ++it) {
            if (n >= maxN_)
                break;
            if (!emuSelector_(*it))
                continue;

            //if (it->Neighbor() != 0)  continue;
            //if (!it->Is_CSC_hit())  continue;

            const CSCDetId cscDet(it->detId);
            const uint32_t moduleId = ModuleIdHelper::getModuleId(cscDet);
            const bool evenBit      = ModuleIdHelper::isEven(cscDet);
            const bool frBit        = ModuleIdHelper::isFront(cscDet);
            const bool ccwBit       = ModuleIdHelper::isCounterClockwise(cscDet);
            const bool neighBit     = ModuleIdHelper::isNeighbor(cscDet);

            // Fill the vectors
            v2_geoId           ->push_back(cscDet.rawId());
            v2_moduleId        ->push_back(moduleId);
            v2_evenBit         ->push_back(evenBit);
            v2_frBit           ->push_back(frBit);
            v2_ccwBit          ->push_back(ccwBit);
            v2_neighBit        ->push_back(neighBit);
            v2_iendcap         ->push_back(it->endcap);
            v2_istation        ->push_back(it->station);
            v2_iring           ->push_back(it->ring);
            v2_ichamber        ->push_back(it->chamber);
            v2_isector         ->push_back(it->sector);
            v2_isubsector      ->push_back(it->subsector);
            v2_cscID           ->push_back(it->csc_ID);
            v2_cscnID          ->push_back(it->cscn_ID);
            v2_bx              ->push_back(it->bx);
            v2_subsystem       ->push_back(it->subsystem);
            v2_pcSector        ->push_back(it->pc_sector);
            v2_pcStation       ->push_back(it->pc_station);
            v2_pcChamber       ->push_back(it->pc_chamber);
            v2_pcSegment       ->push_back(it->pc_segment);
            v2_valid           ->push_back(it->valid);
            v2_strip           ->push_back(it->strip);
            v2_wire            ->push_back(it->wire);
            v2_quality         ->push_back(it->quality);
            v2_pattern         ->push_back(it->pattern);
            v2_bend            ->push_back(it->bend);
            v2_phi             ->push_back(it->phi_fp);
            v2_theta           ->push_back(it->theta_fp);
            v2_phzvl           ->push_back(it->phzvl);
            v2_phhit           ->push_back(it->ph_hit);
            v2_zoneHit         ->push_back(it->zone_hit);
            v2_zoneCode        ->push_back(it->zone_code);
            v2_fsSegment       ->push_back(it->fs_segment);
            v2_fsZoneCode      ->push_back(it->fs_zone_code);
        }

        *v2_size = v2_geoId->size();

    } else {
        edm::LogError("NtupleEMUTFTriggerPrimitives") << "Cannot get the product: " << emuStubTag_;
    }


    //__________________________________________________________________________
    iEvent.put(v2_geoId           , emuPrefix_ + "geoId"           + emuSuffix_);
    iEvent.put(v2_moduleId        , emuPrefix_ + "moduleId"        + emuSuffix_);
    iEvent.put(v2_evenBit         , emuPrefix_ + "evenBit"         + emuSuffix_);
    iEvent.put(v2_frBit           , emuPrefix_ + "frBit"           + emuSuffix_);
    iEvent.put(v2_ccwBit          , emuPrefix_ + "ccwBit"          + emuSuffix_);
    iEvent.put(v2_neighBit        , emuPrefix_ + "neighBit"        + emuSuffix_);
    iEvent.put(v2_iendcap         , emuPrefix_ + "iendcap"         + emuSuffix_);
    iEvent.put(v2_istation        , emuPrefix_ + "istation"        + emuSuffix_);
    iEvent.put(v2_iring           , emuPrefix_ + "iring"           + emuSuffix_);
    iEvent.put(v2_ichamber        , emuPrefix_ + "ichamber"        + emuSuffix_);
    iEvent.put(v2_isector         , emuPrefix_ + "isector"         + emuSuffix_);
    iEvent.put(v2_isubsector      , emuPrefix_ + "isubsector"      + emuSuffix_);
    iEvent.put(v2_cscID           , emuPrefix_ + "cscID"           + emuSuffix_);
    iEvent.put(v2_cscnID          , emuPrefix_ + "cscnID"          + emuSuffix_);
    iEvent.put(v2_bx              , emuPrefix_ + "bx"              + emuSuffix_);
    iEvent.put(v2_subsystem       , emuPrefix_ + "subsystem"       + emuSuffix_);
    iEvent.put(v2_pcSector        , emuPrefix_ + "pcSector"        + emuSuffix_);
    iEvent.put(v2_pcStation       , emuPrefix_ + "pcStation"       + emuSuffix_);
    iEvent.put(v2_pcChamber       , emuPrefix_ + "pcChamber"       + emuSuffix_);
    iEvent.put(v2_pcSegment       , emuPrefix_ + "pcSegment"       + emuSuffix_);
    iEvent.put(v2_valid           , emuPrefix_ + "valid"           + emuSuffix_);
    iEvent.put(v2_strip           , emuPrefix_ + "strip"           + emuSuffix_);
    iEvent.put(v2_wire            , emuPrefix_ + "wire"            + emuSuffix_);
    iEvent.put(v2_quality         , emuPrefix_ + "quality"         + emuSuffix_);
    iEvent.put(v2_pattern         , emuPrefix_ + "pattern"         + emuSuffix_);
    iEvent.put(v2_bend            , emuPrefix_ + "bend"            + emuSuffix_);
    iEvent.put(v2_phi             , emuPrefix_ + "phi"             + emuSuffix_);
    iEvent.put(v2_theta           , emuPrefix_ + "theta"           + emuSuffix_);
    iEvent.put(v2_phzvl           , emuPrefix_ + "phzvl"           + emuSuffix_);
    iEvent.put(v2_phhit           , emuPrefix_ + "phhit"           + emuSuffix_);
    iEvent.put(v2_zoneHit         , emuPrefix_ + "zoneHit"         + emuSuffix_);
    iEvent.put(v2_zoneCode        , emuPrefix_ + "zoneCode"        + emuSuffix_);
    iEvent.put(v2_fsSegment       , emuPrefix_ + "fsSegment"       + emuSuffix_);
    iEvent.put(v2_fsZoneCode      , emuPrefix_ + "fsZoneCode"      + emuSuffix_);
    iEvent.put(v2_size            , emuPrefix_ + "size"            + emuSuffix_);
}

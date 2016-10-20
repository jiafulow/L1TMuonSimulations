#include "L1TMuonSimulations/NtupleTools/interface/NtupleCSCTriggerPrimitives.h"

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/CSCTriggerNumbering.h"
#include "L1Trigger/CSCCommonTrigger/interface/CSCConstants.h"
//#include "L1Trigger/CSCCommonTrigger/interface/CSCPatternLUT.h"

#include "L1TMuonSimulations/MuonTools/interface/ModuleIdHelper.h"


NtupleCSCTriggerPrimitives::NtupleCSCTriggerPrimitives(const edm::ParameterSet& iConfig) :
  //wireTag_(iConfig.getParameter<edm::InputTag>("wireTag")),
  //stripTag_(iConfig.getParameter<edm::InputTag>("stripTag")),
  //compTag_(iConfig.getParameter<edm::InputTag>("compTag")),
  //alctTag_(iConfig.getParameter<edm::InputTag>("alctTag")),
  //clctTag_(iConfig.getParameter<edm::InputTag>("clctTag")),
  corrlctTag_(iConfig.getParameter<edm::InputTag>("corrlctTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    theGeometryTranslator_.reset(new L1TMuon::GeometryTranslator2());

    //wireToken_ = consumes<CSCCorrelatedLCTDigiCollection>(wireTag_);
    //stripToken_ = consumes<CSCCorrelatedLCTDigiCollection>(stripTag_);
    //compToken_ = consumes<CSCCorrelatedLCTDigiCollection>(compTag_);
    //alctToken_ = consumes<CSCCorrelatedLCTDigiCollection>(alctTag_);
    //clctToken_ = consumes<CSCCorrelatedLCTDigiCollection>(clctTag_);
    corrlctToken_ = consumes<CSCCorrelatedLCTDigiCollection>(corrlctTag_);

    produces<std::vector<uint32_t> >          (prefix_ + "geoId"           + suffix_);
    //produces<std::vector<unsigned> >          (prefix_ + "subsystem"       + suffix_);
    produces<std::vector<uint32_t> >          (prefix_ + "moduleId"        + suffix_);
    produces<std::vector<bool> >              (prefix_ + "evenBit"         + suffix_);
    produces<std::vector<bool> >              (prefix_ + "frBit"           + suffix_);
    produces<std::vector<bool> >              (prefix_ + "ccwBit"          + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "globalSector"    + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "globalSubSector" + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "iendcap"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "istation"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "iring"           + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "ichamber"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "isector"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "isubsector"      + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "trknmb"          + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "valid"           + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "quality"         + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "keywire"         + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "strip"           + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "pattern"         + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "bend"            + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "bx"              + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "mpclink"         + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "bx0"             + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "syncErr"         + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "cscID"           + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalPhi"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalTheta"     + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalEta"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalRho"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalX"         + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalY"         + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalZ"         + suffix_);
    produces<std::vector<float> >             (prefix_ + "stripPhiPitch"   + suffix_);
    produces<std::vector<float> >             (prefix_ + "wirePitch"       + suffix_);
    produces<unsigned>                        (prefix_ + "size"            + suffix_);
}

NtupleCSCTriggerPrimitives::~NtupleCSCTriggerPrimitives() {}

void NtupleCSCTriggerPrimitives::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    /// Geometry setup
    edm::ESHandle<CSCGeometry> cscGeometryHandle;
    iSetup.get<MuonGeometryRecord>().get(cscGeometryHandle);
    if (!cscGeometryHandle.isValid()) {
        edm::LogWarning("NtupleCSCTriggerPrimitives") << "Unable to get MuonGeometryRecord!";
    } else {
        theCSCGeometry_ = cscGeometryHandle.product();
    }

    /// Magnetic field setup
    //edm::ESHandle<MagneticField> magneticFieldHandle;
    //iSetup.get<IdealMagneticFieldRecord>().get(magneticFieldHandle);
    //if (!magneticFieldHandle.isValid()) {
    //    edm::LogWarning("NtupleCSCTriggerPrimitives") << "Unable to get IdealMagneticFieldRecord!";
    //} else {
    //    theMagneticField_ = magneticFieldHandle.product();
    //}

    theGeometryTranslator_->checkAndUpdateGeometry(iSetup);
}

// _____________________________________________________________________________
using L1TMuon::TriggerPrimitive;
using L1TMuon::TriggerPrimitiveCollection;

void NtupleCSCTriggerPrimitives::extractPrimitives(edm::Handle<CSCCorrelatedLCTDigiCollection> cscDigis,
        L1TMuon::TriggerPrimitiveCollection& out) const {

  auto chamber = cscDigis->begin();
  auto chend  = cscDigis->end();
  for( ; chamber != chend; ++chamber ) {
    auto digi = (*chamber).second.first;
    auto dend = (*chamber).second.second;
    for( ; digi != dend; ++digi ) {
      out.push_back(TriggerPrimitive((*chamber).first,*digi));
    }
  }
}

// _____________________________________________________________________________
void NtupleCSCTriggerPrimitives::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<uint32_t> >          v_geoId           (new std::vector<uint32_t>());
    //std::auto_ptr<std::vector<unsigned> >          v_subsystem       (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_globalSector    (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_globalSubSector (new std::vector<unsigned>());
    std::auto_ptr<std::vector<uint32_t> >          v_moduleId        (new std::vector<uint32_t>());
    std::auto_ptr<std::vector<bool> >              v_evenBit         (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >              v_frBit           (new std::vector<bool>());
    std::auto_ptr<std::vector<bool> >              v_ccwBit          (new std::vector<bool>());
    std::auto_ptr<std::vector<int16_t> >           v_iendcap         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_istation        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_iring           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_ichamber        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_isector         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_isubsector      (new std::vector<int16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_trknmb          (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_valid           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_quality         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_keywire         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_strip           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_pattern         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_bend            (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_bx              (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_mpclink         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_bx0             (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_syncErr         (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_cscID           (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<float> >             v_globalPhi       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalTheta     (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalEta       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalRho       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalX         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalY         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalZ         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_stripPhiPitch   (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_wirePitch       (new std::vector<float>());
    std::auto_ptr<unsigned>                        v_size            (new unsigned(0));


    //__________________________________________________________________________

    edm::Handle<CSCCorrelatedLCTDigiCollection> corrlcts;
    //iEvent.getByLabel(corrlctTag_, corrlcts);
    if (!corrlctToken_.isUninitialized())
        iEvent.getByToken(corrlctToken_, corrlcts);

    if (corrlcts.isValid()) {
        //edm::LogInfo("NtupleCSCTriggerPrimitives") << "Size: " << corrlcts->size();
        edm::LogInfo("NtupleCSCTriggerPrimitives") << "Size: ??";

        // Make trigger primitives
        L1TMuon::TriggerPrimitiveCollection trigPrims;
        extractPrimitives(corrlcts, trigPrims);

        // Loop over trigger primitives
        unsigned n = 0;
        for (L1TMuon::TriggerPrimitiveCollection::const_iterator it = trigPrims.cbegin(); it != trigPrims.cend(); ++it) {
            if (n >= maxN_)
                break;

            if (it->subsystem() == L1TMuon::TriggerPrimitive::kCSC) {

                const CSCDetId cscDet = it->detId<CSCDetId>();
                const L1TMuon::TriggerPrimitive::CSCData cscData = it->getCSCData();

                assert(cscDet.rawId() == it->rawId());
                const uint32_t moduleId = ModuleIdHelper::getModuleId(cscDet);
                const bool evenBit      = ModuleIdHelper::isEven(cscDet);
                const bool frBit        = ModuleIdHelper::isFront(cscDet);
                const bool ccwBit       = ModuleIdHelper::isCounterClockwise(cscDet);

                const int isector = (cscDet.endcap()-1)*6 + (cscDet.triggerSector()-1);
                const int isubsector = (cscDet.station() != 1) ? 0 : ((cscDet.chamber()%6 > 2) ? 1 : 2);

                // Global coordinates
                //const double globalPhi = theGeometryTranslator_->calculateGlobalPhi(*it);
                //const double globalEta = theGeometryTranslator_->calculateGlobalEta(*it);
                const GlobalPoint& gp = theGeometryTranslator_->getGlobalPoint(*it);

                // Sanity checks
                assert(CSCTriggerNumbering::triggerSectorFromLabels(cscDet) == cscDet.triggerSector());
                assert(CSCTriggerNumbering::triggerCscIdFromLabels(cscDet) == cscDet.triggerCscId());
                assert(CSCTriggerNumbering::triggerSubSectorFromLabels(cscDet) == (int) isubsector);
                assert(cscData.cscID == cscDet.triggerCscId());

                const CSCLayer * cscLayer = theCSCGeometry_->layer(cscDet);     // CSCLayer is inherited from GeomDetUnit
                const CSCLayerGeometry * cscLayerGeom = cscLayer->geometry();   // CSCLayerGeometry is inherited from TrapezoidalPlaneBounds
                //const CSCChamber * cscChamber = cscLayer->chamber();            // CSCChamber is inherited from GeomDet

                // Geom
                float stripPhiPitch = cscLayerGeom->stripPhiPitch();  // angular pitch
                float wirePitch = cscLayerGeom->wirePitch();


                // Fill the vectors
                v_geoId           ->push_back(it->rawId().rawId());
                //v_subsystem       ->push_back(it->subsystem());
                v_globalSector    ->push_back(it->getGlobalSector());
                v_globalSubSector ->push_back(it->getSubSector());
                v_moduleId        ->push_back(moduleId);
                v_evenBit         ->push_back(evenBit);
                v_frBit           ->push_back(frBit);
                v_ccwBit          ->push_back(ccwBit);
                v_iendcap         ->push_back(cscDet.endcap());
                v_istation        ->push_back(cscDet.station());
                v_iring           ->push_back(cscDet.ring());
                v_ichamber        ->push_back(cscDet.chamber());
                v_isector         ->push_back(isector);
                v_isubsector      ->push_back(isubsector);
                v_trknmb          ->push_back(cscData.trknmb);
                v_valid           ->push_back(cscData.valid);
                v_quality         ->push_back(cscData.quality);
                v_keywire         ->push_back(cscData.keywire);  // wire group 0-111
                v_strip           ->push_back(cscData.strip);    // half-strip precision 0-159
                v_pattern         ->push_back(cscData.pattern);  // 0-10
                v_bend            ->push_back(cscData.bend);
                v_bx              ->push_back(cscData.bx);
                v_mpclink         ->push_back(cscData.mpclink);
                v_bx0             ->push_back(cscData.bx0);
                v_syncErr         ->push_back(cscData.syncErr);
                v_cscID           ->push_back(cscData.cscID);
                v_globalPhi       ->push_back(gp.phi());
                v_globalTheta     ->push_back(gp.theta());
                v_globalEta       ->push_back(gp.eta());
                v_globalRho       ->push_back(gp.perp());
                v_globalX         ->push_back(gp.x());
                v_globalY         ->push_back(gp.y());
                v_globalZ         ->push_back(gp.z());
                v_stripPhiPitch   ->push_back(stripPhiPitch);
                v_wirePitch       ->push_back(wirePitch);

                n++;
            }
        }

        *v_size = v_geoId->size();

    } else {
        edm::LogError("NtupleCSCTriggerPrimitives") << "Cannot get the product: " << corrlctTag_;
    }


    //__________________________________________________________________________
    iEvent.put(v_geoId           , prefix_ + "geoId"           + suffix_);
    //iEvent.put(v_subsystem       , prefix_ + "subsystem"       + suffix_);
    iEvent.put(v_globalSector    , prefix_ + "globalSector"    + suffix_);
    iEvent.put(v_globalSubSector , prefix_ + "globalSubSector" + suffix_);
    iEvent.put(v_moduleId        , prefix_ + "moduleId"        + suffix_);
    iEvent.put(v_evenBit         , prefix_ + "evenBit"         + suffix_);
    iEvent.put(v_frBit           , prefix_ + "frBit"           + suffix_);
    iEvent.put(v_ccwBit          , prefix_ + "ccwBit"          + suffix_);
    iEvent.put(v_iendcap         , prefix_ + "iendcap"         + suffix_);
    iEvent.put(v_istation        , prefix_ + "istation"        + suffix_);
    iEvent.put(v_iring           , prefix_ + "iring"           + suffix_);
    iEvent.put(v_ichamber        , prefix_ + "ichamber"        + suffix_);
    iEvent.put(v_isector         , prefix_ + "isector"         + suffix_);
    iEvent.put(v_isubsector      , prefix_ + "isubsector"      + suffix_);
    iEvent.put(v_trknmb          , prefix_ + "trknmb"          + suffix_);
    iEvent.put(v_valid           , prefix_ + "valid"           + suffix_);
    iEvent.put(v_quality         , prefix_ + "quality"         + suffix_);
    iEvent.put(v_keywire         , prefix_ + "keywire"         + suffix_);
    iEvent.put(v_strip           , prefix_ + "strip"           + suffix_);
    iEvent.put(v_pattern         , prefix_ + "pattern"         + suffix_);
    iEvent.put(v_bend            , prefix_ + "bend"            + suffix_);
    iEvent.put(v_bx              , prefix_ + "bx"              + suffix_);
    iEvent.put(v_mpclink         , prefix_ + "mpclink"         + suffix_);
    iEvent.put(v_bx0             , prefix_ + "bx0"             + suffix_);
    iEvent.put(v_syncErr         , prefix_ + "syncErr"         + suffix_);
    iEvent.put(v_cscID           , prefix_ + "cscID"           + suffix_);
    iEvent.put(v_globalPhi       , prefix_ + "globalPhi"       + suffix_);
    iEvent.put(v_globalTheta     , prefix_ + "globalTheta"     + suffix_);
    iEvent.put(v_globalEta       , prefix_ + "globalEta"       + suffix_);
    iEvent.put(v_globalRho       , prefix_ + "globalRho"       + suffix_);
    iEvent.put(v_globalX         , prefix_ + "globalX"         + suffix_);
    iEvent.put(v_globalY         , prefix_ + "globalY"         + suffix_);
    iEvent.put(v_globalZ         , prefix_ + "globalZ"         + suffix_);
    iEvent.put(v_stripPhiPitch   , prefix_ + "stripPhiPitch"   + suffix_);
    iEvent.put(v_wirePitch       , prefix_ + "wirePitch"       + suffix_);
    iEvent.put(v_size            , prefix_ + "size"            + suffix_);
}

#include "L1TMuonSimulations/NtupleTools/interface/NtupleDTTriggerPrimitives.h"

#include "DataFormats/MuonDetId/interface/DTChamberId.h"


NtupleDTTriggerPrimitives::NtupleDTTriggerPrimitives(const edm::ParameterSet& iConfig) :
  bx_min(iConfig.existsAs<int>("BX_min") ? iConfig.getParameter<int>("BX_min") : -9),
  bx_max(iConfig.existsAs<int>("BX_max") ? iConfig.getParameter<int>("BX_max") : 7),
  //
  dtPhTag_   (iConfig.getParameter<edm::InputTag>("dtPhTag")),
  dtThTag_   (iConfig.getParameter<edm::InputTag>("dtThTag")),
  prefix_    (iConfig.getParameter<std::string>("prefix")),
  suffix_    (iConfig.getParameter<std::string>("suffix")),
  selectorPh_(iConfig.existsAs<std::string>("cutPh") ? iConfig.getParameter<std::string>("cutPh") : "", true),
  selectorTh_(iConfig.existsAs<std::string>("cutTh") ? iConfig.getParameter<std::string>("cutTh") : "", true),
  maxN_      (iConfig.getParameter<unsigned>("maxN")) {

    if (iConfig.existsAs<edm::ParameterSet>("bxCleanerCfg")) {
        edm::ParameterSet bxccfg = iConfig.getParameterSet("bxCleanerCfg");
        _bxc.reset(new L1TMuon::DTBunchCrossingCleaner(bxccfg));
    } else {
        edm::ParameterSet bxccfg;
        bxccfg.addParameter<int>("bxWindowSize", 1);
        _bxc.reset(new L1TMuon::DTBunchCrossingCleaner(bxccfg));
    }

    theGeometryTranslator_.reset(new L1TMuon::GeometryTranslator2());

    dtPhToken_ = consumes<L1MuDTChambPhContainer>(dtPhTag_);
    dtThToken_ = consumes<L1MuDTChambThContainer>(dtThTag_);

    produces<std::vector<uint32_t> >          (prefix_ + "geoId"           + suffix_);
    //produces<std::vector<unsigned> >          (prefix_ + "subsystem"       + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "globalSector"    + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "globalSubSector" + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "iwheel"          + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "istation"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "isector"         + suffix_);
    produces<std::vector<int> >               (prefix_ + "bx"              + suffix_);
    produces<std::vector<int> >               (prefix_ + "wheel"           + suffix_);
    produces<std::vector<int> >               (prefix_ + "sector"          + suffix_);
    produces<std::vector<int> >               (prefix_ + "station"         + suffix_);
    produces<std::vector<int> >               (prefix_ + "radialAngle"     + suffix_);
    produces<std::vector<int> >               (prefix_ + "bendingAngle"    + suffix_);
    produces<std::vector<int> >               (prefix_ + "qualityCode"     + suffix_);
    produces<std::vector<int> >               (prefix_ + "ts2TagCode"      + suffix_);
    produces<std::vector<int> >               (prefix_ + "bxCntCode"       + suffix_);
    produces<std::vector<int> >               (prefix_ + "thetaBtiGroup"   + suffix_);
    produces<std::vector<int> >               (prefix_ + "segmentNumber"   + suffix_);
    produces<std::vector<int> >               (prefix_ + "thetaCode"       + suffix_);
    produces<std::vector<int> >               (prefix_ + "thetaQuality"    + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalPhi"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalTheta"     + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalEta"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalRho"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalX"         + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalY"         + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalZ"         + suffix_);
    produces<unsigned>                        (prefix_ + "size"            + suffix_);
}

NtupleDTTriggerPrimitives::~NtupleDTTriggerPrimitives() {}

void NtupleDTTriggerPrimitives::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    theGeometryTranslator_->checkAndUpdateGeometry(iSetup);
}

// _____________________________________________________________________________
// Taken from L1Trigger/L1TMuonBarrel/src/Twinmux_v1/DTCollector.cc
using L1TMuon::TriggerPrimitive;
using L1TMuon::TriggerPrimitiveCollection;

void NtupleDTTriggerPrimitives::extractPrimitives(edm::Handle<L1MuDTChambPhContainer> phiDigis,
        edm::Handle<L1MuDTChambThContainer> thetaDigis,
        L1TMuon::TriggerPrimitiveCollection& out) const {

  TriggerPrimitiveCollection cleaned, temp, chamb_list;

  for( int wheel = -2; wheel <= 2 ; ++wheel ) {
    for( int station = 1; station <= 4; ++station ) {
      for( int sector = 0; sector <= 11; ++sector ) {
        chamb_list.clear();
        for( int bx = bx_min; bx <= bx_max; ++bx) {
          std::unique_ptr<const L1MuDTChambPhDigi> phi_segm_1(
            phiDigis->chPhiSegm1(wheel,station,sector,bx)
            );
          std::unique_ptr<const L1MuDTChambPhDigi> phi_segm_2(
            phiDigis->chPhiSegm2(wheel,station,sector,bx)
            );
          std::unique_ptr<const L1MuDTChambThDigi> theta_segm(
            thetaDigis->chThetaSegm(wheel,station,sector,bx)
            );

          int bti_group_1=-1, bti_group_2=-1;

          if( theta_segm ) {
            bti_group_1 = findBTIGroupForThetaDigi(*theta_segm,1);
            bti_group_2 = findBTIGroupForThetaDigi(*theta_segm,2);
          }

          if( phi_segm_1 && bti_group_1 != -1 ) {
            chamb_list.push_back(processDigis(*phi_segm_1,
                                              *theta_segm,
                                              bti_group_1));
          } else if ( phi_segm_1 && bti_group_1 == -1 ) {
            chamb_list.push_back(processDigis(*phi_segm_1,1));
          } else if ( !phi_segm_1 && bti_group_1 != -1 ) {
            chamb_list.push_back(processDigis(*theta_segm,
                                              bti_group_1));
          }

          if( phi_segm_2 && bti_group_2 != -1) {
            chamb_list.push_back(processDigis(*phi_segm_2,
                                              *theta_segm,
                                              bti_group_2));
          } else if ( phi_segm_2 && bti_group_2 == -1 ) {
            chamb_list.push_back(processDigis(*phi_segm_2,2));
          } else if ( !phi_segm_2 && bti_group_2 != -1 ) {
            chamb_list.push_back(processDigis(*phi_segm_2,bti_group_2));
          }

          phi_segm_1.release();
          phi_segm_2.release();
          theta_segm.release();
        }
        if( _bxc ) {
          temp = _bxc->clean(chamb_list);
          cleaned.insert(cleaned.end(),temp.begin(),temp.end());
        } else {
          cleaned.insert(cleaned.end(),chamb_list.begin(),chamb_list.end());
        }
      }
    }
  }
  out.insert(out.end(),cleaned.begin(),cleaned.end());
}

L1TMuon::TriggerPrimitive NtupleDTTriggerPrimitives::processDigis(const L1MuDTChambPhDigi& digi,
        const int &segment_number) const {
  DTChamberId detid(digi.whNum(),digi.stNum(),digi.scNum()+1);
  return TriggerPrimitive(detid,digi,segment_number);
}

L1TMuon::TriggerPrimitive NtupleDTTriggerPrimitives::processDigis(const L1MuDTChambThDigi& digi_th,
        const int bti_group) const {
  DTChamberId detid(digi_th.whNum(),digi_th.stNum(),digi_th.scNum()+1);
  return TriggerPrimitive(detid,digi_th,bti_group);
}

L1TMuon::TriggerPrimitive NtupleDTTriggerPrimitives::processDigis(const L1MuDTChambPhDigi& digi_phi,
        const L1MuDTChambThDigi& digi_theta,
        const int bti_group) const {
  DTChamberId detid(digi_phi.whNum(),digi_phi.stNum(),digi_phi.scNum()+1);
  return TriggerPrimitive(detid,digi_phi,digi_theta,bti_group);
}

int NtupleDTTriggerPrimitives::findBTIGroupForThetaDigi(const L1MuDTChambThDigi& digi,
        const int pos) const {
  //if( digi.stNum() == 4 ) return -1; // there is no theta layer there
  int result = -1;
  for( int i = 0; i < 7; ++i ) {
    if( digi.position(i) == pos ) result = i;
  }
  return result;
}

// _____________________________________________________________________________
void NtupleDTTriggerPrimitives::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<uint32_t> >          v_geoId           (new std::vector<uint32_t>());
    //std::auto_ptr<std::vector<unsigned> >          v_subsystem       (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_globalSector    (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_globalSubSector (new std::vector<unsigned>());
    std::auto_ptr<std::vector<int16_t> >           v_iwheel          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_istation        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_isector         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int> >               v_bx              (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_wheel           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_sector          (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_station         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_radialAngle     (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_bendingAngle    (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_qualityCode     (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_ts2TagCode      (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_bxCntCode       (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_thetaBtiGroup   (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_segmentNumber   (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_thetaCode       (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_thetaQuality    (new std::vector<int>());
    std::auto_ptr<std::vector<float> >             v_globalPhi       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalTheta     (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalEta       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalRho       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalX         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalY         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalZ         (new std::vector<float>());
    std::auto_ptr<unsigned>                        v_size            (new unsigned(0));


    //__________________________________________________________________________
    edm::Handle<L1MuDTChambPhContainer> dtPhDigis;
    edm::Handle<L1MuDTChambThContainer> dtThDigis;
    //iEvent.getByLabel(dtPhTag_, dtPhDigis);
    //iEvent.getByLabel(dtThTag_, dtThDigis);
    if (!dtPhToken_.isUninitialized())
        iEvent.getByToken(dtPhToken_, dtPhDigis);
    if (!dtThToken_.isUninitialized())
        iEvent.getByToken(dtThToken_, dtThDigis);

    if (dtPhDigis.isValid() && dtThDigis.isValid()) {
        //edm::LogInfo("NtupleDTTriggerPrimitives") << "Size: " << dtPhDigis->size();
        edm::LogInfo("NtupleDTTriggerPrimitives") << "Size: ??";

        // Make trigger primitives
        L1TMuon::TriggerPrimitiveCollection trigPrims;
        extractPrimitives(dtPhDigis, dtThDigis, trigPrims);

        // Loop over trigger primitives
        unsigned n = 0;
        for (L1TMuon::TriggerPrimitiveCollection::const_iterator it = trigPrims.cbegin(); it != trigPrims.cend(); ++it) {
            if (n >= maxN_)
                break;

            if (it->subsystem() == L1TMuon::TriggerPrimitive::kDT) {

                const DTChamberId dtDet = it->detId<DTChamberId>();
                const L1TMuon::TriggerPrimitive::DTData dtData = it->getDTData();

                // Global coordinates
                //const double globalPhi = theGeometryTranslator_->calculateGlobalPhi(*it);
                //const double globalEta = theGeometryTranslator_->calculateGlobalEta(*it);
                const GlobalPoint& gp = theGeometryTranslator_->getGlobalPoint(*it);


                // Fill the vectors
                v_geoId           ->push_back(it->rawId().rawId());
                //v_subsystem       ->push_back(it->subsystem());
                v_globalSector    ->push_back(it->getGlobalSector());
                v_globalSubSector ->push_back(it->getSubSector());
                v_iwheel          ->push_back(dtDet.wheel());
                v_istation        ->push_back(dtDet.station());
                v_isector         ->push_back(dtDet.sector());
                v_bx              ->push_back(dtData.bx);
                v_wheel           ->push_back(dtData.wheel);
                v_sector          ->push_back(dtData.sector);
                v_station         ->push_back(dtData.station);
                v_radialAngle     ->push_back(dtData.radialAngle);
                v_bendingAngle    ->push_back(dtData.bendingAngle);
                v_qualityCode     ->push_back(dtData.qualityCode);
                v_ts2TagCode      ->push_back(dtData.Ts2TagCode);
                v_bxCntCode       ->push_back(dtData.BxCntCode);
                v_thetaBtiGroup   ->push_back(dtData.theta_bti_group);
                v_segmentNumber   ->push_back(dtData.segment_number);
                v_thetaCode       ->push_back(dtData.theta_code);
                v_thetaQuality    ->push_back(dtData.theta_quality);
                v_globalPhi       ->push_back(gp.phi());
                v_globalTheta     ->push_back(gp.theta());
                v_globalEta       ->push_back(gp.eta());
                v_globalRho       ->push_back(gp.perp());
                v_globalX         ->push_back(gp.x());
                v_globalY         ->push_back(gp.y());
                v_globalZ         ->push_back(gp.z());

                n++;
            }
        }

        *v_size = v_geoId->size();

    } else {
        edm::LogError("NtupleDTTriggerPrimitives") << "Cannot get the products: " << dtPhTag_ << ", " << dtThTag_;
    }


    //__________________________________________________________________________
    iEvent.put(v_geoId           , prefix_ + "geoId"           + suffix_);
    //iEvent.put(v_subsystem       , prefix_ + "subsystem"       + suffix_);
    iEvent.put(v_globalSector    , prefix_ + "globalSector"    + suffix_);
    iEvent.put(v_globalSubSector , prefix_ + "globalSubSector" + suffix_);
    iEvent.put(v_iwheel          , prefix_ + "iwheel"          + suffix_);
    iEvent.put(v_istation        , prefix_ + "istation"        + suffix_);
    iEvent.put(v_isector         , prefix_ + "isector"         + suffix_);
    iEvent.put(v_bx              , prefix_ + "bx"              + suffix_);
    iEvent.put(v_wheel           , prefix_ + "wheel"           + suffix_);
    iEvent.put(v_sector          , prefix_ + "sector"          + suffix_);
    iEvent.put(v_station         , prefix_ + "station"         + suffix_);
    iEvent.put(v_radialAngle     , prefix_ + "radialAngle"     + suffix_);
    iEvent.put(v_bendingAngle    , prefix_ + "bendingAngle"    + suffix_);
    iEvent.put(v_qualityCode     , prefix_ + "qualityCode"     + suffix_);
    iEvent.put(v_ts2TagCode      , prefix_ + "ts2TagCode"      + suffix_);
    iEvent.put(v_bxCntCode       , prefix_ + "bxCntCode"       + suffix_);
    iEvent.put(v_thetaBtiGroup   , prefix_ + "thetaBtiGroup"   + suffix_);
    iEvent.put(v_segmentNumber   , prefix_ + "segmentNumber"   + suffix_);
    iEvent.put(v_thetaCode       , prefix_ + "thetaCode"       + suffix_);
    iEvent.put(v_thetaQuality    , prefix_ + "thetaQuality"    + suffix_);
    iEvent.put(v_globalPhi       , prefix_ + "globalPhi"       + suffix_);
    iEvent.put(v_globalTheta     , prefix_ + "globalTheta"     + suffix_);
    iEvent.put(v_globalEta       , prefix_ + "globalEta"       + suffix_);
    iEvent.put(v_globalRho       , prefix_ + "globalRho"       + suffix_);
    iEvent.put(v_globalX         , prefix_ + "globalX"         + suffix_);
    iEvent.put(v_globalY         , prefix_ + "globalY"         + suffix_);
    iEvent.put(v_globalZ         , prefix_ + "globalZ"         + suffix_);
    iEvent.put(v_size            , prefix_ + "size"            + suffix_);
}

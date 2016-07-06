#include "L1TMuonSimulations/NtupleTools/interface/NtupleRPCTriggerPrimitives.h"

#include "DataFormats/MuonDetId/interface/RPCDetId.h"
//#include "DataFormats/RPCDigi/interface/RPCDigiL1Link.h"


NtupleRPCTriggerPrimitives::NtupleRPCTriggerPrimitives(const edm::ParameterSet& iConfig) :
  inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    theGeometryTranslator_.reset(new L1TMuon::GeometryTranslator2());

    token_ = consumes<RPCDigiCollection>(inputTag_);

    produces<std::vector<uint32_t> >          (prefix_ + "geoId"           + suffix_);
    //produces<std::vector<unsigned> >          (prefix_ + "subsystem"       + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "globalSector"    + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "globalSubSector" + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "iregion"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "iring"           + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "istation"        + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "isector"         + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "ilayer"          + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "isubsector"      + suffix_);
    produces<std::vector<int16_t> >           (prefix_ + "iroll"           + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "strip"           + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "layer"           + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "bx"              + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalPhi"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalTheta"     + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalEta"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalRho"       + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalX"         + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalY"         + suffix_);
    produces<std::vector<float> >             (prefix_ + "globalZ"         + suffix_);
    produces<unsigned>                        (prefix_ + "size"            + suffix_);
}

NtupleRPCTriggerPrimitives::~NtupleRPCTriggerPrimitives() {}

void NtupleRPCTriggerPrimitives::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    theGeometryTranslator_->checkAndUpdateGeometry(iSetup);
}

// _____________________________________________________________________________
using L1TMuon::TriggerPrimitive;
using L1TMuon::TriggerPrimitiveCollection;

void NtupleRPCTriggerPrimitives::extractPrimitives(edm::Handle<RPCDigiCollection> rpcDigis,
        L1TMuon::TriggerPrimitiveCollection& out) const {

  auto chamber = rpcDigis->begin();
  auto chend  = rpcDigis->end();
  for( ; chamber != chend; ++chamber ) {
    auto digi = (*chamber).second.first;
    auto dend = (*chamber).second.second;
    for( ; digi != dend; ++digi ) {
      out.push_back(TriggerPrimitive((*chamber).first,
                                     digi->strip(),
                                     (*chamber).first.layer(),
                                     digi->bx()));
    }
  }
}

// _____________________________________________________________________________
void NtupleRPCTriggerPrimitives::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<uint32_t> >          v_geoId           (new std::vector<uint32_t>());
    //std::auto_ptr<std::vector<unsigned> >          v_subsystem       (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_globalSector    (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_globalSubSector (new std::vector<unsigned>());
    std::auto_ptr<std::vector<int16_t> >           v_iregion         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_iring           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_istation        (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_isector         (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_ilayer          (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_isubsector      (new std::vector<int16_t>());
    std::auto_ptr<std::vector<int16_t> >           v_iroll           (new std::vector<int16_t>());
    std::auto_ptr<std::vector<unsigned> >          v_strip           (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_layer           (new std::vector<unsigned>());
    std::auto_ptr<std::vector<uint16_t> >          v_bx              (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<float> >             v_globalPhi       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalTheta     (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalEta       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalRho       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalX         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalY         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalZ         (new std::vector<float>());
    std::auto_ptr<unsigned>                        v_size            (new unsigned(0));


    //__________________________________________________________________________

    edm::Handle<RPCDigiCollection> rpcDigis;
    //iEvent.getByLabel(inputTag_, rpcDigis);
    if (!token_.isUninitialized())
        iEvent.getByToken(token_, rpcDigis);

    if (rpcDigis.isValid()) {
        //edm::LogInfo("NtupleRPCTriggerPrimitives") << "Size: " << rpcDigis->size();
        edm::LogInfo("NtupleRPCTriggerPrimitives") << "Size: ??";

        // Make trigger primitives
        L1TMuon::TriggerPrimitiveCollection trigPrims;
        extractPrimitives(rpcDigis, trigPrims);

        // Loop over trigger primitives
        unsigned n = 0;
        for (L1TMuon::TriggerPrimitiveCollection::const_iterator it = trigPrims.cbegin(); it != trigPrims.cend(); ++it) {
            if (n >= maxN_)
                break;

            if (it->subsystem() == L1TMuon::TriggerPrimitive::kRPC) {

                const RPCDetId rpcDet = it->detId<RPCDetId>();
                const L1TMuon::TriggerPrimitive::RPCData rpcData = it->getRPCData();

                // Global coordinates
                //const double globalPhi = theGeometryTranslator_->calculateGlobalPhi(*it);
                //const double globalEta = theGeometryTranslator_->calculateGlobalEta(*it);
                const GlobalPoint& gp = theGeometryTranslator_->getGlobalPoint(*it);

                // Fill the vectors
                v_geoId           ->push_back(it->rawId().rawId());
                //v_subsystem       ->push_back(it->subsystem());
                v_globalSector    ->push_back(it->getGlobalSector());
                v_globalSubSector ->push_back(it->getSubSector());
                v_iregion         ->push_back(rpcDet.region());
                v_iring           ->push_back(rpcDet.ring());
                v_istation        ->push_back(rpcDet.station());
                v_isector         ->push_back(rpcDet.sector());
                v_ilayer          ->push_back(rpcDet.layer());
                v_isubsector      ->push_back(rpcDet.subsector());
                v_iroll           ->push_back(rpcDet.roll());
                v_strip           ->push_back(rpcData.strip);
                v_layer           ->push_back(rpcData.layer);
                v_bx              ->push_back(rpcData.bx);
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
        edm::LogError("NtupleRPCTriggerPrimitives") << "Cannot get the products: " << inputTag_;
    }


    //__________________________________________________________________________
    iEvent.put(v_geoId           , prefix_ + "geoId"           + suffix_);
    //iEvent.put(v_subsystem       , prefix_ + "subsystem"       + suffix_);
    iEvent.put(v_globalSector    , prefix_ + "globalSector"    + suffix_);
    iEvent.put(v_globalSubSector , prefix_ + "globalSubSector" + suffix_);
    iEvent.put(v_iregion         , prefix_ + "iregion"         + suffix_);
    iEvent.put(v_iring           , prefix_ + "iring"           + suffix_);
    iEvent.put(v_istation        , prefix_ + "istation"        + suffix_);
    iEvent.put(v_isector         , prefix_ + "isector"         + suffix_);
    iEvent.put(v_ilayer          , prefix_ + "ilayer"          + suffix_);
    iEvent.put(v_isubsector      , prefix_ + "isubsector"      + suffix_);
    iEvent.put(v_iroll           , prefix_ + "iroll"           + suffix_);
    iEvent.put(v_strip           , prefix_ + "strip"           + suffix_);
    iEvent.put(v_layer           , prefix_ + "layer"           + suffix_);
    iEvent.put(v_bx              , prefix_ + "bx"              + suffix_);
    iEvent.put(v_globalPhi       , prefix_ + "globalPhi"       + suffix_);
    iEvent.put(v_globalTheta     , prefix_ + "globalTheta"     + suffix_);
    iEvent.put(v_globalEta       , prefix_ + "globalEta"       + suffix_);
    iEvent.put(v_globalRho       , prefix_ + "globalRho"       + suffix_);
    iEvent.put(v_globalX         , prefix_ + "globalX"         + suffix_);
    iEvent.put(v_globalY         , prefix_ + "globalY"         + suffix_);
    iEvent.put(v_globalZ         , prefix_ + "globalZ"         + suffix_);
    iEvent.put(v_size            , prefix_ + "size"            + suffix_);
}


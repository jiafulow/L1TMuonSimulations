#include "L1TMuonSimulations/NtupleTools/interface/NtupleRPCTriggerPrimitives.h"

#include "DataFormats/MuonDetId/interface/RPCDetId.h"


NtupleRPCTriggerPrimitives::NtupleRPCTriggerPrimitives(const edm::ParameterSet& iConfig) :
  inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    theGeometryTranslator_.reset(new L1TMuon::GeometryTranslator());

    token_ = consumes<RPCDigiCollection>(inputTag_);

    produces<std::vector<uint32_t> >          (prefix_ + "geoId"           + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "subsystem"       + suffix_);
    produces<std::vector<int> >               (prefix_ + "iregion"         + suffix_);
    produces<std::vector<int> >               (prefix_ + "iring"           + suffix_);
    produces<std::vector<int> >               (prefix_ + "istation"        + suffix_);
    produces<std::vector<int> >               (prefix_ + "isector"         + suffix_);
    produces<std::vector<int> >               (prefix_ + "ilayer"          + suffix_);
    produces<std::vector<int> >               (prefix_ + "isubsector"      + suffix_);
    produces<std::vector<int> >               (prefix_ + "iroll"           + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "strip"           + suffix_);
    produces<std::vector<unsigned> >          (prefix_ + "layer"           + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "bx"              + suffix_);
    produces<unsigned>                        (prefix_ + "size"            + suffix_);
}

NtupleRPCTriggerPrimitives::~NtupleRPCTriggerPrimitives() {}

void NtupleRPCTriggerPrimitives::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    theGeometryTranslator_->checkAndUpdateGeometry(iSetup);
}

void NtupleRPCTriggerPrimitives::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<uint32_t> >          v_geoId           (new std::vector<uint32_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_subsystem       (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<int> >               v_iregion         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_iring           (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_istation        (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_isector         (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_ilayer          (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_isubsector      (new std::vector<int>());
    std::auto_ptr<std::vector<int> >               v_iroll           (new std::vector<int>());
    std::auto_ptr<std::vector<unsigned> >          v_strip           (new std::vector<unsigned>());
    std::auto_ptr<std::vector<unsigned> >          v_layer           (new std::vector<unsigned>());
    std::auto_ptr<std::vector<uint16_t> >          v_bx              (new std::vector<uint16_t>());
    std::auto_ptr<unsigned>                        v_size            (new unsigned(0));


    //__________________________________________________________________________

    edm::Handle<RPCDigiCollection> rpcDigis;
    //iEvent.getByLabel(inputTag_, rpcDigis);
    if (!token_.isUninitialized())
        iEvent.getByToken(token_, rpcDigis);

    if (rpcDigis.isValid()) {
        //edm::LogInfo("NtupleRPCTriggerPrimitives") << "Size: " << chambPhDigis->size();
        edm::LogInfo("NtupleRPCTriggerPrimitives") << "Size: ??";


        // Make trigger primitives

        std::vector<L1TMuon::TriggerPrimitive> trigPrims;

        // Loop over chambers
        for (RPCDigiCollection::DigiRangeIterator itr = rpcDigis->begin(); itr != rpcDigis->end(); ++itr) {

            // Loop over digis
            for (RPCDigiCollection::const_iterator it = (*itr).second.first; it != (*itr).second.second; ++it) {
                //if (!selector_(*it))
                //    continue;

                trigPrims.push_back(L1TMuon::TriggerPrimitive((*itr).first, (*it).strip(), (*itr).first.layer(), (*it).bx()));
            }
        }

        unsigned n = 0;

        // Loop over trigger primitives
        for (std::vector<L1TMuon::TriggerPrimitive>::const_iterator it = trigPrims.cbegin(); it != trigPrims.cend(); ++it) {
            if (n >= maxN_)
                break;

            if (it->subsystem() == L1TMuon::TriggerPrimitive::kRPC) {

                const RPCDetId rpcDet = it->detId<RPCDetId>();
                const L1TMuon::TriggerPrimitive::RPCData rpcData = it->getRPCData();

                v_geoId           ->push_back(it->rawId().rawId());
                v_subsystem       ->push_back(it->subsystem());
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

                n++;
            }
        }

        *v_size = v_geoId->size();

    } else {
        edm::LogError("NtupleRPCTriggerPrimitives") << "Cannot get the products: " << inputTag_;
    }


    //__________________________________________________________________________
    iEvent.put(v_geoId           , prefix_ + "geoId"           + suffix_);
    iEvent.put(v_subsystem       , prefix_ + "subsystem"       + suffix_);
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
    iEvent.put(v_size            , prefix_ + "size"            + suffix_);
}

#include "L1TMuonSimulations/NtupleTools/interface/NtupleDTTriggerPrimitives.h"

#include "DataFormats/MuonDetId/interface/DTChamberId.h"


NtupleDTTriggerPrimitives::NtupleDTTriggerPrimitives(const edm::ParameterSet& iConfig) :
  chambPhTag_(iConfig.getParameter<edm::InputTag>("chambPhTag")),
  chambThTag_(iConfig.getParameter<edm::InputTag>("chambThTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    theGeometryTranslator_.reset(new L1TMuon::GeometryTranslator());

    chambPhToken_ = consumes<L1MuDTChambPhContainer>(chambPhTag_);
    chambThToken_ = consumes<L1MuDTChambThContainer>(chambThTag_);

    produces<std::vector<uint32_t> >          (prefix_ + "geoId"           + suffix_);
    produces<std::vector<uint16_t> >          (prefix_ + "subsystem"       + suffix_);
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

void NtupleDTTriggerPrimitives::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<uint32_t> >          v_geoId           (new std::vector<uint32_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_subsystem       (new std::vector<uint16_t>());
    std::auto_ptr<std::vector<float> >             v_globalPhi       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalTheta     (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalEta       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalRho       (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalX         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalY         (new std::vector<float>());
    std::auto_ptr<std::vector<float> >             v_globalZ         (new std::vector<float>());
    std::auto_ptr<unsigned>                        v_size            (new unsigned(0));


    //__________________________________________________________________________

    edm::Handle<L1MuDTChambPhContainer> chambPhDigis;
    edm::Handle<L1MuDTChambThContainer> chambThDigis;
    //iEvent.getByLabel(chambPhTag_, chambPhDigis);
    //iEvent.getByLabel(chambThTag_, chambThDigis);
    if (!chambPhToken_.isUninitialized())
        iEvent.getByToken(chambPhToken_, chambPhDigis);
    if (!chambThToken_.isUninitialized())
        iEvent.getByToken(chambThToken_, chambThDigis);

    if (chambPhDigis.isValid() && chambThDigis.isValid()) {
        //edm::LogInfo("NtupleDTTriggerPrimitives") << "Size: " << chambPhDigis->size();
        edm::LogInfo("NtupleDTTriggerPrimitives") << "Size: ??";


        // Make trigger primitives

        std::vector<L1TMuon::TriggerPrimitive> trigPrims;

        // Loop over containers
        const L1MuDTChambPhContainer::Phi_Container * container = chambPhDigis->getContainer();
        for (L1MuDTChambPhContainer::Phi_Container::const_iterator it = container->begin(); it != container->end(); ++it) {
            // FIXME
        }

        unsigned n = 0;

        // Loop over trigger primitives
        for (std::vector<L1TMuon::TriggerPrimitive>::const_iterator it = trigPrims.cbegin(); it != trigPrims.cend(); ++it) {
            if (n >= maxN_)
                break;

            if (it->subsystem() == L1TMuon::TriggerPrimitive::kDT) {

                //const DTChamberId dtDet = it->detId<DTChamberId>();
                //const L1TMuon::TriggerPrimitive::DTData dtData = it->getDTData();

                // Global coordinates
                //const double globalPhi = theGeometryTranslator_->calculateGlobalPhi(*it);
                //const double globalEta = theGeometryTranslator_->calculateGlobalEta(*it);
                const GlobalPoint& gp = theGeometryTranslator_->getGlobalPoint(*it);

                // FIXME

                v_geoId           ->push_back(it->rawId().rawId());
                v_subsystem       ->push_back(it->subsystem());
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
        edm::LogError("NtupleDTTriggerPrimitives") << "Cannot get the products: " << chambPhTag_ << ", " << chambThTag_;
    }


    //__________________________________________________________________________
    iEvent.put(v_geoId           , prefix_ + "geoId"           + suffix_);
    iEvent.put(v_subsystem       , prefix_ + "subsystem"       + suffix_);
    iEvent.put(v_globalPhi       , prefix_ + "globalPhi"       + suffix_);
    iEvent.put(v_globalTheta     , prefix_ + "globalTheta"     + suffix_);
    iEvent.put(v_globalEta       , prefix_ + "globalEta"       + suffix_);
    iEvent.put(v_globalRho       , prefix_ + "globalRho"       + suffix_);
    iEvent.put(v_globalX         , prefix_ + "globalX"         + suffix_);
    iEvent.put(v_globalY         , prefix_ + "globalY"         + suffix_);
    iEvent.put(v_globalZ         , prefix_ + "globalZ"         + suffix_);
    iEvent.put(v_size            , prefix_ + "size"            + suffix_);
}

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
    produces<unsigned>                        (prefix_ + "size"            + suffix_);
}

NtupleDTTriggerPrimitives::~NtupleDTTriggerPrimitives() {}

void NtupleDTTriggerPrimitives::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    theGeometryTranslator_->checkAndUpdateGeometry(iSetup);
}

void NtupleDTTriggerPrimitives::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<uint32_t> >          v_geoId           (new std::vector<uint32_t>());
    std::auto_ptr<std::vector<uint16_t> >          v_subsystem       (new std::vector<uint16_t>());
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

        }

        unsigned n = 0;

        // Loop over trigger primitives
        for (std::vector<L1TMuon::TriggerPrimitive>::const_iterator it = trigPrims.cbegin(); it != trigPrims.cend(); ++it) {
            if (n >= maxN_)
                break;

            if (it->subsystem() == L1TMuon::TriggerPrimitive::kDT) {

                //const DTChamberId dtDet = it->detId<DTChamberId>();
                //const L1TMuon::TriggerPrimitive::DTData dtData = it->getDTData();

                v_geoId           ->push_back(it->rawId().rawId());
                v_subsystem       ->push_back(it->subsystem());

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
    iEvent.put(v_size            , prefix_ + "size"            + suffix_);
}

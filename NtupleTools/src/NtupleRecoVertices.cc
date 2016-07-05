#include "L1TMuonSimulations/NtupleTools/interface/NtupleRecoVertices.h"


NtupleRecoVertices::NtupleRecoVertices(const edm::ParameterSet& iConfig) :
  inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")),
  prefix_  (iConfig.getParameter<std::string>("prefix")),
  suffix_  (iConfig.getParameter<std::string>("suffix")),
  selector_(iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_    (iConfig.getParameter<unsigned>("maxN")) {

    token_ = consumes<reco::VertexCollection>(inputTag_);

    produces<std::vector<float> >    (prefix_ + "x"                 + suffix_);
    produces<std::vector<float> >    (prefix_ + "y"                 + suffix_);
    produces<std::vector<float> >    (prefix_ + "z"                 + suffix_);
    produces<std::vector<float> >    (prefix_ + "chi2"              + suffix_);
    produces<std::vector<float> >    (prefix_ + "ndof"              + suffix_);
    produces<std::vector<unsigned> > (prefix_ + "ntracks"           + suffix_);
    produces<unsigned>               (prefix_ + "size"              + suffix_);
}

NtupleRecoVertices::~NtupleRecoVertices() {}

void NtupleRecoVertices::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<float> >    v_x                (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_y                (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_z                (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_chi2             (new std::vector<float>());
    std::auto_ptr<std::vector<float> >    v_ndof             (new std::vector<float>());
    std::auto_ptr<std::vector<unsigned> > v_ntracks          (new std::vector<unsigned>());
    std::auto_ptr<unsigned>               v_size             (new unsigned(0));

    //__________________________________________________________________________
    edm::Handle<reco::VertexCollection> vertices;
    //iEvent.getByLabel(inputTag_, vertices);
    if (!token_.isUninitialized())
        iEvent.getByToken(token_, vertices);

    if (vertices.isValid()) {
        edm::LogInfo("NtupleRecoVertices") << "Size: " << vertices->size();

        unsigned n = 0;
        for (reco::VertexCollection::const_iterator it = vertices->begin(); it != vertices->end(); ++it) {
            if (n >= maxN_)
                break;
            if (!selector_(*it))
                continue;

            // Fill the vectors
            v_x                ->push_back(it->position().x());
            v_y                ->push_back(it->position().y());
            v_z                ->push_back(it->position().z());
            v_chi2             ->push_back(it->chi2());
            v_ndof             ->push_back(it->ndof());
            v_ntracks          ->push_back(it->tracksSize());

            n++;
        }
        *v_size = v_x->size();

    } else {
        edm::LogError("NtupleRecoVertices") << "Cannot get the product: " << inputTag_;
    }

    //__________________________________________________________________________
    iEvent.put(v_x                , prefix_ + "x"                 + suffix_);
    iEvent.put(v_y                , prefix_ + "y"                 + suffix_);
    iEvent.put(v_z                , prefix_ + "z"                 + suffix_);
    iEvent.put(v_chi2             , prefix_ + "chi2"              + suffix_);
    iEvent.put(v_ndof             , prefix_ + "ndof"              + suffix_);
    iEvent.put(v_ntracks          , prefix_ + "ntracks"           + suffix_);
    iEvent.put(v_size             , prefix_ + "size"              + suffix_);
}

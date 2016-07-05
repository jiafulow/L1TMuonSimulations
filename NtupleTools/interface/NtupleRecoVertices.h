#ifndef NtupleTools_NtupleRecoVertices_h_
#define NtupleTools_NtupleRecoVertices_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"


class NtupleRecoVertices : public edm::EDProducer {
  public:
    explicit NtupleRecoVertices(const edm::ParameterSet&);
    ~NtupleRecoVertices();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<reco::Vertex> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<reco::VertexCollection> token_;
};

#endif

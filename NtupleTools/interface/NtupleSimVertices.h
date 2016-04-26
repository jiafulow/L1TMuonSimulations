#ifndef NtupleTools_NtupleSimVertices_h_
#define NtupleTools_NtupleSimVertices_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"


class NtupleSimVertices : public edm::EDProducer {
  public:
    explicit NtupleSimVertices(const edm::ParameterSet&);
    ~NtupleSimVertices();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<SimVertex> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<edm::SimVertexContainer> token_;
};

#endif

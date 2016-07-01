#ifndef NtupleTools_NtupleRecoMuons_h_
#define NtupleTools_NtupleRecoMuons_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"


class NtupleRecoMuons : public edm::EDProducer {
  public:
    explicit NtupleRecoMuons(const edm::ParameterSet&);
    ~NtupleRecoMuons();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_, vertexTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<reco::Muon> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<reco::MuonCollection> token_;
    edm::EDGetTokenT<reco::VertexCollection> vertexToken_;
};

#endif

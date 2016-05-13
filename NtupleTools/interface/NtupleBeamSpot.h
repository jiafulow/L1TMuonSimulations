#ifndef NtupleTools_NtupleBeamSpot_h_
#define NtupleTools_NtupleBeamSpot_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"



class NtupleBeamSpot : public edm::EDProducer {
  public:
    explicit NtupleBeamSpot(const edm::ParameterSet&);
    ~NtupleBeamSpot();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<reco::BeamSpot> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<reco::BeamSpot> token_;
};

#endif

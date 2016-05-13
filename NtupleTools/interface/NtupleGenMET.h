#ifndef NtupleTools_NtupleGenMET_h_
#define NtupleTools_NtupleGenMET_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETFwd.h"


class NtupleGenMET : public edm::EDProducer {
  public:
    explicit NtupleGenMET(const edm::ParameterSet&);
    ~NtupleGenMET();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<reco::GenMET> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<reco::GenMETCollection> token_;
};

#endif

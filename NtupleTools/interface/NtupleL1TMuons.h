#ifndef NtupleTools_NtupleL1TMuons_h_
#define NtupleTools_NtupleL1TMuons_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/L1Trigger/interface/Muon.h"


class NtupleL1TMuons : public edm::EDProducer {
  public:
    explicit NtupleL1TMuons(const edm::ParameterSet&);
    ~NtupleL1TMuons();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();
    const edm::InputTag trackTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<l1t::Muon> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<l1t::MuonBxCollection> trackToken_;
};

#endif

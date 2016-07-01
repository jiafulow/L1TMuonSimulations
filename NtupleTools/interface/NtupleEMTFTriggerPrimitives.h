#ifndef NtupleTools_NtupleEMTFTriggerPrimitives_h_
#define NtupleTools_NtupleEMTFTriggerPrimitives_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/L1TMuon/interface/EMTFHit.h"
#include "DataFormats/L1TMuon/interface/EMTFHitExtra.h"


class NtupleEMTFTriggerPrimitives : public edm::EDProducer {
  public:
    explicit NtupleEMTFTriggerPrimitives(const edm::ParameterSet&);
    ~NtupleEMTFTriggerPrimitives();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    //virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    //virtual void endRun(const edm::Run&, const edm::EventSetup&);

    const edm::InputTag stubTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<l1t::EMTFHitExtra> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<l1t::EMTFHitExtraCollection> stubToken_;
};

#endif

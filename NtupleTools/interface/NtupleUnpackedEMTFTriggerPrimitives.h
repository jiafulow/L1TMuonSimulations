#ifndef NtupleTools_NtupleUnpackedEMTFTriggerPrimitives_h_
#define NtupleTools_NtupleUnpackedEMTFTriggerPrimitives_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/L1TMuon/interface/EMTFHit.h"
//#include "DataFormats/L1TMuon/interface/EMTFHitExtra.h"


class NtupleUnpackedEMTFTriggerPrimitives : public edm::EDProducer {
  public:
    explicit NtupleUnpackedEMTFTriggerPrimitives(const edm::ParameterSet&);
    ~NtupleUnpackedEMTFTriggerPrimitives();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    //virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    //virtual void endRun(const edm::Run&, const edm::EventSetup&);

    const edm::InputTag stubTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<l1t::EMTFHit> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<l1t::EMTFHitCollection> stubToken_;
};

#endif

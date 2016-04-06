#ifndef NtupleTools_NtupleCSCTriggerPrimitives_h_
#define NtupleTools_NtupleCSCTriggerPrimitives_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"


class NtupleCSCTriggerPrimitives : public edm::EDProducer {
  public:
    explicit NtupleCSCTriggerPrimitives(const edm::ParameterSet&);
    ~NtupleCSCTriggerPrimitives();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<CSCCorrelatedLCTDigi> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection> token_;
};

#endif

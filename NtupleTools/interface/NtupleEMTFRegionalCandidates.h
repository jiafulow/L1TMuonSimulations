#ifndef NtupleTools_NtupleEMTFRegionalCandidates_h_
#define NtupleTools_NtupleEMTFRegionalCandidates_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"

#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"

#include "L1Trigger/L1TMuon/interface/deprecate/MuonTriggerPrimitive.h"    // why deprecate?
#include "L1Trigger/L1TMuon/interface/deprecate/MuonTriggerPrimitiveFwd.h" // why deprecate?
//#include "L1Trigger/L1TMuon/interface/deprecate/GeometryTranslator.h"      // why deprecate?


class NtupleEMTFRegionalCandidates : public edm::EDProducer {
  public:
    explicit NtupleEMTFRegionalCandidates(const edm::ParameterSet&);
    ~NtupleEMTFRegionalCandidates();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();
    const edm::InputTag trackTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<l1t::RegionalMuonCand> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> trackToken_;
};

#endif

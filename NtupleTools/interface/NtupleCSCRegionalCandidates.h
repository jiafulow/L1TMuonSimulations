#ifndef NtupleTools_NtupleCSCRegionalCandidates_h_
#define NtupleTools_NtupleCSCRegionalCandidates_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"

#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCandFwd.h"

#include "L1Trigger/L1TMuon/interface/deprecate/MuonTriggerPrimitive.h"    // why deprecate?
#include "L1Trigger/L1TMuon/interface/deprecate/MuonTriggerPrimitiveFwd.h" // why deprecate?
//#include "L1Trigger/L1TMuon/interface/deprecate/GeometryTranslator.h"      // why deprecate?


class NtupleCSCRegionalCandidates : public edm::EDProducer {
  public:
    explicit NtupleCSCRegionalCandidates(const edm::ParameterSet&);
    ~NtupleCSCRegionalCandidates();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag corrlctTag_;
    const edm::InputTag trackTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<l1t::RegionalMuonCandBxCollection> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection>    corrlctToken_;
    edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> trackToken_;
};

#endif

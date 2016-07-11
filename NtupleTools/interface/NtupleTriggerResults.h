#ifndef NtupleTools_NtupleTriggerResults_h_
#define NtupleTools_NtupleTriggerResults_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "CondFormats/DataRecord/interface/L1TUtmTriggerMenuRcd.h"
#include "CondFormats/L1TObjects/interface/L1TUtmTriggerMenu.h"


class NtupleTriggerResults : public edm::EDProducer {
  public:
    explicit NtupleTriggerResults(const edm::ParameterSet&);
    ~NtupleTriggerResults();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    //virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    //virtual void endRun(const edm::Run&, const edm::EventSetup&);

    void syncHLTPaths(const edm::TriggerNames& triggerNames);
    void syncL1TPaths(const edm::EventSetup& iSetup);

    const edm::InputTag hltTag_;
    const edm::InputTag l1tTag_;
    const std::string   prefix_, suffix_;

    edm::ParameterSetID      hltPSetID_;
    std::vector<std::string> hltPaths_;
    std::vector<std::string> hltPathNames_;
    std::vector<unsigned>    hltPathIndices_;

    unsigned long long       l1tCacheID_;
    std::vector<std::string> l1tPaths_;
    std::vector<std::string> l1tPathNames_;
    std::vector<unsigned>    l1tPathIndices_;

    edm::EDGetTokenT<edm::TriggerResults> hltToken_;
    edm::EDGetTokenT<GlobalAlgBlkBxCollection> l1tToken_;
};

#endif


#ifndef NtupleTools_NtupleGenJets_h_
#define NtupleTools_NtupleGenJets_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"


class NtupleGenJets : public edm::EDProducer {
  public:
    explicit NtupleGenJets(const edm::ParameterSet&);
    ~NtupleGenJets();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<reco::GenJet> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<reco::GenJetCollection> token_;
};

#endif

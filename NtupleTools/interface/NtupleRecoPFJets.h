#ifndef NtupleTools_NtupleRecoPFJets_h_
#define NtupleTools_NtupleRecoPFJets_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"


class NtupleRecoPFJets : public edm::EDProducer {
  public:
    explicit NtupleRecoPFJets(const edm::ParameterSet&);
    ~NtupleRecoPFJets();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<reco::PFJet> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<reco::PFJetCollection> token_;
};

#endif

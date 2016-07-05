#ifndef NtupleTools_NtupleRecoPFMET_h_
#define NtupleTools_NtupleRecoPFMET_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"


class NtupleRecoPFMET : public edm::EDProducer {
  public:
    explicit NtupleRecoPFMET(const edm::ParameterSet&);
    ~NtupleRecoPFMET();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<reco::PFMET> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<reco::PFMETCollection> token_;
};

#endif

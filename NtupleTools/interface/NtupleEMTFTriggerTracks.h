#ifndef NtupleTools_NtupleEMTFTriggerTracks_h_
#define NtupleTools_NtupleEMTFTriggerTracks_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/L1TMuon/interface/EMTFHit.h"
#include "DataFormats/L1TMuon/interface/EMTFHitExtra.h"
#include "DataFormats/L1TMuon/interface/EMTFTrack.h"
#include "DataFormats/L1TMuon/interface/EMTFTrackExtra.h"


class NtupleEMTFTriggerTracks : public edm::EDProducer {
  public:
    explicit NtupleEMTFTriggerTracks(const edm::ParameterSet&);
    ~NtupleEMTFTriggerTracks();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    //virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    //virtual void endRun(const edm::Run&, const edm::EventSetup&);

    const edm::InputTag stubTag_, trackTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<l1t::EMTFTrackExtra> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<l1t::EMTFHitExtraCollection> stubToken_;
    edm::EDGetTokenT<l1t::EMTFTrackExtraCollection> trackToken_;
};

#endif

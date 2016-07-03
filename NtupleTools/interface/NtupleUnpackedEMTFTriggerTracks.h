#ifndef NtupleTools_NtupleUnpackedEMTFTriggerTracks_h_
#define NtupleTools_NtupleUnpackedEMTFTriggerTracks_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/L1TMuon/interface/EMTFHit.h"
//#include "DataFormats/L1TMuon/interface/EMTFHitExtra.h"
#include "DataFormats/L1TMuon/interface/EMTFTrack.h"
//#include "DataFormats/L1TMuon/interface/EMTFTrackExtra.h"


class NtupleUnpackedEMTFTriggerTracks : public edm::EDProducer {
  public:
    explicit NtupleUnpackedEMTFTriggerTracks(const edm::ParameterSet&);
    ~NtupleUnpackedEMTFTriggerTracks();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    //virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    //virtual void endRun(const edm::Run&, const edm::EventSetup&);

    const edm::InputTag stubTag_, trackTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<l1t::EMTFTrack> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<l1t::EMTFHitCollection> stubToken_;
    edm::EDGetTokenT<l1t::EMTFTrackCollection> trackToken_;
};

#endif

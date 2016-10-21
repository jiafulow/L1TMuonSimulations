#ifndef NtupleTools_NtupleEMUTFTriggerTracks_h_
#define NtupleTools_NtupleEMUTFTriggerTracks_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormatsSep2016/L1TMuon/interface/EMTFHit.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFHitExtra.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFTrack.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFTrackExtra.h"


class NtupleEMUTFTriggerTracks : public edm::EDProducer {
  public:
    explicit NtupleEMUTFTriggerTracks(const edm::ParameterSet&);
    ~NtupleEMUTFTriggerTracks();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    //virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    //virtual void endRun(const edm::Run&, const edm::EventSetup&);

    const edm::InputTag unpStubTag_, unpTrackTag_;
    const std::string   unpPrefix_, unpSuffix_;
    StringCutObjectSelector<L1TMuonEndCap::EMTFTrack> unpSelector_;

    const edm::InputTag emuStubTag_, emuTrackTag_;
    const std::string   emuPrefix_, emuSuffix_;
    StringCutObjectSelector<L1TMuonEndCap::EMTFTrackExtra> emuSelector_;

    const unsigned maxN_;

    edm::EDGetTokenT<L1TMuonEndCap::EMTFHitCollection> unpStubToken_;
    edm::EDGetTokenT<L1TMuonEndCap::EMTFTrackCollection> unpTrackToken_;
    edm::EDGetTokenT<L1TMuonEndCap::EMTFHitExtraCollection> emuStubToken_;
    edm::EDGetTokenT<L1TMuonEndCap::EMTFTrackExtraCollection> emuTrackToken_;
};

#endif

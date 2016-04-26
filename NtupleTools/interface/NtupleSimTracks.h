#ifndef NtupleTools_NtupleSimTracks_h_
#define NtupleTools_NtupleSimTracks_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"


class NtupleSimTracks : public edm::EDProducer {
  public:
    explicit NtupleSimTracks(const edm::ParameterSet&);
    ~NtupleSimTracks();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<SimTrack> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<edm::SimTrackContainer> token_;
};

#endif

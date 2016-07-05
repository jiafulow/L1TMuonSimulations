#ifndef NtupleTools_NtupleRecoTracks_h_
#define NtupleTools_NtupleRecoTracks_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"


class NtupleRecoTracks : public edm::EDProducer {
  public:
    explicit NtupleRecoTracks(const edm::ParameterSet&);
    ~NtupleRecoTracks();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    const edm::InputTag inputTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<reco::Track> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<reco::TrackCollection> token_;
};

#endif

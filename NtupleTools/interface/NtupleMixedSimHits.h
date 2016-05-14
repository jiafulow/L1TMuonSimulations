#ifndef NtupleTools_NtupleMixedSimHits_h_
#define NtupleTools_NtupleMixedSimHits_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCLayerGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"


class NtupleMixedSimHits : public edm::EDProducer {
  public:
    explicit NtupleMixedSimHits(const edm::ParameterSet&);
    ~NtupleMixedSimHits();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    //virtual void endRun(const edm::Run&, const edm::EventSetup&);

    // Event setup
    const CSCGeometry * theCSCGeometry_;

    const edm::InputTag         inputTag_, trkPartTag_;
    edm::ParameterSet           simHitCollectionConfig_;
    std::vector<edm::InputTag>  simHitTags_;
    const std::string           prefix_, suffix_;

    StringCutObjectSelector<PSimHit> selector_;
    const unsigned maxN_;

    //edm::EDGetTokenT<CrossingFrame<PSimHit> > token_;
    edm::EDGetTokenT<TrackingParticleCollection> trkPartToken_;
    std::vector<edm::EDGetTokenT<CrossingFrame<PSimHit> > > simHitTokens_;
};

#endif

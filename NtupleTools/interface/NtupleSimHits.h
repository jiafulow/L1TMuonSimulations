#ifndef NtupleTools_NtupleSimHits_h_
#define NtupleTools_NtupleSimHits_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCLayerGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"


class NtupleSimHits : public edm::EDProducer {
  public:
    explicit NtupleSimHits(const edm::ParameterSet&);
    ~NtupleSimHits();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    //virtual void endRun(const edm::Run&, const edm::EventSetup&);

    // Event setup
    const CSCGeometry * theCSCGeometry_;

    const edm::InputTag         inputTag_;
    edm::ParameterSet           simHitCollectionConfig_;
    std::vector<edm::InputTag>  simHitTags_;
    const std::string           prefix_, suffix_;

    StringCutObjectSelector<PSimHit> selector_;
    const unsigned maxN_;

    //edm::EDGetTokenT<edm::PSimHitContainer> token_;
    std::vector<edm::EDGetTokenT<edm::PSimHitContainer> > simHitTokens_;
};

#endif

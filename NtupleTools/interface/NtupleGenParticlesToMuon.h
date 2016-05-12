#ifndef NtupleTools_NtupleGenParticlesToMuon_h_
#define NtupleTools_NtupleGenParticlesToMuon_h_

#include "L1TMuonSimulations/NtupleTools/interface/NtupleCommon.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "RecoMuon/DetLayers/interface/MuonDetLayerGeometry.h"
#include "DataFormats/GeometrySurface/interface/BoundCylinder.h"
#include "DataFormats/GeometrySurface/interface/BoundDisk.h"


class NtupleGenParticlesToMuon : public edm::EDProducer {
  public:
    explicit NtupleGenParticlesToMuon(const edm::ParameterSet&);
    ~NtupleGenParticlesToMuon();

  private:
    //virtual void beginJob();
    virtual void produce(edm::Event&, const edm::EventSetup&);
    //virtual void endJob();

    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    //virtual void endRun(const edm::Run&, const edm::EventSetup&);

    // Event setup
    const MagneticField * theMagneticField_;
    const Propagator * thePropagator_;
    const MuonDetLayerGeometry * theMuonGeometry_;
    std::vector<const BoundCylinder *> theDTLayers_;
    std::vector<const BoundDisk *> theForwardCSCLayers_;
    std::vector<const BoundDisk *> theBackwardCSCLayers_;

    const edm::InputTag inputTag_;
    const edm::InputTag beamSpotTag_;
    const std::string   prefix_, suffix_;

    StringCutObjectSelector<reco::GenParticle> selector_;
    const unsigned maxN_;

    edm::EDGetTokenT<reco::GenParticleCollection> token_;
    edm::EDGetTokenT<reco::BeamSpot> beamSpotToken_;
};

#endif

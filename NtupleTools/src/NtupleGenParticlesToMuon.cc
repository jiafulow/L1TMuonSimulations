#include "L1TMuonSimulations/NtupleTools/interface/NtupleGenParticlesToMuon.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "RecoMuon/Records/interface/MuonRecoGeometryRecord.h"
#include "TrackingTools/DetLayers/interface/DetLayer.h"

#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/TrajectoryState/interface/TrackCharge.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/PatternTools/interface/TSCBLBuilderNoMaterial.h"
#include "TrackingTools/PatternTools/interface/TSCPBuilderNoMaterial.h"


namespace {
// Copied from DataFormats/TrackReco/interface/TrackBase.h
double get_dxy(const GlobalPoint& v, const GlobalVector& p) {
    return (-v.x()*p.y() + v.y()*p.x()) / p.perp();
    //return (-vx() * py() + vy() * px()) / pt();
}

double get_d0(const GlobalPoint& v, const GlobalVector& p) {
    return -1.0 * get_dxy(v, p);
}

//double get_dsz(const GlobalPoint& v, const GlobalVector& p) {
//    return v.z() * p.perp() / p.mag() - (v.x()*p.x() + v.y()*p.y()) / p.perp() * (p.z() / p.mag());
//    //return vz() * pt() / p() - (vx() * px() + vy() * py()) / pt() * pz() / p();
//}

double get_dz(const GlobalPoint& v, const GlobalVector& p) {
    return v.z() - (v.x()*p.x() + v.y()*p.y()) / p.perp() * (p.z() / p.perp());
    //return vz() - (vx() * px() + vy() * py()) / pt() * (pz() / pt());
}
}

// _____________________________________________________________________________
NtupleGenParticlesToMuon::NtupleGenParticlesToMuon(const edm::ParameterSet& iConfig) :
  inputTag_   (iConfig.getParameter<edm::InputTag>("inputTag")),
  beamSpotTag_(iConfig.getParameter<edm::InputTag>("beamSpotTag")),
  prefix_     (iConfig.getParameter<std::string>("prefix")),
  suffix_     (iConfig.getParameter<std::string>("suffix")),
  selector_   (iConfig.existsAs<std::string>("cut") ? iConfig.getParameter<std::string>("cut") : "", true),
  maxN_       (iConfig.getParameter<unsigned>("maxN")) {

    token_ = consumes<reco::GenParticleCollection>(inputTag_);
    beamSpotToken_ = consumes<reco::BeamSpot>(beamSpotTag_);

    produces<std::vector<float> >               (prefix_ + "invPt"    + suffix_);
    produces<std::vector<float> >               (prefix_ + "cotTheta" + suffix_);
    produces<std::vector<float> >               (prefix_ + "d0"       + suffix_);
    produces<std::vector<float> >               (prefix_ + "dz"       + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalPhiMB"   + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalThetaMB" + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalEtaMB"   + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalRhoMB"   + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalPhiME"   + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalThetaME" + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalEtaME"   + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalRhoME"   + suffix_);
    //produces<unsigned>                          (prefix_ + "size"     + suffix_);
}

NtupleGenParticlesToMuon::~NtupleGenParticlesToMuon() {}

void NtupleGenParticlesToMuon::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    /// Magnetic field setup
    edm::ESHandle<MagneticField> magneticFieldHandle;
    iSetup.get<IdealMagneticFieldRecord>().get(magneticFieldHandle);
    if (!magneticFieldHandle.isValid()) {
        edm::LogWarning("NtupleGenParticlesToMuon") << "Unable to get IdealMagneticFieldRecord!";
    } else {
        theMagneticField_ = magneticFieldHandle.product();
    }

    // Propagator setup
    edm::ESHandle<Propagator> propagatorHandle;
    iSetup.get<TrackingComponentsRecord>().get("SteppingHelixPropagatorAlong", propagatorHandle);
    if (!propagatorHandle.isValid()) {
        edm::LogWarning("NtupleGenParticlesToMuon") << "Unable to get SteppingHelixPropagatorAlong!";
    } else {
        thePropagator_ = propagatorHandle.product();
    }

    // Muon geometry setup
    edm::ESHandle<MuonDetLayerGeometry> muonGeometryHandle;
    iSetup.get<MuonRecoGeometryRecord>().get(muonGeometryHandle);
    if (!muonGeometryHandle.isValid()) {
        edm::LogWarning("NtupleGenParticlesToMuon") << "Unable to get MuonRecoGeometryRecord!";
    } else {
        theMuonGeometry_ = muonGeometryHandle.product();

        const std::vector<const DetLayer*>& dtLayers = theMuonGeometry_->allDTLayers();
        for (unsigned i=0; i<dtLayers.size(); i++) {
            const BoundCylinder * surface = dynamic_cast<const BoundCylinder *>(&dtLayers.at(i)->surface());
            theDTLayers_.push_back(surface);
            //std::cout << "DT " << i << " radius: " << surface->radius() << " half length: " << surface->bounds().length()/2.0 << std::endl;
        }

        const std::vector<const DetLayer*>& forwardCSCLayers = theMuonGeometry_->forwardCSCLayers();  // positive
        for (unsigned i=0; i<forwardCSCLayers.size(); i++) {
            const BoundDisk * surface = dynamic_cast<const BoundDisk *>(&forwardCSCLayers.at(i)->surface());
            theForwardCSCLayers_.push_back(surface);
            //std::cout << "Forward CSC " << i << " innerRadius: " << surface->innerRadius() << " outerRadius: " << surface->outerRadius() << " z: " << surface->position().z() << std::endl;
        }

        const std::vector<const DetLayer*>& backwardCSCLayers = theMuonGeometry_->backwardCSCLayers();  // negative
        for (unsigned i=0; i<backwardCSCLayers.size(); i++) {
            const BoundDisk * surface = dynamic_cast<const BoundDisk *>(&backwardCSCLayers.at(i)->surface());
            theBackwardCSCLayers_.push_back(surface);
            //std::cout << "Backward CSC " << i << " innerRadius: " << surface->innerRadius() << " outerRadius: " << surface->outerRadius() << " z: " << surface->position().z() << std::endl;
        }

        assert(theDTLayers_.size() == 4);
        assert(theForwardCSCLayers_.size() == 5);
        assert(theBackwardCSCLayers_.size() == 5);
    }
}

void NtupleGenParticlesToMuon::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

    std::auto_ptr<std::vector<float> >               v_invPt        (new std::vector<float>());
    std::auto_ptr<std::vector<float> >               v_cotTheta     (new std::vector<float>());
    std::auto_ptr<std::vector<float> >               v_d0           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >               v_dz           (new std::vector<float>());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalPhiMB  (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalThetaMB(new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalEtaMB  (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalRhoMB  (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalPhiME  (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalThetaME(new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalEtaME  (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalRhoME  (new std::vector<std::vector<float> >());
    //std::auto_ptr<unsigned>            v_size     (new unsigned(0));

    //__________________________________________________________________________
    if (!iEvent.isRealData()) {
        edm::Handle<reco::GenParticleCollection> parts;
        //iEvent.getByLabel(inputTag_, parts);
        if (!token_.isUninitialized())
            iEvent.getByToken(token_, parts);

        edm::Handle<reco::BeamSpot> beamSpot;
        //iEvent.getByLabel(inputTagBeamSpot_, beamSpot);
        if (!beamSpotToken_.isUninitialized())
            iEvent.getByToken(beamSpotToken_, beamSpot);


        if (parts.isValid()) {
            edm::LogInfo("NtupleGenParticlesToMuon") << "Size: " << parts->size();

            unsigned n = 0;
            for (reco::GenParticleCollection::const_iterator it = parts->begin(); it != parts->end(); ++it) {
                if (n >= maxN_)
                    break;
                if (!selector_(*it))
                    continue;

                // Beam spot
                GlobalPoint bsPosition(0., 0., 0.);
                if (beamSpot.isValid()) {
                    bsPosition = GlobalPoint(beamSpot->x0(), beamSpot->y0(), beamSpot->z0());
                }

                // Free trajectory state
                const GlobalPoint vertex(it->vx(), it->vy(), it->vz());
                const GlobalVector momentum(it->px(), it->py(), it->pz());
                const TrackCharge charge(it->charge());
                FreeTrajectoryState ftsAtProduction(vertex, momentum, charge, theMagneticField_);
                //std::cout << "part " << it - parts->begin() << " pt " << it->pt() << " eta: " << it->eta() << " phi: " << it->phi() << std::endl;

                // Point of closest approach
                // see SimTracker/TrackAssociation/src/ParametersDefinerForTP.cc
                GlobalPoint poca(0., 0., 0.);
                GlobalVector poca_mom(0., 0., 0.);

                TSCBLBuilderNoMaterial tscblBuilder;
                TrajectoryStateClosestToBeamLine tsAtClosestApproach = tscblBuilder(ftsAtProduction, *beamSpot);

                if (tsAtClosestApproach.isValid()) {
                    //poca = tsAtClosestApproach.trackStateAtPCA().position() - bsPosition;  // not supported!?
                    poca = tsAtClosestApproach.trackStateAtPCA().position();
                    poca = GlobalPoint(poca.x() - bsPosition.x(), poca.y() - bsPosition.y(), poca.z() - bsPosition.z());
                    poca_mom = tsAtClosestApproach.trackStateAtPCA().momentum();
                }

                // Propagator
                // see MuonAnalysis/MuonAssociators/src/PropagateToMuon.cc
                const Propagator * propagatorBarrel = &*thePropagator_;
                const Propagator * propagatorEndcap = &*thePropagator_;
                const std::vector<const BoundCylinder *>& layersBarrel = theDTLayers_;
                const std::vector<const BoundDisk *>& layersEndcap = (it->eta() > 0) ? theForwardCSCLayers_ : theBackwardCSCLayers_;

                TrajectoryStateOnSurface tsosBarrel;
                bool isValidBarrel = true;
                std::vector<GlobalPoint> extrapolatedBarrel;

                for (unsigned i=0; i<layersBarrel.size(); i++) {
                    if (i == 0) {
                        tsosBarrel = propagatorBarrel->propagate(ftsAtProduction, *layersBarrel.at(i));
                    } else if (isValidBarrel) {  // if the previous tsos is valid
                        tsosBarrel = propagatorBarrel->propagate(tsosBarrel, *layersBarrel.at(i));
                    }

                    isValidBarrel = tsosBarrel.isValid() &&
                       fabs(tsosBarrel.globalPosition().z()) <= layersBarrel.at(i)->bounds().length()/2.0;
                    if (isValidBarrel) {
                        extrapolatedBarrel.push_back(tsosBarrel.globalPosition());
                    } else {
                        extrapolatedBarrel.push_back(GlobalPoint(0., 0., 0.));
                    }
                    //std::cout << "part " << it - parts->begin() << " barrel " << i << " isValidBarrel: " << isValidBarrel << " z: " << extrapolatedBarrel.back().z() << std::endl;
                }

                TrajectoryStateOnSurface tsosEndcap, tsosEndcap_1;
                bool isValidEndcap = true, isValidEndcap_1 = true;
                std::vector<GlobalPoint> extrapolatedEndcap;

                for (unsigned i=0; i<layersEndcap.size(); i++) {
                    if (i == 0) {
                        tsosEndcap = propagatorEndcap->propagate(ftsAtProduction, *layersEndcap.at(i));
                    } else if (i == 1) {
                        tsosEndcap_1 = tsosEndcap;
                        isValidEndcap_1 = isValidEndcap;
                        if (isValidEndcap) {  // if the previous tsos is valid
                            tsosEndcap = propagatorEndcap->propagate(tsosEndcap, *layersEndcap.at(i));
                        } else {
                            tsosEndcap = propagatorEndcap->propagate(ftsAtProduction, *layersEndcap.at(i));
                        }
                    } else if (isValidEndcap) {  // if the previous tsos is valid
                        tsosEndcap = propagatorEndcap->propagate(tsosEndcap, *layersEndcap.at(i));
                    }

                    isValidEndcap = tsosEndcap.isValid() &&
                       tsosEndcap.globalPosition().perp() >= layersEndcap.at(i)->innerRadius() &&
                       tsosEndcap.globalPosition().perp() <= layersEndcap.at(i)->outerRadius();
                    if (isValidEndcap) {
                        extrapolatedEndcap.push_back(tsosEndcap.globalPosition());
                    } else {
                        extrapolatedEndcap.push_back(GlobalPoint(0., 0., 0.));
                    }
                    //std::cout << "part " << it - parts->begin() << " endcap " << i << " isValidEndcap: " << isValidEndcap << " r: " << extrapolatedEndcap.back().perp() << std::endl;

                    if (i == 1 && !isValidEndcap) {  // if the second tsos is invalid, switch back to the first tsos
                        tsosEndcap = tsosEndcap_1;
                        isValidEndcap = isValidEndcap_1;
                    }
                }

                // Fill the intermediate vectors
                std::vector<float> globalPhiMB, globalThetaMB, globalEtaMB, globalRhoMB;
                for (unsigned i=0; i<extrapolatedBarrel.size(); i++) {
                    const GlobalPoint& gp = extrapolatedBarrel.at(i);
                    globalPhiMB.push_back(gp.phi());
                    globalThetaMB.push_back(gp.theta());
                    globalEtaMB.push_back(gp.eta());
                    globalRhoMB.push_back(gp.perp());
                }

                std::vector<float> globalPhiME, globalThetaME, globalEtaME, globalRhoME;
                for (unsigned i=0; i<extrapolatedEndcap.size(); i++) {
                    const GlobalPoint& gp = extrapolatedEndcap.at(i);
                    globalPhiME.push_back(gp.phi());
                    globalThetaME.push_back(gp.theta());
                    globalEtaME.push_back(gp.eta());
                    globalRhoME.push_back(gp.perp());
                }

                // Fill the vectors
                v_invPt        ->push_back(float(it->charge()) / it->pt());
                v_cotTheta     ->push_back(it->theta() != 0. ? 1.0 / std::tan(it->theta()) : 1.0 / 1e-16);
                v_d0           ->push_back(get_d0(poca, poca_mom));
                v_dz           ->push_back(get_dz(poca, poca_mom));
                v_globalPhiMB  ->push_back(globalPhiMB);
                v_globalThetaMB->push_back(globalThetaMB);
                v_globalEtaMB  ->push_back(globalEtaMB);
                v_globalRhoMB  ->push_back(globalRhoMB);
                v_globalPhiME  ->push_back(globalPhiME);
                v_globalThetaME->push_back(globalThetaME);
                v_globalEtaME  ->push_back(globalEtaME);
                v_globalRhoME  ->push_back(globalRhoME);

                n++;
            }

        } else {
            edm::LogError("NtupleGenParticlesToMuon") << "Cannot get the product: " << inputTag_;
        }
    }

    //__________________________________________________________________________
    iEvent.put(v_invPt        , prefix_ + "invPt"         + suffix_);
    iEvent.put(v_cotTheta     , prefix_ + "cotTheta"      + suffix_);
    iEvent.put(v_d0           , prefix_ + "d0"            + suffix_);
    iEvent.put(v_dz           , prefix_ + "dz"            + suffix_);
    iEvent.put(v_globalPhiMB  , prefix_ + "globalPhiMB"   + suffix_);
    iEvent.put(v_globalThetaMB, prefix_ + "globalThetaMB" + suffix_);
    iEvent.put(v_globalEtaMB  , prefix_ + "globalEtaMB"   + suffix_);
    iEvent.put(v_globalRhoMB  , prefix_ + "globalRhoMB"   + suffix_);
    iEvent.put(v_globalPhiME  , prefix_ + "globalPhiME"   + suffix_);
    iEvent.put(v_globalThetaME, prefix_ + "globalThetaME" + suffix_);
    iEvent.put(v_globalEtaME  , prefix_ + "globalEtaME"   + suffix_);
    iEvent.put(v_globalRhoME  , prefix_ + "globalRhoME"   + suffix_);
}

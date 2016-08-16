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

    theGeometryTraverser_.reset(new GeometryTraverser());

    produces<std::vector<float> >               (prefix_ + "d0"            + suffix_);
    produces<std::vector<float> >               (prefix_ + "dz"            + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalPhiMB"   + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalThetaMB" + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalEtaMB"   + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalRhoMB"   + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalPxMB"    + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalPyMB"    + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalPzMB"    + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalPhiME"   + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalThetaME" + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalEtaME"   + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalRhoME"   + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalPxME"    + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalPyME"    + suffix_);
    produces<std::vector<std::vector<float> > > (prefix_ + "globalPzME"    + suffix_);
    //produces<unsigned>                          (prefix_ + "size"     + suffix_);
}

NtupleGenParticlesToMuon::~NtupleGenParticlesToMuon() {}

void NtupleGenParticlesToMuon::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    theGeometryTraverser_->checkAndUpdateGeometry(iSetup);

    // Muon geometry
    {
        const std::vector<const DetLayer*>& dtLayers = theGeometryTraverser_->muonGeometry()->allDTLayers();
        for (unsigned i=0; i<dtLayers.size(); i++) {
            const BoundCylinder * surface = dynamic_cast<const BoundCylinder *>(&dtLayers.at(i)->surface());
            theDTLayers_.push_back(surface);
            //std::cout << "DT " << i << " radius: " << surface->radius() << " half length: " << surface->bounds().length()/2.0 << std::endl;
        }

        const std::vector<const DetLayer*>& forwardCSCLayers = theGeometryTraverser_->muonGeometry()->forwardCSCLayers();  // positive
        for (unsigned i=0; i<forwardCSCLayers.size(); i++) {
            const BoundDisk * surface = dynamic_cast<const BoundDisk *>(&forwardCSCLayers.at(i)->surface());
            theForwardCSCLayers_.push_back(surface);
            //std::cout << "Forward CSC " << i << " innerRadius: " << surface->innerRadius() << " outerRadius: " << surface->outerRadius() << " z: " << surface->position().z() << std::endl;
        }

        const std::vector<const DetLayer*>& backwardCSCLayers = theGeometryTraverser_->muonGeometry()->backwardCSCLayers();  // negative
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

    std::auto_ptr<std::vector<float> >               v_d0           (new std::vector<float>());
    std::auto_ptr<std::vector<float> >               v_dz           (new std::vector<float>());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalPhiMB  (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalThetaMB(new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalEtaMB  (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalRhoMB  (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalPxMB   (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalPyMB   (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalPzMB   (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalPhiME  (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalThetaME(new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalEtaME  (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalRhoME  (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalPxME   (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalPyME   (new std::vector<std::vector<float> >());
    std::auto_ptr<std::vector<std::vector<float> > > v_globalPzME   (new std::vector<std::vector<float> >());
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
                FreeTrajectoryState ftsAtProduction(vertex, momentum, charge, theGeometryTraverser_->magneticField());
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
                const std::vector<const BoundCylinder *>& layersBarrel = theDTLayers_;
                const std::vector<const BoundDisk *>& layersEndcap = (it->eta() > 0) ? theForwardCSCLayers_ : theBackwardCSCLayers_;
                theGeometryTraverser_->setFTS(vertex, momentum, charge);

                std::vector<double> vec_r;
                for (unsigned i=0; i<layersBarrel.size(); i++) {
                    vec_r.push_back(layersBarrel.at(i)->radius());
                }
                const std::vector<std::pair<GlobalPoint, GlobalVector> >& propagatedBarrel = theGeometryTraverser_->propagateBarrels(vec_r);

                std::vector<double> vec_z;
                for (unsigned i=0; i<layersEndcap.size(); i++) {
                    vec_z.push_back(layersEndcap.at(i)->position().z());
                }
                const std::vector<std::pair<GlobalPoint, GlobalVector> >& propagatedEndcap = theGeometryTraverser_->propagateEndcaps(vec_z);

                // Fill the intermediate vectors
                std::vector<float> globalPhiMB, globalThetaMB, globalEtaMB, globalRhoMB, globalPxMB, globalPyMB, globalPzMB;
                for (unsigned i=0; i<propagatedBarrel.size(); i++) {
                    const GlobalPoint& gp = propagatedBarrel.at(i).first;
                    const GlobalVector& gv = propagatedBarrel.at(i).second;
                    globalPhiMB.push_back(gp.phi());
                    globalThetaMB.push_back(gp.theta());
                    globalEtaMB.push_back(gp.eta());
                    globalRhoMB.push_back(gp.perp());
                    globalPxMB.push_back(gv.x());
                    globalPyMB.push_back(gv.y());
                    globalPzMB.push_back(gv.z());
                }

                std::vector<float> globalPhiME, globalThetaME, globalEtaME, globalRhoME, globalPxME, globalPyME, globalPzME;
                for (unsigned i=0; i<propagatedEndcap.size(); i++) {
                    const GlobalPoint& gp = propagatedEndcap.at(i).first;
                    const GlobalVector& gv = propagatedEndcap.at(i).second;
                    globalPhiME.push_back(gp.phi());
                    globalThetaME.push_back(gp.theta());
                    globalEtaME.push_back(gp.eta());
                    globalRhoME.push_back(gp.perp());
                    globalPxME.push_back(gv.x());
                    globalPyME.push_back(gv.y());
                    globalPzME.push_back(gv.z());
                }

                // Fill the vectors
                v_d0           ->push_back(get_d0(poca, poca_mom));
                v_dz           ->push_back(get_dz(poca, poca_mom));
                v_globalPhiMB  ->push_back(globalPhiMB);
                v_globalThetaMB->push_back(globalThetaMB);
                v_globalEtaMB  ->push_back(globalEtaMB);
                v_globalRhoMB  ->push_back(globalRhoMB);
                v_globalPxMB   ->push_back(globalPxMB);
                v_globalPyMB   ->push_back(globalPyMB);
                v_globalPzMB   ->push_back(globalPzMB);
                v_globalPhiME  ->push_back(globalPhiME);
                v_globalThetaME->push_back(globalThetaME);
                v_globalEtaME  ->push_back(globalEtaME);
                v_globalRhoME  ->push_back(globalRhoME);
                v_globalPxME   ->push_back(globalPxME);
                v_globalPyME   ->push_back(globalPyME);
                v_globalPzME   ->push_back(globalPzME);

                n++;
            }

        } else {
            edm::LogError("NtupleGenParticlesToMuon") << "Cannot get the product: " << inputTag_;
        }
    }

    //__________________________________________________________________________
    iEvent.put(v_d0           , prefix_ + "d0"            + suffix_);
    iEvent.put(v_dz           , prefix_ + "dz"            + suffix_);
    iEvent.put(v_globalPhiMB  , prefix_ + "globalPhiMB"   + suffix_);
    iEvent.put(v_globalThetaMB, prefix_ + "globalThetaMB" + suffix_);
    iEvent.put(v_globalEtaMB  , prefix_ + "globalEtaMB"   + suffix_);
    iEvent.put(v_globalRhoMB  , prefix_ + "globalRhoMB"   + suffix_);
    iEvent.put(v_globalPxMB   , prefix_ + "globalPxMB"    + suffix_);
    iEvent.put(v_globalPyMB   , prefix_ + "globalPyMB"    + suffix_);
    iEvent.put(v_globalPzMB   , prefix_ + "globalPzMB"    + suffix_);
    iEvent.put(v_globalPhiME  , prefix_ + "globalPhiME"   + suffix_);
    iEvent.put(v_globalThetaME, prefix_ + "globalThetaME" + suffix_);
    iEvent.put(v_globalEtaME  , prefix_ + "globalEtaME"   + suffix_);
    iEvent.put(v_globalRhoME  , prefix_ + "globalRhoME"   + suffix_);
    iEvent.put(v_globalPxME   , prefix_ + "globalPxME"    + suffix_);
    iEvent.put(v_globalPyME   , prefix_ + "globalPyME"    + suffix_);
    iEvent.put(v_globalPzME   , prefix_ + "globalPzME"    + suffix_);
}

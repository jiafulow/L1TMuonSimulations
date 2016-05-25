#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "L1TMuonSimulations/MuonTools/interface/GeometryTraverser.h"

#include "RecoMuon/DetLayers/interface/MuonDetLayerGeometry.h"
#include "TrackingTools/DetLayers/interface/DetLayer.h"
#include "DataFormats/GeometrySurface/interface/BoundCylinder.h"
#include "DataFormats/GeometrySurface/interface/BoundDisk.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/TrajectoryState/interface/TrackCharge.h"

#include <iostream>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"


struct MyPropagator {
    // A simple container of the propagator
    float pt;
    float eta;
    float phi;
    std::vector<float> hits_x;
    std::vector<float> hits_y;
    std::vector<float> hits_z;
    std::vector<float> hits_phi;
    std::vector<float> hits_theta;
    std::vector<float> hits_eta;
    std::vector<float> hits_rho;
};

class AnalyzerPropagateToMuon : public edm::EDAnalyzer {
  public:
    explicit AnalyzerPropagateToMuon(const edm::ParameterSet&);
    ~AnalyzerPropagateToMuon();

  private:
    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    virtual void endRun(const edm::Run&, const edm::EventSetup&);

    virtual void beginJob();
    virtual void endJob();

    virtual void analyze(const edm::Event&, const edm::EventSetup&);

    /// Event setup
    std::unique_ptr<GeometryTraverser> theGeometryTraverser_;
    std::vector<const BoundCylinder *> theDTLayers_;
    std::vector<const BoundDisk *> theForwardCSCLayers_;
    std::vector<const BoundDisk *> theBackwardCSCLayers_;

    /// Output file
    TFile * tfile_;
    TTree * ttree_;
    std::unique_ptr<MyPropagator> mypropagator_;

    /// Configurations
    std::vector<double> ptValues_;
    std::vector<double> etaValues_;
    std::vector<double> phiValues_;
    std::string outname_;
    int verbose_;
};

AnalyzerPropagateToMuon::AnalyzerPropagateToMuon(const edm::ParameterSet& iConfig)
: ptValues_(iConfig.getParameter<std::vector<double> >("ptValues") ),
  etaValues_(iConfig.getParameter<std::vector<double> >("etaValues") ),
  phiValues_(iConfig.getParameter<std::vector<double> >("phiValues") ),
  outname_(iConfig.getParameter<std::string>("out") ),
  verbose_(iConfig.getParameter<int>("verbosity") ) {

    theGeometryTraverser_.reset(new GeometryTraverser());

    mypropagator_.reset(new MyPropagator());
}

AnalyzerPropagateToMuon::~AnalyzerPropagateToMuon() {}

void AnalyzerPropagateToMuon::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    theGeometryTraverser_->checkAndUpdateGeometry(iSetup);

    // Muon geometry
    {
        std::cout << "DT layers" << std::endl;
        const std::vector<const DetLayer*>& dtLayers = theGeometryTraverser_->muonGeometry()->allDTLayers();
        for (unsigned i=0; i<dtLayers.size(); i++) {
            const BoundCylinder * surface = dynamic_cast<const BoundCylinder *>(&dtLayers.at(i)->surface());
            theDTLayers_.push_back(surface);
            //std::cout << "DT " << i << " radius: " << surface->radius() << " half length: " << surface->bounds().length()/2.0 << std::endl;
            std::cout << "Position: " << surface->position().x() << " " << surface->position().y() << " " << surface->position().z() << " Radius: " << surface->radius() << " Bound: " << static_cast<const SimpleCylinderBounds&>(surface->bounds()).length() << " " << static_cast<const SimpleCylinderBounds&>(surface->bounds()).width() << " " << static_cast<const SimpleCylinderBounds&>(surface->bounds()).thickness() << std::endl;
        }

        std::cout << "Forward CSC layers" << std::endl;
        const std::vector<const DetLayer*>& forwardCSCLayers = theGeometryTraverser_->muonGeometry()->forwardCSCLayers();  // positive
        for (unsigned i=0; i<forwardCSCLayers.size(); i++) {
            const BoundDisk * surface = dynamic_cast<const BoundDisk *>(&forwardCSCLayers.at(i)->surface());
            theForwardCSCLayers_.push_back(surface);
            //std::cout << "Forward CSC " << i << " innerRadius: " << surface->innerRadius() << " outerRadius: " << surface->outerRadius() << " z: " << surface->position().z() << std::endl;
            std::cout << "Position: " << surface->position().x() << " " << surface->position().y() << " " << surface->position().z() << " Bound: " << static_cast<const SimpleDiskBounds&>(surface->bounds()).innerRadius() << " " << static_cast<const SimpleDiskBounds&>(surface->bounds()).outerRadius() << " " << static_cast<const SimpleDiskBounds&>(surface->bounds()).minZ() << " " << static_cast<const SimpleDiskBounds&>(surface->bounds()).maxZ() << std::endl;
        }

        std::cout << "Backward CSC layers" << std::endl;
        const std::vector<const DetLayer*>& backwardCSCLayers = theGeometryTraverser_->muonGeometry()->backwardCSCLayers();  // negative
        for (unsigned i=0; i<backwardCSCLayers.size(); i++) {
            const BoundDisk * surface = dynamic_cast<const BoundDisk *>(&backwardCSCLayers.at(i)->surface());
            theBackwardCSCLayers_.push_back(surface);
            //std::cout << "Backward CSC " << i << " innerRadius: " << surface->innerRadius() << " outerRadius: " << surface->outerRadius() << " z: " << surface->position().z() << std::endl;
            std::cout << "Position: " << surface->position().x() << " " << surface->position().y() << " " << surface->position().z() << " Bound: " << static_cast<const SimpleDiskBounds&>(surface->bounds()).innerRadius() << " " << static_cast<const SimpleDiskBounds&>(surface->bounds()).outerRadius() << " " << static_cast<const SimpleDiskBounds&>(surface->bounds()).minZ() << " " << static_cast<const SimpleDiskBounds&>(surface->bounds()).maxZ() << std::endl;
        }

        assert(theDTLayers_.size() == 4);
        assert(theForwardCSCLayers_.size() == 5);
        assert(theBackwardCSCLayers_.size() == 5);
    }

    // _________________________________________________________________________
    for (unsigned i=0; i<phiValues_.size(); ++i) {
        for (unsigned j=0; j<etaValues_.size(); ++j) {
            for (unsigned k=0; k<ptValues_.size(); ++k) {


                TLorentzVector p4(0., 0., 0., 0.);
                p4.SetPtEtaPhiM(ptValues_.at(k), etaValues_.at(j), phiValues_.at(i), 0.105658);

                // Free trajectory state
                const GlobalPoint vertex(0., 0., 0.);
                const GlobalVector momentum(p4.Px(), p4.Py(), p4.Pz());
                const TrackCharge charge(-1);
                //FreeTrajectoryState ftsAtProduction(vertex, momentum, charge, theMagneticField_);
                //std::cout << "part " << it - parts->begin() << " pt " << it->pt() << " eta: " << it->eta() << " phi: " << it->phi() << std::endl;
                theGeometryTraverser_->setFTS(vertex, momentum, charge);


                // Propagate
                //std::vector<double> vec_r;
                //for (unsigned i=2; i<770; i+=2) {
                //    vec_r.push_back(double(i));
                //}
                //const std::vector<GlobalPoint>& propagatedBarrel = theGeometryTraverser_->propagateBarrels(vec_r);

                std::vector<double> vec_z;
                for (unsigned i=2; i<1100; i+=2) {
                    vec_z.push_back(double(i));
                }
                const std::vector<GlobalPoint>& propagatedEndcap = theGeometryTraverser_->propagateEndcaps(vec_z);


                // Fill the vectors
                mypropagator_->pt         = momentum.perp();
                mypropagator_->eta        = momentum.eta();
                mypropagator_->phi        = momentum.phi();
                mypropagator_->hits_x     = std::vector<float>();
                mypropagator_->hits_y     = std::vector<float>();
                mypropagator_->hits_z     = std::vector<float>();
                mypropagator_->hits_phi   = std::vector<float>();
                mypropagator_->hits_theta = std::vector<float>();
                mypropagator_->hits_eta   = std::vector<float>();
                mypropagator_->hits_rho   = std::vector<float>();

                for (unsigned i=0; i<propagatedEndcap.size(); i++) {
                    const GlobalPoint& gp = propagatedEndcap.at(i);
                    mypropagator_->hits_x.push_back(gp.x());
                    mypropagator_->hits_y.push_back(gp.y());
                    mypropagator_->hits_z.push_back(gp.z());
                    mypropagator_->hits_phi.push_back(gp.phi());
                    mypropagator_->hits_theta.push_back(gp.theta());
                    mypropagator_->hits_eta.push_back(gp.eta());
                    mypropagator_->hits_rho.push_back(gp.perp());
                }

                ttree_->Fill();

            }  // END loop over pt
        }  // END loop over eta
    }  // END loop over phi
}

void AnalyzerPropagateToMuon::endRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {}

void AnalyzerPropagateToMuon::beginJob() {
    tfile_ = TFile::Open(outname_.c_str(), "RECREATE");
    ttree_ = new TTree("tree", "propagate_to_muon");

    ttree_->Branch("pt"        , &(mypropagator_->pt));
    ttree_->Branch("eta"       , &(mypropagator_->eta));
    ttree_->Branch("phi"       , &(mypropagator_->phi));
    ttree_->Branch("hits_x"    , &(mypropagator_->hits_x));
    ttree_->Branch("hits_y"    , &(mypropagator_->hits_y));
    ttree_->Branch("hits_z"    , &(mypropagator_->hits_z));
    ttree_->Branch("hits_phi"  , &(mypropagator_->hits_phi));
    ttree_->Branch("hits_theta", &(mypropagator_->hits_theta));
    ttree_->Branch("hits_eta"  , &(mypropagator_->hits_eta));
    ttree_->Branch("hits_rho"  , &(mypropagator_->hits_rho));
}

void AnalyzerPropagateToMuon::endJob() {
    tfile_->Write();
    //tfile_->Close();
}

// ANALYZE
void AnalyzerPropagateToMuon::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    // Do nothing
}

// DEFINE THIS AS A PLUG-IN
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(AnalyzerPropagateToMuon);

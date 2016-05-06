#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"

#include <iostream>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TH2F.h"


struct MyField {
    // A simple container of the magnetic field strength
    float z;
    float r;
    float Bz;
    float Br;
};

class AnalyzerMagneticField : public edm::EDAnalyzer {
  public:
    explicit AnalyzerMagneticField(const edm::ParameterSet&);
    ~AnalyzerMagneticField();

  private:
    virtual void beginRun(const edm::Run&, const edm::EventSetup&);
    virtual void endRun(const edm::Run&, const edm::EventSetup&);

    virtual void beginJob();
    virtual void endJob();

    virtual void analyze(const edm::Event&, const edm::EventSetup&);

    /// Event setup
    const MagneticField * theMagneticField_;

    /// Output file
    TFile * tfile_;
    TTree * ttree_;
    TH2F * h2_bz_;
    TH2F * h2_br_;
    MyField * myfield_;

    /// Configurations
    std::string outname_;
    int verbose_;
};

AnalyzerMagneticField::AnalyzerMagneticField(const edm::ParameterSet& iConfig)
: outname_(iConfig.getParameter<std::string>("out") ),
  verbose_(iConfig.getParameter<int>("verbosity") ) {

    myfield_ = new MyField();
}

AnalyzerMagneticField::~AnalyzerMagneticField() {
    delete myfield_;
}

void AnalyzerMagneticField::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {
    /// Magnetic field setup
    edm::ESHandle<MagneticField> magneticFieldHandle;
    iSetup.get<IdealMagneticFieldRecord>().get(magneticFieldHandle);
    if (!magneticFieldHandle.isValid()) {
        edm::LogWarning("AnalyzerMagneticField") << "Unable to get IdealMagneticFieldRecord!";
    } else {
        theMagneticField_ = magneticFieldHandle.product();
    }

    //const double diameter = 14.6 * 100;
    //const double length = 21.6 * 100;
    const double diameter = 16 * 100;
    const double length = 24 * 100;
    double rmin = 0, rmax = diameter/2.0;
    double zmin = 0, zmax = length/2.0;

    int nr = 1000;
    int nz = 1000;

    h2_bz_->SetBins(nz, zmin, zmax, nr, rmin, rmax);
    h2_br_->SetBins(nz, zmin, zmax, nr, rmin, rmax);

    for (int iz=0; iz<nz; ++iz) {
        double z = zmin + (zmax-zmin)/double(nz)*(0.5+double(iz));

        for (int ir=0; ir<nr; ++ir) {
            double r = rmin + (rmax-rmin)/double(nr)*(0.5+double(ir));

            const GlobalPoint gp(r, 0, z);  // phi = 0
            const GlobalVector& gv = theMagneticField_->inTesla(gp);

            // Fill
            myfield_->z = gp.z();
            myfield_->r = gp.perp();
            myfield_->Bz = gv.z();
            myfield_->Br = gv.perp();
            ttree_->Fill();

            h2_bz_->SetBinContent(iz, ir, gv.z());
            h2_br_->SetBinContent(iz, ir, gv.perp());
        }
    }
}

void AnalyzerMagneticField::endRun(const edm::Run& iRun, const edm::EventSetup& iSetup) {}

void AnalyzerMagneticField::beginJob() {
    tfile_ = TFile::Open(outname_.c_str(), "RECREATE");
    ttree_ = new TTree("tree", "magnetic_field");
    ttree_->Branch("z" , &(myfield_->z));
    ttree_->Branch("r" , &(myfield_->r));
    ttree_->Branch("Bz", &(myfield_->Bz));
    ttree_->Branch("Br", &(myfield_->Br));

    h2_bz_ = new TH2F("bz", "; z [cm]; r [cm]; B_{z} [T]", 100, 0, 1, 100, 0, 1);
    h2_br_ = new TH2F("br", "; z [cm]; r [cm]; B_{r} [T]", 100, 0, 1, 100, 0, 1);
}

void AnalyzerMagneticField::endJob() {
    h2_bz_->Write();
    h2_br_->Write();

    tfile_->Write();
    //tfile_->Close();
}

// ANALYZE
void AnalyzerMagneticField::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    // Do nothing
}

// DEFINE THIS AS A PLUG-IN
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(AnalyzerMagneticField);

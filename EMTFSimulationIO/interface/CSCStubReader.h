#ifndef EMTFSimulationIO_CSCStubReader_h_
#define EMTFSimulationIO_CSCStubReader_h_

#include "TChain.h"
#include "TFile.h"
#include "TFileCollection.h"
#include "TROOT.h"
#include "TString.h"
#include "TTree.h"
#include "TBranchElement.h"
#include <memory>
#include <vector>


namespace phasetwoemtf {

// _____________________________________________________________________________
class CSCStubReader {
public:
    CSCStubReader(int verbose=1);
    ~CSCStubReader();

    void init(TString src, TString prefix="", TString suffix="");

    Long64_t loadTree(Long64_t entry) { return tchain_->LoadTree(entry); }

    Int_t getEntry(Long64_t entry) { return tchain_->GetEntry(entry); }

    Long64_t getEntries() const { return tchain_->GetEntries(); }

    TChain* getChain() { return tchain_; }

    // genParticle information
    std::vector<float> *          vp_pt;
    std::vector<float> *          vp_eta;
    std::vector<float> *          vp_phi;
    std::vector<float> *          vp_vx;
    std::vector<float> *          vp_vy;
    std::vector<float> *          vp_vz;
    std::vector<int16_t> *        vp_charge;
    std::vector<int> *            vp_pdgId;
    std::vector<int> *            vp_status;

    // genParticle propagator information
    std::vector<std::vector<float> > * vp_globalPhiME;
    std::vector<std::vector<float> > * vp_globalThetaME;
    std::vector<std::vector<float> > * vp_globalEtaME;
    std::vector<std::vector<float> > * vp_globalRhoME;

    // Stub information
    std::vector<float> *          vb_globalPhi;
    std::vector<float> *          vb_globalTheta;
    std::vector<float> *          vb_globalEta;
    std::vector<float> *          vb_globalRho;
    std::vector<float> *          vb_globalX;
    std::vector<float> *          vb_globalY;
    std::vector<float> *          vb_globalZ;
    std::vector<uint32_t> *       vb_geoId;
    std::vector<uint32_t> *       vb_moduleId;
    std::vector<int16_t> *        vb_isector;
    std::vector<int16_t> *        vb_isubsector;
    std::vector<uint16_t> *       vb_keywire;
    std::vector<uint16_t> *       vb_strip;
    std::vector<uint16_t> *       vb_pattern;
    std::vector<uint16_t> *       vb_cscID;

protected:
    TChain* tchain_;
    int treenumber_;
    const int verbose_;
};


// _____________________________________________________________________________
class CSCStubWriter {
public:
    CSCStubWriter(int verbose=1);
    ~CSCStubWriter();

    void init(TTree* intree, TString out, TString prefix="", TString suffix="");

    void resetDeleteBranches (TTree* tree);

    void fill() { ttree_->Fill(); }

    Long64_t writeTree();

protected:
    TFile* tfile_;
    TTree* ttree_;
    const int verbose_;
};

}  // namespace phasetwoemtf

#endif

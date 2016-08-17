#include "L1TMuonSimulations/EMTFSimulationIO/interface/CSCStubReader.h"

#include <iostream>
#include <cassert>
#include <stdexcept>


namespace phasetwoemtf {

// _____________________________________________________________________________
CSCStubReader::CSCStubReader(int verbose)
: vp_pt               (0),
  vp_eta              (0),
  vp_phi              (0),
  vp_vx               (0),
  vp_vy               (0),
  vp_vz               (0),
  vp_charge           (0),
  vp_pdgId            (0),
  vp_status           (0),
  //
  vp_globalPhiME      (0),
  vp_globalThetaME    (0),
  vp_globalEtaME      (0),
  vp_globalRhoME      (0),
  vp_globalPxME       (0),
  vp_globalPyME       (0),
  vp_globalPzME       (0),
  //
  vb_globalPhi        (0),
  vb_globalTheta      (0),
  vb_globalEta        (0),
  vb_globalRho        (0),
  vb_globalX          (0),
  vb_globalY          (0),
  vb_globalZ          (0),
  vb_geoId            (0),
  vb_moduleId         (0),
  vb_isector          (0),
  vb_isubsector       (0),
  vb_keywire          (0),
  vb_strip            (0),
  vb_pattern          (0),
  vb_cscID            (0),
  //
  verbose_(verbose) {}

CSCStubReader::~CSCStubReader() {
    if (tchain_)  delete tchain_;
}

void CSCStubReader::init(TString src, TString prefix, TString suffix) {
    if (!src.EndsWith(".root") && !src.EndsWith(".txt")) {
        TString msg = "Input source must be either .root or .txt";
        throw std::invalid_argument(msg.Data());
    }

    //if (verbose_)  std::cout << "Opening " << src << std::endl;
    tchain_ = new TChain("ntupler/tree");

    if (src.EndsWith(".root")) {
        if (tchain_->Add(src) ) {
            //if (verbose_)  std::cout << "Successfully read " << src << std::endl;
        } else {
            TString msg = "Failed to read " + src;
            throw std::invalid_argument(msg.Data());
        }

    } else if (src.EndsWith(".txt")) {
        TFileCollection fc("fileinfolist", "", src);
        if (tchain_->AddFileInfoList((TCollection*) fc.GetList()) ) {
            //if (verbose_)  std::cout << "Successfully read " << src << std::endl;
        } else {
            TString msg = "Failed to read " + src;
            throw std::invalid_argument(msg.Data());
        }
    }

    assert(tchain_ != 0);
    treenumber_ = tchain_->GetTreeNumber();

    tchain_->SetBranchAddress("genParts_pt"           , &(vp_pt));
    tchain_->SetBranchAddress("genParts_eta"          , &(vp_eta));
    tchain_->SetBranchAddress("genParts_phi"          , &(vp_phi));
    tchain_->SetBranchAddress("genParts_vx"           , &(vp_vx));
    tchain_->SetBranchAddress("genParts_vy"           , &(vp_vy));
    tchain_->SetBranchAddress("genParts_vz"           , &(vp_vz));
    tchain_->SetBranchAddress("genParts_charge"       , &(vp_charge));
    tchain_->SetBranchAddress("genParts_pdgId"        , &(vp_pdgId));
    tchain_->SetBranchAddress("genParts_status"       , &(vp_status));
    //
    tchain_->SetBranchAddress("genParts_globalPhiME"  , &(vp_globalPhiME));
    tchain_->SetBranchAddress("genParts_globalThetaME", &(vp_globalThetaME));
    tchain_->SetBranchAddress("genParts_globalEtaME"  , &(vp_globalEtaME));
    tchain_->SetBranchAddress("genParts_globalRhoME"  , &(vp_globalRhoME));
    tchain_->SetBranchAddress("genParts_globalPxME"   , &(vp_globalPxME));
    tchain_->SetBranchAddress("genParts_globalPyME"   , &(vp_globalPyME));
    tchain_->SetBranchAddress("genParts_globalPzME"   , &(vp_globalPzME));
    //
    tchain_->SetBranchAddress("CSCStubs_globalPhi"    , &(vb_globalPhi));
    tchain_->SetBranchAddress("CSCStubs_globalTheta"  , &(vb_globalTheta));
    tchain_->SetBranchAddress("CSCStubs_globalEta"    , &(vb_globalEta));
    tchain_->SetBranchAddress("CSCStubs_globalRho"    , &(vb_globalRho));
    tchain_->SetBranchAddress("CSCStubs_globalX"      , &(vb_globalX));
    tchain_->SetBranchAddress("CSCStubs_globalY"      , &(vb_globalY));
    tchain_->SetBranchAddress("CSCStubs_globalZ"      , &(vb_globalZ));
    tchain_->SetBranchAddress("CSCStubs_geoId"        , &(vb_geoId));
    tchain_->SetBranchAddress("CSCStubs_moduleId"     , &(vb_moduleId));
    tchain_->SetBranchAddress("CSCStubs_isector"      , &(vb_isector));
    tchain_->SetBranchAddress("CSCStubs_isubsector"   , &(vb_isubsector));
    tchain_->SetBranchAddress("CSCStubs_keywire"      , &(vb_keywire));
    tchain_->SetBranchAddress("CSCStubs_strip"        , &(vb_strip));
    tchain_->SetBranchAddress("CSCStubs_pattern"      , &(vb_pattern));
    tchain_->SetBranchAddress("CSCStubs_cscID"        , &(vb_cscID));

    tchain_->SetBranchStatus("*"                      , 0);
    tchain_->SetBranchStatus("genParts_pt"            , 1);
    tchain_->SetBranchStatus("genParts_eta"           , 1);
    tchain_->SetBranchStatus("genParts_phi"           , 1);
    tchain_->SetBranchStatus("genParts_vx"            , 1);
    tchain_->SetBranchStatus("genParts_vy"            , 1);
    tchain_->SetBranchStatus("genParts_vz"            , 1);
    tchain_->SetBranchStatus("genParts_charge"        , 1);
    tchain_->SetBranchStatus("genParts_pdgId"         , 1);
    tchain_->SetBranchStatus("genParts_status"        , 1);
    //
    tchain_->SetBranchStatus("genParts_globalPhiME"   , 1);
    tchain_->SetBranchStatus("genParts_globalThetaME" , 1);
    tchain_->SetBranchStatus("genParts_globalEtaME"   , 1);
    tchain_->SetBranchStatus("genParts_globalRhoME"   , 1);
    tchain_->SetBranchStatus("genParts_globalPxME"    , 1);
    tchain_->SetBranchStatus("genParts_globalPyME"    , 1);
    tchain_->SetBranchStatus("genParts_globalPzME"    , 1);
    //
    tchain_->SetBranchStatus("CSCStubs_globalPhi"     , 1);
    tchain_->SetBranchStatus("CSCStubs_globalTheta"   , 1);
    tchain_->SetBranchStatus("CSCStubs_globalEta"     , 1);
    tchain_->SetBranchStatus("CSCStubs_globalRho"     , 1);
    tchain_->SetBranchStatus("CSCStubs_globalX"       , 1);
    tchain_->SetBranchStatus("CSCStubs_globalY"       , 1);
    tchain_->SetBranchStatus("CSCStubs_globalZ"       , 1);
    tchain_->SetBranchStatus("CSCStubs_geoId"         , 1);
    tchain_->SetBranchStatus("CSCStubs_moduleId"      , 1);
    tchain_->SetBranchStatus("CSCStubs_isector"       , 1);
    tchain_->SetBranchStatus("CSCStubs_isubsector"    , 1);
    tchain_->SetBranchStatus("CSCStubs_keywire"       , 1);
    tchain_->SetBranchStatus("CSCStubs_strip"         , 1);
    tchain_->SetBranchStatus("CSCStubs_pattern"       , 1);
    tchain_->SetBranchStatus("CSCStubs_cscID"         , 1);
}


// _____________________________________________________________________________
CSCStubWriter::CSCStubWriter(int verbose)
: verbose_(verbose) {}

CSCStubWriter::~CSCStubWriter() {
    if (ttree_)  delete ttree_;
    if (tfile_)  delete tfile_;
}

void CSCStubWriter::init(TTree* intree, TString out, TString prefix, TString suffix) {
    gROOT->ProcessLine("#include <vector>");  // how is it not loaded?

    if (!out.EndsWith(".root")) {
        TString msg = "Output filename must be .root";
        throw std::invalid_argument(msg.Data());
    }

    //if (verbose_)  std::cout << "Opening " << out << std::endl;
    tfile_ = TFile::Open(out, "RECREATE");

    if (tfile_) {
        //if (verbose_)  std::cout << "Successfully opened " << out << std::endl;
    } else {
        TString msg = "Failed to open " + out;
        throw std::invalid_argument(msg.Data());
    }

    tfile_->mkdir("ntupler")->cd();
    ttree_ = (TTree*) intree->CloneTree(0); // Do not copy the data yet
    // The clone should not delete any shared i/o buffers.
    resetDeleteBranches(ttree_);
}

void CSCStubWriter::resetDeleteBranches (TTree* tree) {
    TObjArray* branches = tree->GetListOfBranches();
    Int_t nb = branches->GetEntriesFast();
    for (Int_t i = 0; i < nb; ++i) {
        TBranch* br = (TBranch*) branches->UncheckedAt(i);
        if (br->InheritsFrom(TBranchElement::Class())) {
            ((TBranchElement*) br)->ResetDeleteObject();
        }
    }
}

Long64_t CSCStubWriter::writeTree() {
    Long64_t nentries = ttree_->GetEntries();
    tfile_->Write();
    //tfile_->Close();
    return nentries;
}

}  // namespace phasetwoemtf

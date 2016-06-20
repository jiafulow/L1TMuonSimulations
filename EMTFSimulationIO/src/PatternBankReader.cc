#include "L1TMuonSimulations/EMTFSimulationIO/interface/PatternBankReader.h"

#include <iostream>
#include <cassert>
#include <stdexcept>


// _____________________________________________________________________________
PatternBankReader::PatternBankReader(int verbose)
: pb_superstripIds  (0),
  pb_popularity     (0),
  pb_invPt_mean     (0.),
  pb_invPt_sigma    (0.),
  pb_cotTheta_mean  (0.),
  pb_cotTheta_sigma (0.),
  pb_phi_mean       (0.),
  pb_phi_sigma      (0.),
  pb_z0_mean        (0.),
  pb_z0_sigma       (0.),
  //
  pb_coverage       (0.),
  pb_count          (0),
  pb_tower          (0),
  pb_sector         (0),
  pb_superstrip     (0),
  pb_superstrip_nx  (0),
  pb_superstrip_nz  (0),
  //
  verbose_(verbose) {}

PatternBankReader::~PatternBankReader() {
    if (ttree2_) delete ttree2_;
    if (ttree_)  delete ttree_;
    if (tfile_)  delete tfile_;
}

void PatternBankReader::init(TString src) {
    if (!src.EndsWith(".root")) {
        TString msg = "Input source must be .root";
        throw std::invalid_argument(msg.Data());
    }

    //if (verbose_)  std::cout << "Opening " << src << std::endl;
    tfile_ = TFile::Open(src);

    if (tfile_) {
        //if (verbose_)  std::cout << "Successfully read " << src << std::endl;
    } else {
        TString msg = "Failed to read " + src;
        throw std::invalid_argument(msg.Data());
    }

    ttree2_ = (TTree*) tfile_->Get("patternBankInfo");
    assert(ttree2_ != 0);

    ttree2_->SetBranchAddress("coverage"     , &pb_coverage);
    ttree2_->SetBranchAddress("count"        , &pb_count);
    ttree2_->SetBranchAddress("tower"        , &pb_tower);
    ttree2_->SetBranchAddress("sector"       , &pb_sector);
    ttree2_->SetBranchAddress("superstrip"   , &pb_superstrip);
    ttree2_->SetBranchAddress("superstrip_nx", &pb_superstrip_nx);
    ttree2_->SetBranchAddress("superstrip_nz", &pb_superstrip_nz);

    ttree_ = (TTree*) tfile_->Get("patternBank");
    assert(ttree_ != 0);

    ttree_->SetBranchAddress("popularity"    , &pb_popularity);
    ttree_->SetBranchAddress("superstripIds" , &pb_superstripIds);
    ttree_->SetBranchAddress("invPt_mean"    , &pb_invPt_mean);
    ttree_->SetBranchAddress("invPt_sigma"   , &pb_invPt_sigma);
    ttree_->SetBranchAddress("cotTheta_mean" , &pb_cotTheta_mean);
    ttree_->SetBranchAddress("cotTheta_sigma", &pb_cotTheta_sigma);
    ttree_->SetBranchAddress("phi_mean"      , &pb_phi_mean);
    ttree_->SetBranchAddress("phi_sigma"     , &pb_phi_sigma);
    ttree_->SetBranchAddress("z0_mean"       , &pb_z0_mean);
    ttree_->SetBranchAddress("z0_sigma"      , &pb_z0_sigma);
}


// _____________________________________________________________________________
PatternBankWriter::PatternBankWriter(int verbose)
: pb_superstripIds  (new std::vector<superstrip_type>()),
  pb_popularity     (new ULong64_t(0)),
  pb_invPt_mean     (new float(0.)),
  pb_invPt_sigma    (new float(0.)),
  pb_cotTheta_mean  (new float(0.)),
  pb_cotTheta_sigma (new float(0.)),
  pb_phi_mean       (new float(0.)),
  pb_phi_sigma      (new float(0.)),
  pb_z0_mean        (new float(0.)),
  pb_z0_sigma       (new float(0.)),
  //
  pb_coverage       (new float(0.)),
  pb_count          (new unsigned(0)),
  pb_tower          (new unsigned(0)),
  pb_sector         (new unsigned(0)),
  pb_superstrip     (new std::string("")),
  pb_superstrip_nx  (new unsigned(0)),
  pb_superstrip_nz  (new unsigned(0)),
  //
  verbose_(verbose) {}

PatternBankWriter::~PatternBankWriter() {
    if (ttree2_) delete ttree2_;
    if (ttree_)  delete ttree_;
    if (tfile_)  delete tfile_;
}

void PatternBankWriter::init(TString out) {
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

    // Pattern bank info
    ttree2_ = new TTree("patternBankInfo", "");
    ttree2_->Branch("coverage"      , &(*pb_coverage));
    ttree2_->Branch("count"         , &(*pb_count));
    ttree2_->Branch("tower"         , &(*pb_tower));
    ttree2_->Branch("sector"        , &(*pb_sector));
    ttree2_->Branch("superstrip"    , &(*pb_superstrip));
    ttree2_->Branch("superstrip_nx" , &(*pb_superstrip_nx));
    ttree2_->Branch("superstrip_nz" , &(*pb_superstrip_nz));

    // Pattern bank
    ttree_ = new TTree("patternBank", "");
    ttree_->Branch("popularity"     , &(*pb_popularity));
    ttree_->Branch("superstripIds"  , &(*pb_superstripIds));
    ttree_->Branch("invPt_mean"     , &(*pb_invPt_mean));
    ttree_->Branch("invPt_sigma"    , &(*pb_invPt_sigma));
    ttree_->Branch("cotTheta_mean"  , &(*pb_cotTheta_mean));
    ttree_->Branch("cotTheta_sigma" , &(*pb_cotTheta_sigma));
    ttree_->Branch("phi_mean"       , &(*pb_phi_mean));
    ttree_->Branch("phi_sigma"      , &(*pb_phi_sigma));
    ttree_->Branch("z0_mean"        , &(*pb_z0_mean));
    ttree_->Branch("z0_sigma"       , &(*pb_z0_sigma));
}

void PatternBankWriter::fillPatternBankInfo() {
    ttree2_->Fill();
    assert(ttree2_->GetEntries() == 1);
}

void PatternBankWriter::fillPatternBank() {
    ttree_->Fill();
}

Long64_t PatternBankWriter::writeTree() {
    Long64_t nentries = ttree_->GetEntries();
    tfile_->Write();
    //tfile_->Close();
    return nentries;
}

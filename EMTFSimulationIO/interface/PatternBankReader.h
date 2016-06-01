#ifndef EMTFSimulationIO_PatternBankReader_h_
#define EMTFSimulationIO_PatternBankReader_h_

#include "L1TMuonSimulations/EMTFSimulationDataFormats/interface/Pattern.h"

#include "TFile.h"
#include "TROOT.h"
#include "TString.h"
#include "TTree.h"
#include <memory>
#include <string>
#include <vector>


// _____________________________________________________________________________
class PatternBankReader {
  public:
    PatternBankReader(int verbose=1);
    ~PatternBankReader();

    void init(TString src);

    void getPatternBankInfo(float& coverage, unsigned& count, unsigned& tower, unsigned& sector, std::string& superstrip,
                            unsigned& superstrip_nx, unsigned& superstrip_nz);

    Int_t getEntry(Long64_t entry) { return ttree_->GetEntry(entry); }

    Long64_t getEntries() const { return ttree_->GetEntries(); }

    TTree * getTree() { return ttree_; }

    // Pattern bank
    std::vector<superstrip_type> * pb_superstripIds;
    ULong64_t                      pb_popularity;
    float                          pb_invPt_mean;
    float                          pb_invPt_sigma;
    float                          pb_cotTheta_mean;
    float                          pb_cotTheta_sigma;
    float                          pb_phi_mean;
    float                          pb_phi_sigma;
    float                          pb_z0_mean;
    float                          pb_z0_sigma;

    // Pattern bank info
    float                          pb_coverage;
    unsigned                       pb_count;
    unsigned                       pb_tower;
    unsigned                       pb_sector;
    std::string *                  pb_superstrip;
    unsigned                       pb_superstrip_nx;
    unsigned                       pb_superstrip_nz;

  protected:
    TFile* tfile_;
    TTree* ttree_;   // for pattern bank
    TTree* ttree2_;  // for pattern bank info
    const int verbose_;
};


// _____________________________________________________________________________
class PatternBankWriter {
  public:
    PatternBankWriter(int verbose=1);
    ~PatternBankWriter();

    void init(TString out);

    void fillPatternBankInfo();  // must be called before fillPatternBank()

    void fillPatternBank();

    Long64_t writeTree();

    // Pattern bank
    std::unique_ptr<std::vector<superstrip_type> > pb_superstripIds;
    std::unique_ptr<ULong64_t>                     pb_popularity;
    std::unique_ptr<float>                         pb_invPt_mean;
    std::unique_ptr<float>                         pb_invPt_sigma;
    std::unique_ptr<float>                         pb_cotTheta_mean;
    std::unique_ptr<float>                         pb_cotTheta_sigma;
    std::unique_ptr<float>                         pb_phi_mean;
    std::unique_ptr<float>                         pb_phi_sigma;
    std::unique_ptr<float>                         pb_z0_mean;
    std::unique_ptr<float>                         pb_z0_sigma;

    // Pattern bank statistics
    std::unique_ptr<float>                         pb_coverage;
    std::unique_ptr<unsigned>                      pb_count;
    std::unique_ptr<unsigned>                      pb_tower;
    std::unique_ptr<unsigned>                      pb_sector;
    std::unique_ptr<std::string>                   pb_superstrip;
    std::unique_ptr<unsigned>                      pb_superstrip_nx;
    std::unique_ptr<unsigned>                      pb_superstrip_nz;


  protected:
    TFile* tfile_;
    TTree* ttree_;   // for pattern bank
    TTree* ttree2_;  // for pattern bank info
    const int verbose_;
};

#endif

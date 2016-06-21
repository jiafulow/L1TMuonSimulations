#ifndef EMTFSimulation_PatternGenerator_h_
#define EMTFSimulation_PatternGenerator_h_

#include "L1TMuonSimulations/EMTFSimulation/interface/Helper.h"
#include "L1TMuonSimulations/EMTFSimulation/interface/ProgramOption.h"

#include "L1TMuonSimulations/EMTFSimulation/interface/PatternBankContainer.h"
#include "L1TMuonSimulations/EMTFSimulation/interface/SuperstripArbiter.h"


class PatternGenerator {
  public:
    PatternGenerator(const ProgramOption& po);
    ~PatternGenerator();

    void init();
    void run();

  private:
    // Make pattern bank
    void makePatterns(TString src);

    // Write pattern bank
    void writePatterns(TString out);

    // Program options
    const ProgramOption po_;
    long long nEvents_;
    int verbose_;

    // Pattern bank data
    std::unique_ptr<PatternBankInfo>       bankInfo_;
    std::unique_ptr<PatternBankContainer>  bank_;

    // Superstrip operation
    std::unique_ptr<SuperstripArbiter>     arbiter_;
};

#endif

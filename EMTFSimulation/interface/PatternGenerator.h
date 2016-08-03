#ifndef EMTFSimulation_PatternGenerator_h_
#define EMTFSimulation_PatternGenerator_h_

#include "L1TMuonSimulations/EMTFSimulation/interface/Helper.h"
#include "L1TMuonSimulations/EMTFSimulation/interface/ProgramOption.h"

#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/PatternBankContainer.h"
#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/SuperstripArbiter.h"


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

    // Analyze pattern bank
    void analyzePatterns(TString bank);

    // Program options
    const ProgramOption po_;
    long long nEvents_;
    int verbose_;

    // Pattern bank
    std::unique_ptr<PatternBankContainer>  bank_;

    // Superstrip operation
    std::unique_ptr<SuperstripArbiter>     arbiter_;
};

#endif

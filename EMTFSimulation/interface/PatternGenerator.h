#ifndef EMTFSimulation_PatternGenerator_h_
#define EMTFSimulation_PatternGenerator_h_

#include "L1TMuonSimulations/EMTFSimulationDataFormats/interface/Pattern.h"
#include "L1TMuonSimulations/EMTFSimulation/interface/Helper.h"
#include "L1TMuonSimulations/EMTFSimulation/interface/ProgramOption.h"

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
    std::map<pattern_type, pattern_attr>   patternBank_map_;
    std::vector<pattern_pair>              patternBank_pairs_;

    // Bookkeepers
    float coverage_;
    unsigned coverage_count_;

    // Operators
    std::unique_ptr<SuperstripArbiter> arbiter_;
};

#endif

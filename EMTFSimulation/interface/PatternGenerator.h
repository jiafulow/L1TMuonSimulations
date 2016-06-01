#ifndef EMTFSimulation_PatternGenerator_h_
#define EMTFSimulation_PatternGenerator_h_

#include "L1TMuonSimulations/EMTFSimulation/interface/Helper.h"
#include "L1TMuonSimulations/EMTFSimulation/interface/Pattern.h"
#include "L1TMuonSimulations/EMTFSimulation/interface/ProgramOption.h"

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
    std::map<pattern_type, unsigned>                patternBank_map_;
    std::vector<std::pair<pattern_type, unsigned> > patternBank_pairs_;

    // Bookkeepers
    float coverage_;
    unsigned coverage_count_;
};

#endif


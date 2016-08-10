#ifndef EMTFSimulation_PatternMatcher_h_
#define EMTFSimulation_PatternMatcher_h_

#include "L1TMuonSimulations/EMTFSimulation/interface/ProgramOption.h"

#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/Helper.h"
#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/SuperstripArbiter.h"
#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/AssociativeMemory.h"
#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/HitBuffer.h"


namespace phasetwoemtf {

class PatternMatcher {
  public:
    PatternMatcher(const ProgramOption& po);
    ~PatternMatcher();

    void init();
    void run();

  private:
    // Load pattern bank
    void loadPatterns(TString bank);

    // Do pattern recognition
    void matchPatterns(TString src, TString out);

    // Program options
    const ProgramOption po_;
    long long nEvents_;
    long long firstEvent_;
    int verbose_;

    // Configurations
    const TString prefixRoad_;
    const TString suffix_;

    // Superstrip operation
    std::unique_ptr<SuperstripArbiter> arbiter_;

    // Associlative memory
    std::unique_ptr<AssociativeMemory> associativeMemory_;

    // Hit buffer
    std::unique_ptr<HitBuffer> hitBuffer_;
};

}  // namespace phasetwoemtf

#endif

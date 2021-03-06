#ifndef EMTFSimulation_StubSelector_h_
#define EMTFSimulation_StubSelector_h_

#include "L1TMuonSimulations/EMTFSimulation/interface/ProgramOption.h"

#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/Helper.h"
#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/StubSelectorAlgo.h"


namespace phasetwoemtf {

class StubSelector {
  public:
    StubSelector(const ProgramOption& po);
    ~StubSelector();

    void init();
    void run();

  private:
    // Select one unique stub per layer
    void selectStubs(TString src, TString out);

    // Program options
    const ProgramOption po_;
    long long nEvents_;
    long long firstEvent_;
    int verbose_;

    // Selector algo
    std::unique_ptr<StubSelectorAlgo> algo_;
};

}  // namespace phasetwoemtf

#endif

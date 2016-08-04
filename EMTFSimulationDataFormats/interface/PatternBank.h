#ifndef EMTFSimulationDataFormats_PatternBank_h_
#define EMTFSimulationDataFormats_PatternBank_h_

#include "L1TMuonSimulations/EMTFSimulationDataFormats/interface/Pattern.h"
#include <vector>
#include <string>

namespace phasetwoemtf {

class PatternBankInfo {
public:
    PatternBankInfo() : coverage(0.), count(0) {}
    ~PatternBankInfo() {}

    float coverage;
    unsigned long count;
    unsigned tower;
    unsigned sector;
    std::string superstrip;
    unsigned superstrip_nx;
    unsigned superstrip_nz;
};

typedef std::vector<pattern_pair> PatternBank;

}  // namespace phasetwoemtf

#endif

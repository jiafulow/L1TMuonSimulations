#include "L1TMuonSimulations/EMTFSimulationDataFormats/interface/EMTFRoad.h"
#include <iostream>

namespace phasetwoemtf {

std::ostream& operator<<(std::ostream& o, const EMTFRoad& road) {
    o << "patternRef: " << road.patternRef << " sector: " << road.sector << " # stubs: " << road.nstubs << " patternInvPt: " << road.patternInvPt << " superstripIds: (";
    for (unsigned i=0; i<road.superstripIds.size(); ++i) {
        o << road.superstripIds.at(i) << ",";
    }
    o << ")" << " stubRefs: (";
    for (unsigned i=0; i<road.stubRefs.size(); ++i) {
        o << "[";
        for (unsigned j=0; j<road.stubRefs.at(i).size(); ++j) {
            o << road.stubRefs.at(i).at(j) << ",";
        }
        o << "],";
    }
    o << ")";
    return o;
}

}  // namespace phasetwoemtf

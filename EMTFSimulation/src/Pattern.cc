#include "L1TMuonSimulations/EMTFSimulation/interface/Pattern.h"
#include <iostream>
#include <iomanip>

std::ostream& operator<<(std::ostream& o, const pattern_type& patt) {
    for (pattern_type::const_iterator it=patt.begin(); it!=patt.end(); ++it) {
        o << std::setw(6) << *it << " ";
    }
    return o;
}

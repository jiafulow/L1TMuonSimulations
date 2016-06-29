#include "L1TMuonSimulations/EMTFSimulationDataFormats/interface/Pattern.h"
#include <iostream>
#include <iomanip>
#include <cmath>

std::ostream& operator<<(std::ostream& o, const pattern_t& patt) {
    for (pattern_t::const_iterator it=patt.begin(); it!=patt.end(); ++it) {
        o << std::setw(6) << *it << " ";
    }
    return o;
}

std::ostream& operator<<(std::ostream& o, const attrib_t& attr) {
    o << "n: " << attr.n
      << " invPt_mean: " << attr.invPt_mean
      << " invPt_variance: " << attr.invPt_variance
      << " cotTheta_mean: " << attr.cotTheta_mean
      << " cotTheta_variance: " << attr.cotTheta_variance
      << " phi_mean: " << attr.phi_mean
      << " phi_variance: " << attr.phi_variance
      << " z0_mean: " << attr.z0_mean
      << " z0_variance: " << attr.z0_variance
      //<< " d0_mean: " << attr.d0_mean
      //<< " d0_variance: " << attr.d0_variance
      ;
    return o;
}

std::ostream& operator<<(std::ostream& o, const pattern_pair& pair) {
    o << pair.first << " " << pair.second;
    return o;
}

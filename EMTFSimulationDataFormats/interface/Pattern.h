#ifndef EMTFSimulationDataFormats_Pattern_h_
#define EMTFSimulationDataFormats_Pattern_h_

#include <stdint.h>
#include <array>
#include <iosfwd>

typedef uint32_t superstrip_type;
typedef std::array<superstrip_type,8> pattern_type;

struct PatternAttribute {
    unsigned long n;  // popularity
    float invPt_mean;
    float invPt_variance;
    float cotTheta_mean;
    float cotTheta_variance;
    float phi_mean;
    float phi_variance;
    float z0_mean;
    float z0_variance;
    //float d0_mean;
    //float d0_variance;
};
typedef PatternAttribute pattern_attr;

typedef std::pair<pattern_type, pattern_attr> pattern_pair;


// _____________________________________________________________________________
// Output streams
std::ostream& operator<<(std::ostream& o, const pattern_type& patt);

#endif


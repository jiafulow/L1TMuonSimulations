#ifndef EMTFSimulationDataFormats_Pattern_h_
#define EMTFSimulationDataFormats_Pattern_h_

#include <cstdint>
#include <array>
#include <iosfwd>


namespace phasetwoemtf {

typedef std::uint32_t superstrip_t;
typedef std::array<superstrip_t,8> pattern_t;

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

    void reset() {
        n                 = 0;
        invPt_mean        = 0.;
        invPt_variance    = 0.;
        cotTheta_mean     = 0.;
        cotTheta_variance = 0.;
        phi_mean          = 0.;
        phi_variance      = 0.;
        z0_mean           = 0.;
        z0_variance       = 0.;
        //d0_mean           = 0.;
        //d0_variance       = 0.;
    }
};
typedef PatternAttribute attrib_t;

typedef std::pair<pattern_t, attrib_t> pattern_pair;


// _____________________________________________________________________________
// Output streams
std::ostream& operator<<(std::ostream& o, const pattern_t& patt);
std::ostream& operator<<(std::ostream& o, const attrib_t& attr);
std::ostream& operator<<(std::ostream& o, const pattern_pair& pair);

}  // namespace phasetwoemtf

#endif


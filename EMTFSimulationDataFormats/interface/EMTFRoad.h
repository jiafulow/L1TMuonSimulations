#ifndef EMTFSimulationDataFormats_EMTFRoad_h_
#define EMTFSimulationDataFormats_EMTFRoad_h_

#include <vector>
#include <iosfwd>


namespace phasetwoemtf {

struct EMTFRoad {
    unsigned patternRef;
    unsigned sector;
    unsigned nstubs;
    float    patternInvPt;

    std::vector<unsigned> superstripIds;           // superstripIds[superstrip i]
    std::vector<std::vector<unsigned> > stubRefs;  // stubRefs[superstrip i][stub j]
};


// _____________________________________________________________________________
// Output streams
std::ostream& operator<<(std::ostream& o, const EMTFRoad& road);

}  // namespace phasetwoemtf

#endif

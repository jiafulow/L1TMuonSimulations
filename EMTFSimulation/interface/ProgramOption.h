#ifndef EMTFSimulation_ProgramOption_h_
#define EMTFSimulation_ProgramOption_h_

#include <string>
#include <vector>
#include <iosfwd>


namespace phasetwoemtf {

struct ProgramOption {
    std::string input;
    std::string output;
    std::string bankfile;

    int         verbose;
    long long   maxEvents;
    long long   skipEvents;
    std::string datadir;

    float       minPt;
    float       maxPt;
    float       minInvPt;
    float       maxInvPt;
    float       minEta;
    float       maxEta;
    float       minPhi;
    float       maxPhi;
    float       minVz;
    float       maxVz;

    unsigned    tower;
    unsigned    sector;
    std::string superstrip;
    std::string fitter;

    int         minPopularity;
    float       maxCoverage;
};

// _____________________________________________________________________________
// Output streams
std::ostream& operator<<(std::ostream& o, const ProgramOption& po);

}  // namespace phasetwoemtf

#endif


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

    unsigned    nLayers;

    unsigned    tower;
    unsigned    sector;
    std::string superstrip;
    std::string fitter;

    unsigned    minPopularity;
    float       maxCoverage;
    float       maxPattInvPt;
    long int    maxPatterns;
    int         maxMisses;
    int         maxStubs;
    int         maxRoads;
};

// _____________________________________________________________________________
// Output streams
std::ostream& operator<<(std::ostream& o, const ProgramOption& po);

}  // namespace phasetwoemtf

#endif


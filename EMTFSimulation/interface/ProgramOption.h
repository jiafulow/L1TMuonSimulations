#ifndef EMTFSimulation_ProgramOption_h_
#define EMTFSimulation_ProgramOption_h_

#include <string>
#include <vector>
#include <iosfwd>

struct ProgramOption {
    std::string input;
    std::string output;
    std::string bankfile;

    int         verbose;
    long long   maxEvents;
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

    int         minFrequency;
    float       maxCoverage;
};

// _____________________________________________________________________________
// Output streams
std::ostream& operator<<(std::ostream& o, const ProgramOption& po);

#endif


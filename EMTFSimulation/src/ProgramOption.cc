#include "L1TMuonSimulations/EMTFSimulation/interface/ProgramOption.h"

#include <iostream>
#include <iterator>

// Print vector
template<class T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& v) {
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(o, " "));
    return o;
}

std::ostream& operator<<(std::ostream& o, const ProgramOption& po) {
    o << "Parsed program options:\n"
      << "  input: "        << po.input
      << "  output: "       << po.output
      << "  bankfile: "     << po.bankfile

      << "  verbose: "      << po.verbose
      << "  maxEvents: "    << po.maxEvents
      << "  datadir: "      << po.datadir

      << "  minPt: "        << po.minPt
      << "  maxPt: "        << po.maxPt
      << "  minInvPt: "     << po.minInvPt
      << "  maxInvPt: "     << po.maxInvPt
      << "  minEta: "       << po.minEta
      << "  maxEta: "       << po.maxEta
      << "  minPhi: "       << po.minPhi
      << "  maxPhi: "       << po.maxPhi
      << "  minVz: "        << po.minVz
      << "  maxVz: "        << po.maxVz

      << "  tower: "        << po.tower
      << "  sector: "       << po.sector
      << "  superstrip: "   << po.superstrip
      << "  fitter: "       << po.fitter

      << "  minPopularity: " << po.minPopularity
      << "  maxCoverage: "  << po.maxCoverage
      ;

    return o;
}


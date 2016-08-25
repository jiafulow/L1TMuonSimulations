#include "L1TMuonSimulations/EMTFSimulation/interface/ProgramOption.h"
#include "L1TMuonSimulations/EMTFSimulationIO/interface/MessageLogger.h"


namespace phasetwoemtf {

std::ostream& operator<<(std::ostream& o, const ProgramOption& po) {
    o << "Parsed program options:\n"
      << "input: "           << po.input
      << "  output: "        << po.output
      << "  bankfile: "      << po.bankfile

      << "  verbose: "       << po.verbose
      << "  maxEvents: "     << po.maxEvents
      << "  skipEvents: "    << po.skipEvents
      << "  datadir: "       << po.datadir

      << "  minPt: "         << po.minPt
      << "  maxPt: "         << po.maxPt
      << "  minInvPt: "      << po.minInvPt
      << "  maxInvPt: "      << po.maxInvPt
      << "  minEta: "        << po.minEta
      << "  maxEta: "        << po.maxEta
      << "  minPhi: "        << po.minPhi
      << "  maxPhi: "        << po.maxPhi
      << "  minVz: "         << po.minVz
      << "  maxVz: "         << po.maxVz

      << "  nLayers: "       << po.nLayers

      << "  tower: "         << po.tower
      << "  sector: "        << po.sector
      << "  superstrip: "    << po.superstrip
      << "  fitter: "        << po.fitter

      << "  minPopularity: " << po.minPopularity
      << "  maxCoverage: "   << po.maxCoverage
      << "  maxPattInvPt: "  << po.maxPattInvPt
      << "  maxPatterns: "   << po.maxPatterns
      << "  maxMisses: "     << po.maxMisses
      << "  maxStubs: "      << po.maxStubs
      << "  maxRoads: "      << po.maxRoads
      ;

    return o;
}

}  // namespace phasetwoemtf


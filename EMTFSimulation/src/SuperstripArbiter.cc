#include "L1TMuonSimulations/EMTFSimulation/interface/SuperstripArbiter.h"


namespace {
    unsigned round_to_uint(float x) {
        return std::floor(x + 0.5);
    }
}

// _____________________________________________________________________________
SuperstripArbiter::SuperstripArbiter()
: sstype_(SuperstripType::UNKNOWN),
  nx_(1),
  nz_(1),
  useGlobalCoord_(false) {

}

SuperstripArbiter::~SuperstripArbiter() {

}

// _____________________________________________________________________________
void SuperstripArbiter::setDefinition(TString definition) {
    if (definition == "") {
        sstype_ = SuperstripType::UNKNOWN;
    }
}

// _____________________________________________________________________________
unsigned SuperstripArbiter::superstripLocal(unsigned moduleId, float strip, float segment, float ds) const {
    unsigned ss = (moduleId << 16) | (round_to_uint(strip) << 0);  // FIXME
    return ss;
}

// _____________________________________________________________________________
unsigned SuperstripArbiter::superstripGlobal(unsigned moduleId, float rho, float phi, float theta, float bend) const {
    unsigned ss = 0;  // FIXME
    return ss;
}

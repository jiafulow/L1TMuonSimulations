#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/SuperstripArbiter.h"

#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/Helper.h"
#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/HelperString.h"
#include <cassert>
#include <stdexcept>
#include <iostream>


namespace {
unsigned round_to_uint(float x) {
    return std::floor(x + 0.5);
}
}  // namespace

namespace phasetwoemtf {

// _____________________________________________________________________________
SuperstripArbiter::SuperstripArbiter()
: definition_(""),
  coordType_(SuperstripCoordType::LOCAL),
  shapeType_(SuperstripShapeType::UNKNOWN),
  nx_(1),
  nz_(1),
  localParams_(),
  globalParams_() {}

SuperstripArbiter::~SuperstripArbiter() {}

// _____________________________________________________________________________
void SuperstripArbiter::setDefinition(const std::string& definition) {
    definition_ = definition;

    if (startswith(definition_, "ss")) {
        SuperstripFixedWidth shape;
        shape.setDefinition(definition_, coordType_, shapeType_, nx_, nz_, localParams_);

    } else if (startswith(definition_, "nx")) {
        SuperstripProjective shape;
        shape.setDefinition(definition_, coordType_, shapeType_, nx_, nz_, globalParams_);

    } else {
        throw std::invalid_argument("Incorrect superstrip definition.");
    }
}

// _____________________________________________________________________________
unsigned SuperstripArbiter::superstripLocal(unsigned moduleId, float strip, float segment, float ds) const {
    unsigned ss = 0;

    unsigned istrip = strip;  // in half-strip unit
    unsigned isegment = segment;

    unsigned layME = decodeLayerME(moduleId);
    unsigned istation = decodeStation(moduleId);
    unsigned iring = decodeRing(moduleId);

    // Fix ME1/A
    if (istation == 1 && iring == 1 && istrip >= 128) {
        iring = 4;
        istrip -= 128;
    }

    // Normalize strip pitch because strip pitch is not constant in ME1/n
    if (istation == 1 && iring == 4) {
        istrip *= unsigned(std::round(3.878509045 / 2.327105045 * 1024));
        istrip >>= 10;

    } else if (istation == 1 && iring == 3) {
        istrip *= unsigned(std::round(2.152551889 / 2.327105045 * 1024));
        istrip >>= 10;

    } else if (istation == 1 && iring == 1) {
        istrip *= unsigned(std::round(2.958184958 / 2.327105045 * 1024));
        istrip >>= 10;

    } else {
        istrip *= 1024;
        istrip >>= 10;
    }

    // Use full-strip in 10-degree chambers (but half-strip in 20-degree chambers)
    if (istation == 1 || iring == 2) {
        istrip >>= 1;
    }

    // Join the numbers
    const SuperstripLocalParams& p = localParams_.at(layME);
    ss |= ((istrip >> p.right_shift_x) & p.mask_x);
    ss |= (((isegment >> p.right_shift_z) & p.mask_z) << p.shift_z);
    ss |= (moduleId << p.shift_m);
    return ss;
}

// _____________________________________________________________________________
unsigned SuperstripArbiter::superstripGlobal(unsigned moduleId, float rho, float phi, float theta, float bend) const {
    unsigned ss = 0;

    //
    // FIXME
    //

    return ss;
}

// _____________________________________________________________________________
std::string SuperstripArbiter::str() const {
    std::ostringstream o;
    o << "def: " << getDefinition()
      << "  coord: " << getCoordType()
      << "  shape: " << getShapeType()
      << "  nx: " << nx()
      << "  nz: " << nz()
      << "  localParams[0]: " << (localParams_.empty() ? SuperstripLocalParams() : localParams_.front())
      << "  globalParams[0]: " << (globalParams_.empty() ? SuperstripGlobalParams() : globalParams_.front());
    return o.str();
}

}  // namespace phasetwoemtf

#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/SuperstripShapes.h"

#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/HelperString.h"
#include <cassert>
#include <stdexcept>
#include <iostream>


namespace {
static const unsigned NLAYERS = 5;

bool is_power_of_two(int x) {
    return x > 0 && !(x & (x-1));
}

unsigned most_sig_bit(unsigned v) {
    unsigned r = 0;
    while (v >>= 1)
        r++;
    return r;
}
}  // namespace

namespace phasetwoemtf {

// _____________________________________________________________________________
void SuperstripFixedWidth::setDefinition(const std::string& definition, SuperstripCoordType& coord, SuperstripShapeType& shape,
        unsigned& nx, unsigned& nz, std::vector<SuperstripLocalParams>& params) const {
    // Parse the definition string
    std::vector<std::string> tokens = split(definition, '_');
    assert(tokens.size() == 2);

    std::string token1 = tokens.at(0);
    assert(startswith(token1, "ss"));
    token1 = token1.substr(2);
    token1 = replace_all(token1, "p", ".");
    float token1f = std::stof(token1);

    std::string token2 = tokens.at(1);
    assert(startswith(token2, "sg"));
    token2 = token2.substr(2);
    token2 = replace_all(token2, "p", ".");
    float token2f = std::stof(token2);

    unsigned nstrips = token1f;
    assert(is_power_of_two(nstrips));

    unsigned nsegments = token2f;
    assert(is_power_of_two(nsegments));

    // Calculate the parameters
    SuperstripLocalParams p;
    p.right_shift_x = most_sig_bit(nstrips);
    p.mask_x = 0xff; // 0-255 (8 bits)
    p.right_shift_z = most_sig_bit(nsegments);
    p.mask_z = 0x7f; // 0-127 (7 bits)
    p.shift_z = 8;
    p.shift_m = 8 + 7;

    // Return values
    coord = coordType_;
    shape = shapeType_;
    nx = 1u<<8;
    nz = 1u<<7;
    params.clear();
    for (unsigned i=0; i<NLAYERS; i++)
        params.push_back(p);
    return;
}

void SuperstripFixedWidth::setDefinition(const std::string& definition, SuperstripCoordType& coord, SuperstripShapeType& shape,
        unsigned& nx, unsigned& nz, std::vector<SuperstripGlobalParams>& params) const {
    throw std::invalid_argument("Incorrect superstrip definition.");
}

// _____________________________________________________________________________
void SuperstripProjective::setDefinition(const std::string& definition, SuperstripCoordType& coord, SuperstripShapeType& shape,
        unsigned& nx, unsigned& nz, std::vector<SuperstripLocalParams>& params) const {
    throw std::invalid_argument("Incorrect superstrip definition.");
}

void SuperstripProjective::setDefinition(const std::string& definition, SuperstripCoordType& coord, SuperstripShapeType& shape,
        unsigned& nx, unsigned& nz, std::vector<SuperstripGlobalParams>& params) const {
    // Parse the definition string
    std::vector<std::string> tokens = split(definition, '_');
    assert(tokens.size() == 2);

    std::string token1 = tokens.at(0);
    assert(startswith(token1, "nx"));
    token1 = token1.substr(2);
    token1 = replace_all(token1, "p", ".");
    float token1f = std::stof(token1);

    std::string token2 = tokens.at(1);
    assert(startswith(token2, "nz"));
    token2 = token2.substr(2);
    token2 = replace_all(token2, "p", ".");
    float token2f = std::stof(token2);

    //
    // FIXME
    //

    return;
}

// _____________________________________________________________________________
std::ostream& operator<<(std::ostream& o, const SuperstripLocalParams& p) {
    o << "rsx: " << p.right_shift_x << " mx: " << p.mask_x << " rsz: " << p.right_shift_z << " mz: " << p.mask_z << " sz: " << p.shift_z << " sm: " << p.shift_m;
    return o;
}

std::ostream& operator<<(std::ostream& o, const SuperstripGlobalParams& p) {
    o << "rox: " << p.rotate_x << " lx: " << p.low_x << " dx: " << p.delta_x << " lz: " << p.low_z << " dz: " << p.delta_z;
    return o;
}

}  // namespace phasetwoemtf

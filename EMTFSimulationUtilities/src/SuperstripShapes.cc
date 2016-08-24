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
        unsigned& nx, unsigned& nz, unsigned& nss, std::vector<SuperstripLocalParams>& params) const {
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
    p.mask_z = 0xf;  // 0-15 (4 bits)
    p.shift_z = 8;
    p.mask_m = 0x3f; // 0-63 (6 bits)
    p.shift_m = 8+4;

    // Return values
    coord = coordType_;
    shape = shapeType_;
    nx = 1u<<8;
    nz = 1u<<4;
    nss = 1u<<(8+4+6);
    params.clear();
    for (unsigned i=0; i<NLAYERS; i++)
        params.push_back(p);
    return;
}

void SuperstripFixedWidth::setDefinition(const std::string& definition, SuperstripCoordType& coord, SuperstripShapeType& shape,
        unsigned& nx, unsigned& nz, unsigned& nss, std::vector<SuperstripGlobalParams>& params) const {
    throw std::invalid_argument("Incorrect superstrip definition.");
}

// _____________________________________________________________________________
void SuperstripProjective::setDefinition(const std::string& definition, SuperstripCoordType& coord, SuperstripShapeType& shape,
        unsigned& nx, unsigned& nz, unsigned& nss, std::vector<SuperstripLocalParams>& params) const {
    throw std::invalid_argument("Incorrect superstrip definition.");
}

void SuperstripProjective::setDefinition(const std::string& definition, SuperstripCoordType& coord, SuperstripShapeType& shape,
        unsigned& nx, unsigned& nz, unsigned& nss, std::vector<SuperstripGlobalParams>& params) const {
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

    // Calculate the parameters
    SuperstripGlobalParams p;
    p.low_x = -M_PI/3;     // CUIDADO: this is to cover -60 to +60 degrees after rotation
    p.delta_x = 2.327105045e-3;  // single-strip pitch in the 10-degree chamber
    p.mask_x = 0x1fff;  // 0-8191 (12 bits)
    p.low_z = 0.0;
    p.delta_z = 1.0;
    p.mask_z = 0xf;  // 0-15 (4 bits)
    p.shift_z = 12;

    // Return values
    coord = coordType_;
    shape = shapeType_;
    nx = 1u<<12;
    nz = 1u<<4;
    nss = 1u<<(12+4);
    params.clear();
    for (unsigned i=0; i<NLAYERS; i++)
        params.push_back(p);
    return;
}

// _____________________________________________________________________________
std::ostream& operator<<(std::ostream& o, const SuperstripLocalParams& p) {
    o << "rsx: " << p.right_shift_x << " mx: " << p.mask_x << " rsz: " << p.right_shift_z << " mz: " << p.mask_z << " sz: " << p.shift_z << " mm: " << p.mask_m << " sm: " << p.shift_m;
    return o;
}

std::ostream& operator<<(std::ostream& o, const SuperstripGlobalParams& p) {
    o << " lx: " << p.low_x << " dx: " << p.delta_x << " mx: " << p.mask_x << " lz: " << p.low_z << " dz: " << p.delta_z << " mz: " << p.mask_z << " sz: " << p.shift_z;
    return o;
}

}  // namespace phasetwoemtf

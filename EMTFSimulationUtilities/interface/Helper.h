#ifndef EMTFSimulationUtilities_Helper_h_
#define EMTFSimulationUtilities_Helper_h_

#include <algorithm>
//#include <functional>
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "TROOT.h"  // for gROOT
#include "TString.h"
#include "TTree.h"

// Retrieve layer, ladder, module from a moduleId
inline unsigned decodeLayer(unsigned moduleId) {   // mysector 0-11
    return (moduleId / (2*12 + 3*9));
}
inline unsigned decodeLadder(unsigned moduleId) {  // mystation 0-5
    return (moduleId % (2*12 + 3*9)) < (2*12) ? (moduleId % (2*12 + 3*9))/12 : ((moduleId % (2*12 + 3*9)) - (2*12))/9 + 2;
}
inline unsigned decodeModule(unsigned moduleId) {  // mychamber 0-11
    return (moduleId % (2*12 + 3*9)) < (2*12) ? (moduleId % (2*12 + 3*9))%12 : ((moduleId % (2*12 + 3*9)) - (2*12))%9;
}

// Retrieve endcap, station, ring. cscID, sector, subsector
inline unsigned decodeEndcap(unsigned moduleId) {
    unsigned lay = decodeLayer(moduleId);
    return (lay/6) + 1;
}
inline unsigned decodeStation(unsigned moduleId) {
    unsigned lad = decodeLadder(moduleId);
    return lad < 2 ? 1 : lad;
}
inline unsigned decodeRing(unsigned moduleId) {
    unsigned lad = decodeLadder(moduleId);
    unsigned mod = decodeModule(moduleId);
    return lad < 2 ? ((mod/3) + 1) : ((mod/3) == 0 ? 1 : 2);
}
inline unsigned decodeCscID(unsigned moduleId) {
    unsigned lad = decodeLadder(moduleId);
    unsigned mod = decodeModule(moduleId);
    return lad < 2 ? ((mod%9) + 1) : (mod + 1);
}
inline unsigned decodeSector(unsigned moduleId) {
    unsigned lay = decodeLayer(moduleId);
    return (lay%6) + 1;
}
inline unsigned decodeSubSector(unsigned moduleId) {
    unsigned lad = decodeLadder(moduleId);
    return lad < 2 ? lad + 1 : 0;
}

// Retrieve 'layerME' used for making patterns
inline unsigned decodeLayerME(unsigned moduleId) {
    unsigned lad = decodeLadder(moduleId);
    unsigned mod = decodeModule(moduleId);
    return lad < 2 ? ((mod%9) > 2) : lad;
}

// Get min, max moduleId
inline unsigned getMinModuleId() {
    return 0;
}
inline unsigned getMaxModuleId() {
    return (12 * (2*12 + 3*9)) - 1;
}

#endif

#ifndef EMTFSimulation_Helper_h_
#define EMTFSimulation_Helper_h_

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
inline unsigned decodeLayer(unsigned moduleId) {
    return (moduleId / 10000);
}
inline unsigned decodeLadder(unsigned moduleId) {
    return (moduleId / 100) % 100;
}
inline unsigned decodeModule(unsigned moduleId) {
    return (moduleId) % 100;
}

// Compress layer id
inline unsigned decodeLayerME(unsigned moduleId) {
    unsigned layME = 99;
    unsigned lay = decodeLayer(moduleId);
    unsigned lad = decodeLadder(moduleId);
    unsigned st = lay%10;
    if (st == 1 && lad == 1) {
        layME = 0;
    } else if (st == 1) {
        layME = 1;
    } else if (2 <= st && st <= 4) {
        layME = st;
    }
    return layME;
}

#endif


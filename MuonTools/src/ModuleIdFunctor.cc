#include "L1TMuonSimulations/MuonTools/interface/ModuleIdFunctor.h"

#include <cassert>

uint32_t ModuleIdFunctor::operator() (const DetId& id) const {
    uint32_t layer  = 999999;
    uint32_t ladder = 999999;
    uint32_t module = 999999;
    uint32_t ret    = 999999;

    if (id.det() == DetId::Muon) {
        if (id.subdetId() == (int) MuonSubdetId::CSC) {
            CSCDetId cscDet(id);
            layer  = cscDet.endcap() * 10 + cscDet.station();
            ladder = cscDet.ring();
            module = cscDet.chamber();
        }
    }

    if (layer != 999999 && ladder != 999999 && module != 999999) {
        ret = 10000*layer + 100*ladder + module;
    }

    return ret;
}

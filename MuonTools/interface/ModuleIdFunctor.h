#ifndef MuonTools_ModuleIdFunctor_h_
#define MuonTools_ModuleIdFunctor_h_

#include "DataFormats/DetId/interface/DetId.h"


class ModuleIdFunctor {
  public:
    uint32_t operator() (const DetId& id) const;
};

class ModuleIdHelper {
public:
    static uint32_t moduleId(const DetId& id);

    static bool isEven(const DetId& id);
    static bool isFront(const DetId& id);
    static bool isCounterClockwise(const DetId& id);
};

#endif

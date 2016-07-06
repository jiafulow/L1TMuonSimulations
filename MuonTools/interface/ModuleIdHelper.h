#ifndef MuonTools_ModuleIdHelper_h_
#define MuonTools_ModuleIdHelper_h_

#include "DataFormats/DetId/interface/DetId.h"


class ModuleIdHelper {
public:
    static uint32_t getModuleId(const DetId& id);

    static bool isEven(const DetId& id);
    static bool isFront(const DetId& id);
    static bool isCounterClockwise(const DetId& id);
};

#endif

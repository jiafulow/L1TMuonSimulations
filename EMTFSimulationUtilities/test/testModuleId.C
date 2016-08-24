#include "../interface/Helper.h"

#include <iostream>

int testModuleId() {
    std::cout << "[INFO] Trying to decode all the moduleIds" << std::endl;

    unsigned minModuleId = getMinModuleId();
    unsigned maxModuleId = getMaxModuleId();

    for (unsigned moduleId=minModuleId; moduleId<=maxModuleId; ++moduleId) {
        unsigned lay = decodeLayer(moduleId);
        unsigned lad = decodeLadder(moduleId);
        unsigned mod = decodeModule(moduleId);
        unsigned layME = decodeLayerME(moduleId);
        unsigned modME = decodeModuleME(moduleId);

        unsigned iendcap = decodeEndcap(moduleId);
        unsigned istation = decodeStation(moduleId);
        unsigned iring = decodeRing(moduleId);
        unsigned cscID = decodeCscID(moduleId);
        unsigned isector = decodeSector(moduleId);
        unsigned isubsector = decodeSubSector(moduleId);
        unsigned ichambertype = decodeChamberType(moduleId);

        std::cout << "moduleId: " << moduleId
                  << " mysector: " << lay
                  << " mystation: " << lad
                  << " mychamber: " << mod
                  << " mylayer: " << layME
                  << " mymodule: " << modME
                  << " iendcap: " << iendcap
                  << " istation: " << istation
                  << " iring: " << iring
                  << " cscID: " << cscID
                  << " isector: " << isector
                  << " isubsector: " << isubsector
                  << " ichambertype: " << ichambertype
                  << std::endl;
    }

    return 0;
}

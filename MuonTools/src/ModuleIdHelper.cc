#include "L1TMuonSimulations/MuonTools/interface/ModuleIdHelper.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"

#include <cassert>

// TODO: fix ME1/A coordinates

uint32_t ModuleIdHelper::getModuleId(const DetId& id) {
    uint32_t result = 0;
    if (id.det() == DetId::Muon) {
        if (id.subdetId() == (int) MuonSubdetId::CSC) {
            const CSCDetId cscDet(id);

            if (cscDet.station() == 1) {  // station 1
                int subsector = (cscDet.chamber()%6 > 2) ? 1 : 2;
                int mystation = subsector - 1;
                int mychamber = (cscDet.ring() == 4) ? cscDet.triggerCscId()-1+9 : cscDet.triggerCscId()-1;
                result = (mystation * 12) + mychamber;

            } else {                      // stations 2,3,4
                int mystation = cscDet.station()-2;
                int mychamber = cscDet.triggerCscId()-1;
                int offset    = (2*12);
                result = (mystation * 9) + mychamber + offset;
            }

            int mysector  = (cscDet.endcap()-1)*6 + (cscDet.triggerSector()-1);
            result += (mysector * (2*12 + 3*9));
        }
    }
    return result;
}

bool ModuleIdHelper::isEven(const DetId& id) {
    bool result = false;
    if (id.det() == DetId::Muon) {
        if (id.subdetId() == (int) MuonSubdetId::CSC) {
            const CSCDetId cscDet(id);

            result = (cscDet.chamber()%2 == 0);
        }
    }
    return result;
}

bool ModuleIdHelper::isFront(const DetId& id) {
    bool result = false;
    if (id.det() == DetId::Muon) {
        if (id.subdetId() == (int) MuonSubdetId::CSC) {
            const CSCDetId cscDet(id);

            bool isOverlapping = !(cscDet.station() == 1 && cscDet.ring() == 3);
            if (isOverlapping) {
                bool isEven = (cscDet.chamber()%2 == 0);
                result = (cscDet.station()<3) ? isEven: !isEven;
            }
        }
    }
    return result;
}

bool ModuleIdHelper::isCounterClockwise(const DetId& id) {
    bool result = false;
    if (id.det() == DetId::Muon) {
        if (id.subdetId() == (int) MuonSubdetId::CSC) {
            const CSCDetId cscDet(id);

            bool isStationOneOrTwo = (cscDet.station() == 1 || cscDet.station() == 2);
            result = (cscDet.endcap()==1) ? isStationOneOrTwo: !isStationOneOrTwo;
        }
    }
    return result;
}


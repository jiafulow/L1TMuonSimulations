#ifndef MuonTools_ModuleIdFunctor_h_
#define MuonTools_ModuleIdFunctor_h_

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"


class ModuleIdFunctor {
  public:
    uint32_t operator() (const DetId& id) const;
};

#endif

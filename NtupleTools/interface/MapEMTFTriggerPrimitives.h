#ifndef NTupleTools_MapEMTFTriggerPrimitives_h_
#define NTupleTools_MapEMTFTriggerPrimitives_h_

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/L1TMuon/interface/EMTFHit.h"
#include "DataFormats/L1TMuon/interface/EMTFHitExtra.h"
#include <map>


class MapEMTFTriggerPrimitives {
  public:
    typedef l1t::EMTFHitExtra           Stub;
    typedef l1t::EMTFHitExtraCollection StubCollection;
    typedef edm::Ref<StubCollection>    StubRef;

    MapEMTFTriggerPrimitives() {}
    ~MapEMTFTriggerPrimitives() {}

    void setup(const edm::Handle<StubCollection>& handle);

    unsigned size() const { return mapping.size(); }

    unsigned get(const StubRef aref);

  private:
    std::map<StubRef, unsigned> mapping;
};

#endif

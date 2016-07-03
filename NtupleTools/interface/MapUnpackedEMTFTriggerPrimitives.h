#ifndef NTupleTools_MapUnpackedEMTFTriggerPrimitives_h_
#define NTupleTools_MapUnpackedEMTFTriggerPrimitives_h_

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/L1TMuon/interface/EMTFHit.h"
//#include "DataFormats/L1TMuon/interface/EMTFHitExtra.h"
#include <map>


class MapUnpackedEMTFTriggerPrimitives {
  public:
    typedef l1t::EMTFHit                Stub;
    typedef l1t::EMTFHitCollection      StubCollection;
    typedef edm::Ref<StubCollection>    StubRef;

    MapUnpackedEMTFTriggerPrimitives() {}
    ~MapUnpackedEMTFTriggerPrimitives() {}

    void setup(const edm::Handle<StubCollection>& handle);

    unsigned size() const { return mapping.size(); }

    unsigned get(const StubRef aref);

  private:
    std::map<StubRef, unsigned> mapping;
};

#endif

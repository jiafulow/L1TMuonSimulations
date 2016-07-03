#ifndef NTupleTools_MapCSCTriggerPrimitives_h_
#define NTupleTools_MapCSCTriggerPrimitives_h_

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"
#include <map>


class MapCSCTriggerPrimitives {
  public:
    typedef CSCCorrelatedLCTDigiCollection StubCollection;
    typedef std::tuple<unsigned, int, int, int, int, int, int> StubRef;  // CSCDetId, keywire, strip, pattern, bend, bx, cscID

    MapCSCTriggerPrimitives() {}
    ~MapCSCTriggerPrimitives() {}

    void setup(const edm::Handle<StubCollection>& handle);

    unsigned size() const { return mapping.size(); }

    unsigned get(const StubRef aref);

  private:
    std::map<StubRef, unsigned> mapping;
};

#endif

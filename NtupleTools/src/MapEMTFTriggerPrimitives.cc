#include "L1TMuonSimulations/NtupleTools/interface/MapEMTFTriggerPrimitives.h"

void MapEMTFTriggerPrimitives::setup(const edm::Handle<StubCollection>& handle) {
    if (handle.isValid()) {
        unsigned n = 0;
        for (StubCollection::const_iterator it = handle->begin(); it != handle->end(); ++it) {
            const StubRef aref(handle, n);
            mapping.insert(std::make_pair(aref, n));

            n++;
        }
    }
}

unsigned MapEMTFTriggerPrimitives::get(const StubRef aref) {
    return mapping.at(aref);  // must exist
}

#include "L1TMuonSimulations/NtupleTools/interface/MapCSCTriggerPrimitives.h"

void MapCSCTriggerPrimitives::setup(const edm::Handle<StubCollection>& handle) {
    if (handle.isValid()) {
        unsigned n = 0;

        // Loop over chambers
        for (CSCCorrelatedLCTDigiCollection::DigiRangeIterator itr = handle->begin(); itr != handle->end(); ++itr) {

            // Loop over digis
            for (CSCCorrelatedLCTDigiCollection::const_iterator it = (*itr).second.first; it != (*itr).second.second; ++it) {
                const StubRef aref = std::make_tuple((*itr).first.rawId(), it->getKeyWG(), it->getStrip(),
                    it->getPattern(), it->getBend(), it->getBX(), it->getCSCID());
                mapping.insert(std::make_pair(aref, n));

                n++;
            }
        }
    }
}

unsigned MapCSCTriggerPrimitives::get(const StubRef aref) {
    return mapping.at(aref);  // must exist
}

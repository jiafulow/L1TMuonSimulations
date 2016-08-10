#ifndef EMTFSimulationUtilities_HitBuffer_h_
#define EMTFSimulationUtilities_HitBuffer_h_

#include "L1TMuonSimulations/EMTFSimulationDataFormats/interface/Pattern.h"
#include <memory>
#include <map>
#include <vector>

namespace phasetwoemtf {

class HitBuffer {
public:
    // Constructor
    HitBuffer() : nlayers_(0), nss_(0), frozen_(false) {}

    // Destructor
    ~HitBuffer() {}

    // Functions
    void init(unsigned nlayers, unsigned nss);

    void reset();

    void insert(unsigned layer, superstrip_t ssId, unsigned stubRef);

    void freeze(unsigned maxStubs);

    bool hasHits(unsigned layer, superstrip_t ssId) const;

    std::vector<unsigned> getHits(unsigned layer, superstrip_t ssId) const;

    // Debug
    std::string str() const;

private:
    // Member data
    std::map<superstrip_t, std::vector<unsigned> > superstripHits_;   // superstrip --> stubRefs (std::map)
    std::vector<bool>                              superstripBools_;  // superstrip --> hit or empty (hash table)
    unsigned nlayers_;
    unsigned nss_;
    bool frozen_;
};

}  // namespace phasetwoemtf

#endif

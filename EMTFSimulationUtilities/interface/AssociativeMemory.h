#ifndef EMTFSimulationUtilities_AssociativeMemory_h_
#define EMTFSimulationUtilities_AssociativeMemory_h_

#include "L1TMuonSimulations/EMTFSimulationDataFormats/interface/Pattern.h"
#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/HitBuffer.h"
#include <memory>
#include <vector>

namespace phasetwoemtf {

class AssociativeMemory {
public:
    // Constructor
    AssociativeMemory() : frozen_(false) {}

    // Destructor
    ~AssociativeMemory() {}

    // Functions
    void init(unsigned npatterns);

    // Insert patterns
    void insert(std::vector<superstrip_t>::const_iterator begin, std::vector<superstrip_t>::const_iterator end, const float invPt);
    void insert(const pattern_t& patt, const float invPt);

    void freeze();

    unsigned size() const { return patternBank_.size(); }

    // Perform direct pattern lookup, return a list of patterns that are fired
    std::vector<unsigned> lookup(const HitBuffer& hitBuffer, const unsigned nLayers, const unsigned maxMisses);

    // Retrieve superstripIds and attributes of a pattern
    void retrieve(const unsigned patternRef, pattern_t& superstripIds, float& invPt);

    // Debug
    std::string str() const;

private:
    // Member data
    std::vector<pattern_t> patternBank_;
    std::vector<float>     patternAttributes_invPt_;
    bool frozen_;
};

}  // namespace phasetwoemtf

#endif

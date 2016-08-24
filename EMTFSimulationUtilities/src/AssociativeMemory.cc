#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/AssociativeMemory.h"
#include <cassert>
#include <sstream>
#include <iostream>

namespace phasetwoemtf {

// _____________________________________________________________________________
void AssociativeMemory::init(unsigned npatterns) {
    patternBank_.clear();
    patternBank_.reserve(npatterns);

    patternAttributes_invPt_.clear();
    patternAttributes_invPt_.reserve(npatterns);
}

// _____________________________________________________________________________
void AssociativeMemory::insert(std::vector<superstrip_t>::const_iterator begin, std::vector<superstrip_t>::const_iterator end, const float invPt) {
    //patternBank_.insert(patternBank_.end(), begin, end);

    pattern_t patt;
    unsigned i = 0;
    for (std::vector<superstrip_t>::const_iterator it = begin; it != end; ++it, ++i) {
        patt.at(i) = *it;
    }
    patternBank_.push_back(patt);
    patternAttributes_invPt_.push_back(invPt);
}

void AssociativeMemory::insert(const pattern_t& patt, const float invPt) {
    patternBank_.push_back(patt);
    patternAttributes_invPt_.push_back(invPt);
}

// _____________________________________________________________________________
void AssociativeMemory::freeze() {
    assert(patternBank_.size() == patternAttributes_invPt_.size());
    frozen_ = true;
}

// _____________________________________________________________________________
std::vector<unsigned> AssociativeMemory::lookup(const HitBuffer& hitBuffer, const unsigned nLayers, const unsigned maxMisses) {
    std::vector<unsigned> firedPatterns;

    for (std::vector<pattern_t>::const_iterator itpatt = patternBank_.begin();
         itpatt != patternBank_.end(); ++itpatt) {
        unsigned nMisses = 0;

        pattern_t::const_reverse_iterator rbegin = itpatt->rend() - nLayers;
        pattern_t::const_reverse_iterator rend = itpatt->rend();
        for (pattern_t::const_reverse_iterator itlayer = rbegin; itlayer != rend; ++itlayer) {
            unsigned layer = (rend - 1) - itlayer;
            superstrip_t ssId = *itlayer;

            if (ssId == 0xffffffff)
                continue;

            if (!hitBuffer.hasHits(layer, ssId))
                ++nMisses;

            // Skip if more misses than allowed
            if (nMisses > maxMisses)
                break;
        }

        bool fired = (nMisses <= maxMisses);
        if (fired) {
            firedPatterns.push_back(itpatt - patternBank_.begin());
        }
    }
    return firedPatterns;
}

// _____________________________________________________________________________
void AssociativeMemory::retrieve(const unsigned patternRef, pattern_t& superstripIds, float& invPt) {
    superstripIds = patternBank_.at(patternRef);
    invPt = patternAttributes_invPt_.at(patternRef);
}

// _____________________________________________________________________________
std::string AssociativeMemory::str() const {
    std::ostringstream o;
    o << "npatterns: " << patternBank_.size();
    return o.str();
}

}  // namespace phasetwoemtf


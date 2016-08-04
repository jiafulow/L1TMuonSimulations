#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/PatternBankContainer.h"
#include <algorithm>
#include <cassert>


namespace {
bool sortByPopularity(const phasetwoemtf::pattern_pair& lhs, const phasetwoemtf::pattern_pair& rhs) {
    return lhs.second.n > rhs.second.n;
}
}  // namespace

namespace phasetwoemtf {

PatternBankContainer::PatternBankContainer()
: patternBankMap_(), patternBankInfo_(), patternBank_(), frozen_(false)
{
    unsigned guess_size = 1<<20;
    patternBankMap_.reserve(guess_size);
    patternBank_.reserve(guess_size);
}

void PatternBankContainer::fill_statistics(const float x, const unsigned long n, float& mean, float& variance) {
    //++n;
    mean += (x - mean)/n;
    if (n > 1)  variance += (x - mean)*(x - mean)/(n-1) - (variance/n);
}

void PatternBankContainer::fill_attribute(const float invPt, const float cotTheta, const float phi, const float z0, attrib_t& attr) {
    ++ attr.n;
    fill_statistics(invPt, attr.n, attr.invPt_mean, attr.invPt_variance);
    fill_statistics(cotTheta, attr.n, attr.cotTheta_mean, attr.cotTheta_variance);
    fill_statistics(phi, attr.n, attr.phi_mean, attr.phi_variance);
    fill_statistics(z0, attr.n, attr.z0_mean, attr.z0_variance);
}

void PatternBankContainer::fill(const pattern_pair& pair, float invPt, float cotTheta, float phi, float z0) {
    // Insert pattern into the map
    typedef std::pair<PatternBankMap::iterator, bool> patternBankMap_return_type;
    patternBankMap_return_type ins = patternBankMap_.insert(pair);

    // Update the pattern attribute
    fill_attribute(invPt, cotTheta, phi, z0, ins.first->second);
}

void PatternBankContainer::freeze() {
    // Convert map to vector of pairs
    const size_t origSize = patternBankMap_.size();

    for (PatternBankMap::const_iterator it = patternBankMap_.begin();
         it != patternBankMap_.end(); ) {  // should not cause bad_alloc
        patternBank_.push_back(*it);
        it = patternBankMap_.erase(it);
    }
    assert(patternBank_.size() == origSize);

    // Clear map and release memory
    patternBankMap_.clear();
    PatternBankMap emptyMap;
    emptyMap.swap(patternBankMap_);

    // Sort by popularity
    std::stable_sort(patternBank_.begin(), patternBank_.end(), sortByPopularity);

    frozen_ = true;
}

void PatternBankContainer::clear() {
    // Clear map and release memory
    patternBankMap_.clear();
    PatternBankMap emptyMap;
    emptyMap.swap(patternBankMap_);

    // Clear vector
    patternBank_.clear();
    PatternBank emptyVector;
    emptyVector.swap(patternBank_);
}

}  // namespace phasetwoemtf

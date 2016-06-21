#include "L1TMuonSimulations/EMTFSimulation/interface/PatternBankContainer.h"

#include <algorithm>
#include <cassert>


namespace {
    bool sortByPopularity(const pattern_pair& lhs, const pattern_pair& rhs) {
        return lhs.second.n > rhs.second.n;
    }
}

void PatternBankContainer::fill_statistics(const float x, const unsigned long n, float& mean, float& variance) {
    //++n;
    mean += (x - mean)/n;
    if (n > 1)  variance += (x - mean)*(x - mean)/(n-1) - (variance/n);
}

void PatternBankContainer::fill_attribute(const float invPt, const float cotTheta, const float phi, const float z0, pattern_attr& attr) {
    ++ attr.n;
    fill_statistics(invPt, attr.n, attr.invPt_mean, attr.invPt_variance);
    fill_statistics(cotTheta, attr.n, attr.cotTheta_mean, attr.cotTheta_variance);
    fill_statistics(phi, attr.n, attr.phi_mean, attr.phi_variance);
    fill_statistics(z0, attr.n, attr.z0_mean, attr.z0_variance);
}

void PatternBankContainer::fill(const pattern_pair& pair, float invPt, float cotTheta, float phi, float z0) {
    // Insert pattern into the map
    typedef std::pair<std::map<pattern_type, pattern_attr>::iterator, bool> map_return_type;
    map_return_type ins = patternBank_map_.insert(pair);

    // Update the pattern attribute
    fill_attribute(invPt, cotTheta, phi, z0, ins.first->second);
}

void PatternBankContainer::freeze() {
    // Convert map to vector of pairs
    const size_t origSize = patternBank_map_.size();

    for (std::map<pattern_type, pattern_attr>::const_iterator it = patternBank_map_.begin();
         it != patternBank_map_.end(); ) {  // should not cause bad_alloc
        patternBank_pairs_.push_back(*it);
        it = patternBank_map_.erase(it);
    }
    assert(patternBank_pairs_.size() == origSize);

    // Clear map and release memory
    std::map<pattern_type, pattern_attr> emptyMap;
    patternBank_map_.clear();
    patternBank_map_.swap(emptyMap);

    // Sort by popularity
    std::stable_sort(patternBank_pairs_.begin(), patternBank_pairs_.end(), sortByPopularity);

    frozen_ = true;
}

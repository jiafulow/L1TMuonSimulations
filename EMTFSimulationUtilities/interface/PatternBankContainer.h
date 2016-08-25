#ifndef EMTFSimulationUtilities_PatternBankContainer_h_
#define EMTFSimulationUtilities_PatternBankContainer_h_

#include "L1TMuonSimulations/EMTFSimulationDataFormats/interface/Pattern.h"
#include "L1TMuonSimulations/EMTFSimulationDataFormats/interface/PatternBank.h"
#include <map>
#include <unordered_map>


namespace phasetwoemtf {

class PatternBankContainer {
public:
    PatternBankContainer();
    ~PatternBankContainer() {}

    void fill(const pattern_pair& pair, float invPt, float cotTheta, float phi, float z0);

    void freeze(unsigned minPopularity=1, float maxPattInvPt=10.);

    void clear();

    size_t size() const { return (frozen_) ? getBank().size() : getMap().size(); }

    // See http://stackoverflow.com/questions/33965364/how-to-use-an-unordered-map-of-fixed-array-and-int
    struct container_hasher {
        template<typename T>
        std::size_t operator()(const T& c) const {
            std::size_t seed = 0;
            for (const auto& value : c) {
                std::hash<typename T::value_type> hasher;
                seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };

    typedef std::unordered_map<pattern_t, attrib_t, container_hasher> PatternBankMap;

    PatternBankMap&        getMap()       { return patternBankMap_; }
    const PatternBankMap&  getMap() const { return patternBankMap_; }

    PatternBankInfo&       getBankInfo()       { return patternBankInfo_; }
    const PatternBankInfo& getBankInfo() const { return patternBankInfo_; }

    PatternBank&           getBank()       { return patternBank_; }
    const PatternBank&     getBank() const { return patternBank_; }

private:
    void fill_statistics(const float x, const unsigned long n, float& mean, float& variance);

    void fill_attribute(const float invPt, const float cotTheta, const float phi, const float z0, attrib_t& attr);

private:
    PatternBankMap         patternBankMap_;
    PatternBankInfo        patternBankInfo_;
    PatternBank            patternBank_;
    bool frozen_;
};

}  // namespace phasetwoemtf

#endif

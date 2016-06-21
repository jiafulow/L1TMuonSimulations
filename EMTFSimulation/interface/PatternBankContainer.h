#ifndef EMTFSimulation_PatternBankContainer_h_
#define EMTFSimulation_PatternBankContainer_h_

#include "L1TMuonSimulations/EMTFSimulationDataFormats/interface/Pattern.h"
#include <map>
#include <vector>


class PatternBankInfo {
public:
    PatternBankInfo() : coverage(0.), count(0) {}
    ~PatternBankInfo() {}

    float coverage;
    unsigned long count;
    unsigned tower;
    unsigned sector;
    std::string superstrip;
    unsigned superstrip_nx;
    unsigned superstrip_nz;
};


class PatternBankContainer {
public:
    PatternBankContainer() : patternBank_map_(), patternBank_pairs_(), frozen_(false) {}
    ~PatternBankContainer() {}

    void fill(const pattern_pair& pair, float invPt, float cotTheta, float phi, float z0);

    void freeze();

    const std::map<pattern_type, pattern_attr>& getMap()   { return patternBank_map_; }
    const std::vector<pattern_pair>&            getPairs() { return patternBank_pairs_; }

private:
    void fill_statistics(const float x, const unsigned long n, float& mean, float& variance);

    void fill_attribute(const float invPt, const float cotTheta, const float phi, const float z0, pattern_attr& attr);

private:
    std::map<pattern_type, pattern_attr>   patternBank_map_;
    std::vector<pattern_pair>              patternBank_pairs_;
    bool frozen_;
};

#endif

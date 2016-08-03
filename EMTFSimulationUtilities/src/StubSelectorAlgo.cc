#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/StubSelectorAlgo.h"

#include "L1TMuonSimulations/EMTFSimulation/interface/Helper.h"


namespace {
    bool approximatelyEqual(float a, float b, float epsilon=1e-6) {
        return std::abs(a-b) < (std::abs(std::min(a,b)) * epsilon);
    }
}

void StubSelectorAlgo::reset() {
    const int nlayers = 5;

    residuals_.clear();
    indices_.clear();

    for (int i=0; i<nlayers; i++) {
        residuals_.push_back(std::make_pair(999999., 999999.));
        indices_.push_back(999999);
    }
}

void StubSelectorAlgo::select(unsigned istub, unsigned ichamber, unsigned isector, unsigned isubsector, float globalPhi, float globalTheta,
    const std::vector<float>& globalPhiME, const std::vector<float>& globalThetaME) {

    unsigned layME = decodeLayerME(ichamber);
    assert(layME <= 4);

    float residualPhi = std::abs(globalPhi - globalPhiME.at(layME));
    float residualTheta = std::abs(globalTheta - globalThetaME.at(layME));

    if (approximatelyEqual(residuals_.at(layME).first, residualPhi)) {
        // Compare theta residual if phi residual is about the same
        if (residuals_.at(layME).second > residualTheta) {
            residuals_.at(layME) = std::make_pair(residualPhi, residualTheta);
            indices_.at(layME) = istub;
        }

    } else {
        // Compare phi residual
        if (residuals_.at(layME).first > residualPhi) {
            residuals_.at(layME) = std::make_pair(residualPhi, residualTheta);
            indices_.at(layME) = istub;
        }
    }
}

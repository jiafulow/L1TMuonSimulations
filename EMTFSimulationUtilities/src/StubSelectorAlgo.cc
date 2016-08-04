#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/StubSelectorAlgo.h"
#include <cassert>


namespace {
bool approximatelyEqual(float a, float b, float epsilon=1e-6) {
    return std::abs(a-b) < (std::abs(std::min(a,b)) * epsilon);
}
}  // namespace

namespace phasetwoemtf {

std::vector<unsigned> StubSelectorAlgo::select(const std::vector<std::vector<unsigned> >& stubRefs,
        const std::vector<std::vector<unsigned> >& moduleIds,
        const std::vector<std::vector<float> >& globalPhiStubs,
        const std::vector<std::vector<float> >& globalThetaStubs,
        const std::vector<float>& globalPhiME,
        const std::vector<float>& globalThetaME)
{
    // Check vector sizes
    unsigned nlayers = stubRefs.size();
    assert(moduleIds.size() == nlayers && globalPhiStubs.size() == nlayers && globalThetaStubs.size() == nlayers && globalPhiME.size() == nlayers && globalThetaME.size() == nlayers);
    for (unsigned i=0; i<nlayers; i++) {
        unsigned nstubs = stubRefs.at(i).size();
        assert(moduleIds.at(i).size() == nstubs && globalPhiStubs.at(i).size() == nstubs && globalThetaStubs.at(i).size() == nstubs);
    }

    // Initialize
    residuals_.clear();
    stubRefs_.clear();

    for (unsigned i=0; i<nlayers; i++) {
        residuals_.push_back(std::make_pair(999999., 999999.));
        stubRefs_.push_back(999999);
    }

    // Now find residuals and select the minimum-residual stubs
    for (unsigned i=0; i<nlayers; i++) {
        unsigned nstubs = stubRefs.at(i).size();

        for (unsigned j=0; j<nstubs; j++) {
            unsigned stubRef = stubRefs.at(i).at(j);
            unsigned moduleId = moduleIds.at(i).at(j);
            float globalPhi = globalPhiStubs.at(i).at(j);
            float globalTheta = globalThetaStubs.at(i).at(j);

            float residualPhi = std::abs(globalPhi - globalPhiME.at(i));
            float residualTheta = std::abs(globalTheta - globalThetaME.at(i));

            if (approximatelyEqual(residuals_.at(i).first, residualPhi)) {
                // Compare theta residual if phi residual is approx the same
                if (residuals_.at(i).second > residualTheta) {
                    residuals_.at(i) = std::make_pair(residualPhi, residualTheta);
                    stubRefs_.at(i) = stubRef;
                }

            } else {
                // Compare phi residual
                if (residuals_.at(i).first > residualPhi) {
                    residuals_.at(i) = std::make_pair(residualPhi, residualTheta);
                    stubRefs_.at(i) = stubRef;
                }
            }

        }  // end loop over stubs
    }  // end loop over layers

    return stubRefs_;
}

}  // namespace phasetwoemtf

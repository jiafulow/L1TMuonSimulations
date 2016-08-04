#ifndef EMTFSimulationUtilities_StubSelectorAlgo_h_
#define EMTFSimulationUtilities_StubSelectorAlgo_h_

#include <cmath>
#include <vector>


namespace phasetwoemtf {

class StubSelectorAlgo {
public:
    StubSelectorAlgo() {}
    ~StubSelectorAlgo() {}

    std::vector<unsigned> select(const std::vector<std::vector<unsigned> >& stubRefs,
            const std::vector<std::vector<unsigned> >& moduleIds,
            const std::vector<std::vector<float> >& globalPhiStubs,
            const std::vector<std::vector<float> >& globalThetaStubs,
            const std::vector<float>& globalPhiME,
            const std::vector<float>& globalThetaME);

private:
    // residuals[ilayer][0] -> min absolute residual in phi at layer i
    // residuals[ilayer][1] -> min absolute residual in theta at layer i
    std::vector<std::pair<float, float> > residuals_;

    // stubRefs[ilayer] -> index of the stub with min residual at layer i
    std::vector<unsigned> stubRefs_;
};

}  // namespace phasetwoemtf

#endif

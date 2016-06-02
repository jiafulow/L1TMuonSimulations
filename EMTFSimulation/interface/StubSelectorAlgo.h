#ifndef EMTFSimulation_StubSelectorAlgo_h_
#define EMTFSimulation_StubSelectorAlgo_h_

#include <vector>

class StubSelectorAlgo {
  public:
    StubSelectorAlgo() {}
    ~StubSelectorAlgo() {}

    void reset();

    void select(unsigned istub, unsigned ichamber, unsigned isector, unsigned isubsector, float globalPhi, float globalTheta,
        const std::vector<float>& globalPhiME, const std::vector<float>& globalThetaME);

    std::vector<unsigned> get_indices() const { return indices_; }

  private:
    // residuals[ilayer][0] -> min residual in phi at layer i
    // residuals[ilayer][1] -> min residual in theta at layer i
    std::vector<std::pair<float, float> > residuals_;

    // indices[ilayer] -> index of the stub with min residual at layer i
    std::vector<unsigned> indices_;
};

#endif

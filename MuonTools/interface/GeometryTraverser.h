#ifndef MuonTools_GeometryTraverser_h_
#define MuonTools_GeometryTraverser_h_

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/TrajectoryState/interface/TrackCharge.h"
#include <memory>
#include <vector>


// forwards
namespace edm {
  class EventSetup;
}

class MagneticField;
class Propagator;
class MuonDetLayerGeometry;


class GeometryTraverser {
  public:
    GeometryTraverser();
    ~GeometryTraverser();

    void setFTS(const GlobalPoint& v, const GlobalVector& m, const TrackCharge& q) {
        _vertex = v;
        _momentum = m;
        _charge = q;
    }

    GlobalPoint propagateBarrel(double r) const;
    GlobalPoint propagateEndcap(double z) const;

    std::vector<GlobalPoint> propagateBarrels(const std::vector<double>& vec_r) const;
    std::vector<GlobalPoint> propagateEndcaps(const std::vector<double>& vec_z) const;

    void checkAndUpdateGeometry(const edm::EventSetup&);

    const MagneticField* magneticField() const { return _magfield.product(); }

    const Propagator* propagator() const { return _propagator.product(); }

    const MuonDetLayerGeometry* muonGeometry() const { return _muongeom.product(); }

  private:
    // pointers to the current geometry records
    unsigned long long _geom_cache_id;
    edm::ESHandle<MagneticField>        _magfield;
    edm::ESHandle<Propagator>           _propagator;
    edm::ESHandle<MuonDetLayerGeometry> _muongeom;

    GlobalPoint   _vertex;
    GlobalVector  _momentum;
    TrackCharge   _charge;

};

#endif

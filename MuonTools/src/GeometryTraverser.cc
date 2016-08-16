#include "L1TMuonSimulations/MuonTools/interface/GeometryTraverser.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "RecoMuon/DetLayers/interface/MuonDetLayerGeometry.h"
#include "DataFormats/GeometrySurface/interface/BoundCylinder.h"
#include "DataFormats/GeometrySurface/interface/BoundDisk.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "RecoMuon/Records/interface/MuonRecoGeometryRecord.h"
#include "TrackingTools/DetLayers/interface/DetLayer.h"

#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"


GeometryTraverser::GeometryTraverser():
  _geom_cache_id(0ULL),
  _vertex(0.,0.,0.), _momentum(0.,0.,0.), _charge(-1) {
}

GeometryTraverser::~GeometryTraverser() {}

void GeometryTraverser::checkAndUpdateGeometry(const edm::EventSetup& es) {
  const MuonRecoGeometryRecord& geom = es.get<MuonRecoGeometryRecord>();
  unsigned long long geomid = geom.cacheIdentifier();
  if( _geom_cache_id != geomid ) {
    geom.get(_muongeom);
    _geom_cache_id = geomid;

    es.get<IdealMagneticFieldRecord>().get(_magfield);
    es.get<TrackingComponentsRecord>().get("SteppingHelixPropagatorAlong", _propagator);
  }
}

std::pair<GlobalPoint, GlobalVector> GeometryTraverser::propagateBarrel(double r) const {
    std::vector<double> vec_r;
    vec_r.push_back(r);
    const std::vector<std::pair<GlobalPoint, GlobalVector> >& res = propagateBarrels(vec_r);
    return res.front();
}

std::pair<GlobalPoint, GlobalVector> GeometryTraverser::propagateEndcap(double z) const {
    std::vector<double> vec_z;
    vec_z.push_back(z);
    const std::vector<std::pair<GlobalPoint, GlobalVector> >& res = propagateEndcaps(vec_z);
    return res.front();
}

std::vector<std::pair<GlobalPoint, GlobalVector> > GeometryTraverser::propagateBarrels(const std::vector<double>& vec_r) const {
    FreeTrajectoryState ftsAtProduction(_vertex, _momentum, _charge, magneticField());
    const Propagator * propagatorBarrel = &*(_propagator.product());

    std::vector<BoundCylinder::CylinderPointer> layersBarrelCylinderPointer;
    std::vector<const BoundCylinder *> layersBarrel;
    for (unsigned i=0; i<vec_r.size(); i++) {
        double radius = vec_r.at(i);
        BoundCylinder::PositionType pos;
        BoundCylinder::RotationType rot;
        SimpleCylinderBounds bounds(radius-29.1677, radius+29.1677, -659.1, 659.1);  // numbers from MB2
        BoundCylinder::CylinderPointer cylinder(new Cylinder(pos, rot, bounds));
        layersBarrelCylinderPointer.push_back(cylinder);
        //const BoundCylinder * surface = new BoundCylinder(pos, rot, bounds);
        const BoundCylinder * surface = dynamic_cast<const BoundCylinder *>(&*cylinder);
        layersBarrel.push_back(surface);
    }

    // see MuonAnalysis/MuonAssociators/src/PropagateToMuon.cc
    TrajectoryStateOnSurface tsosBarrel;
    bool isValidBarrel = true;
    std::vector<std::pair<GlobalPoint, GlobalVector> > propagatedBarrel;

    for (unsigned i=0; i<layersBarrel.size(); i++) {
        if (i == 0) {
            tsosBarrel = propagatorBarrel->propagate(ftsAtProduction, *layersBarrel.at(i));
        } else if (isValidBarrel) {  // if the previous tsos is valid
            tsosBarrel = propagatorBarrel->propagate(tsosBarrel, *layersBarrel.at(i));
        }

        isValidBarrel = tsosBarrel.isValid() &&
           fabs(tsosBarrel.globalPosition().z()) <= layersBarrel.at(i)->bounds().length()/2.0;
        if (isValidBarrel) {
            propagatedBarrel.push_back(std::make_pair(tsosBarrel.globalPosition(), tsosBarrel.globalMomentum()) );
        } else {
            propagatedBarrel.push_back(std::make_pair(GlobalPoint(0., 0., 0.), GlobalVector(0., 0., 0.)) );
        }
    }
    return propagatedBarrel;
}

std::vector<std::pair<GlobalPoint, GlobalVector> > GeometryTraverser::propagateEndcaps(const std::vector<double>& vec_z) const {
    FreeTrajectoryState ftsAtProduction(_vertex, _momentum, _charge, _magfield.product());
    const Propagator * propagatorEndcap = &*(_propagator.product());

    std::vector<BoundDisk::DiskPointer> layersEndcapDiskPointer;
    std::vector<const BoundDisk *> layersEndcap;
    for (unsigned i=0; i<vec_z.size(); i++) {
        double zPos = vec_z.at(i);
        BoundDisk::PositionType pos(0.,0.,zPos);
        BoundDisk::RotationType rot;
        SimpleDiskBounds bounds(0., 699.773, -21.8752, 21.8753);  // numbers from ME2
        BoundDisk::DiskPointer disk(new Disk(pos, rot, bounds));
        layersEndcapDiskPointer.push_back(disk);
        //const BoundDisk * surface = new BoundDisk(pos, rot, bounds);
        const BoundDisk * surface = dynamic_cast<const BoundDisk *>(&*disk);
        layersEndcap.push_back(surface);
    }

    // see MuonAnalysis/MuonAssociators/src/PropagateToMuon.cc
    TrajectoryStateOnSurface tsosEndcap;
    bool isValidEndcap = true;
    std::vector<std::pair<GlobalPoint, GlobalVector> > propagatedEndcap;

    for (unsigned i=0; i<layersEndcap.size(); i++) {
        if (i == 0) {
            tsosEndcap = propagatorEndcap->propagate(ftsAtProduction, *layersEndcap.at(i));
        } else if (isValidEndcap) {  // if the previous tsos is valid
            tsosEndcap = propagatorEndcap->propagate(tsosEndcap, *layersEndcap.at(i));
        }

        isValidEndcap = tsosEndcap.isValid() &&
           tsosEndcap.globalPosition().perp() >= layersEndcap.at(i)->innerRadius() &&
           tsosEndcap.globalPosition().perp() <= layersEndcap.at(i)->outerRadius();
        if (isValidEndcap) {
            propagatedEndcap.push_back(std::make_pair(tsosEndcap.globalPosition(), tsosEndcap.globalMomentum()) );
        } else {
            propagatedEndcap.push_back(std::make_pair(GlobalPoint(0., 0., 0.), GlobalVector(0., 0., 0.)) );
        }
    }
    return propagatedEndcap;
}

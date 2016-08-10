#ifndef EMTFSimulationUtilities_SuperstripArbiter_h_
#define EMTFSimulationUtilities_SuperstripArbiter_h_

#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/SuperstripShapes.h"


namespace phasetwoemtf {

class SuperstripArbiter {
public:
    SuperstripArbiter();
    ~SuperstripArbiter();

    // Superstrip using local coordinates
    unsigned superstripLocal(unsigned moduleId, float strip, float segment, float ds) const;

    // Superstrip using global coordinates
    unsigned superstripGlobal(unsigned moduleId, float rho, float phi, float theta, float bend) const;

    void setDefinition(const std::string& definition);

    std::string getDefinition() const { return definition_; }

    SuperstripCoordType getCoordType() const { return coordType_; }

    SuperstripShapeType getShapeType() const { return shapeType_; }

    unsigned nx() const { return nx_; }

    unsigned nz() const { return nz_; }

    unsigned nss() const { return nss_; }
    unsigned nsuperstripsPerLayer() const { return nss(); }

    // Debug
    std::string str() const;

private:
    std::string         definition_;
    SuperstripCoordType coordType_;
    SuperstripShapeType shapeType_;
    unsigned            nx_;
    unsigned            nz_;
    unsigned            nss_;

    std::vector<SuperstripLocalParams>  localParams_;
    std::vector<SuperstripGlobalParams> globalParams_;
};

}  // namespace phasetwoemtf

#endif

#ifndef EMTFSimulationUtilities_SuperstripShapes_h_
#define EMTFSimulationUtilities_SuperstripShapes_h_

#include <cmath>
#include <string>
#include <vector>


namespace phasetwoemtf {

enum SuperstripCoordType { LOCAL, GLOBAL, LOCALTOGLOBAL };

enum SuperstripShapeType { UNKNOWN, FIXEDWIDTH, PROJECTIVE, FOUNTAIN };

struct SuperstripLocalParams {
    unsigned right_shift_x;
    unsigned mask_x;
    unsigned right_shift_z;
    unsigned mask_z;
    unsigned shift_z;
    unsigned mask_m;
    unsigned shift_m;
};

struct SuperstripGlobalParams {
    float low_x;
    float delta_x;
    unsigned mask_x;
    float low_z;
    float delta_z;
    unsigned mask_z;
    unsigned shift_z;
};

// _____________________________________________________________________________
class SuperstripShapeBase {
public:
    SuperstripShapeBase() {
        coordType_ = SuperstripCoordType::LOCAL;
        shapeType_ = SuperstripShapeType::UNKNOWN;
    }
    ~SuperstripShapeBase() {}

    virtual void setDefinition(const std::string& definition, SuperstripCoordType& coord, SuperstripShapeType& shape,
            unsigned& nx, unsigned& nz, unsigned& nss, std::vector<SuperstripLocalParams>& params) const = 0;
    virtual void setDefinition(const std::string& definition, SuperstripCoordType& coord, SuperstripShapeType& shape,
            unsigned& nx, unsigned& nz, unsigned& nss, std::vector<SuperstripGlobalParams>& params) const = 0;

protected:
    SuperstripCoordType coordType_;
    SuperstripShapeType shapeType_;
};

// _____________________________________________________________________________
class SuperstripFixedWidth : public SuperstripShapeBase {
public:
    SuperstripFixedWidth() : SuperstripShapeBase() {
        coordType_ = SuperstripCoordType::LOCAL;
        shapeType_ = SuperstripShapeType::FIXEDWIDTH;
    }

    void setDefinition(const std::string& definition, SuperstripCoordType& coord, SuperstripShapeType& shape,
            unsigned& nx, unsigned& nz, unsigned& nss, std::vector<SuperstripLocalParams>& params) const;
    void setDefinition(const std::string& definition, SuperstripCoordType& coord, SuperstripShapeType& shape,
            unsigned& nx, unsigned& nz, unsigned& nss, std::vector<SuperstripGlobalParams>& params) const;
};

// _____________________________________________________________________________
class SuperstripProjective : public SuperstripShapeBase {
public:
    SuperstripProjective() : SuperstripShapeBase() {
        coordType_ = SuperstripCoordType::GLOBAL;
        shapeType_ = SuperstripShapeType::PROJECTIVE;
    }

    void setDefinition(const std::string& definition, SuperstripCoordType& coord, SuperstripShapeType& shape,
            unsigned& nx, unsigned& nz, unsigned& nss, std::vector<SuperstripLocalParams>& params) const;
    void setDefinition(const std::string& definition, SuperstripCoordType& coord, SuperstripShapeType& shape,
            unsigned& nx, unsigned& nz, unsigned& nss, std::vector<SuperstripGlobalParams>& params) const;
};

// _____________________________________________________________________________
// Output streams
std::ostream& operator<<(std::ostream& o, const SuperstripLocalParams& p);
std::ostream& operator<<(std::ostream& o, const SuperstripGlobalParams& p);

}  // namespace phasetwoemtf

#endif

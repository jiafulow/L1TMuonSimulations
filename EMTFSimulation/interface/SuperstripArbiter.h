#ifndef EMTFSimulation_SuperstripArbiter_h_
#define EMTFSimulation_SuperstripArbiter_h_

#include "TString.h"
#include <vector>

enum SuperstripType {UNKNOWN, FIXEDWIDTH, PROJECTIVE, FOUNTAIN, OPTIMIZED};

class SuperstripArbiter {
  public:
    SuperstripArbiter();
    ~SuperstripArbiter();

    // Superstrip using local coordinates
    unsigned superstripLocal(unsigned moduleId, float strip, float segment, float ds) const;

    // Superstrip using global coordinates
    unsigned superstripGlobal(unsigned moduleId, float rho, float phi, float theta, float bend) const;

    void setDefinition(TString definition);

    unsigned nx() const { return nx_; }

    unsigned nz() const { return nz_; }

    bool useGlobalCoord() const { return useGlobalCoord_; }

  private:
    SuperstripType     sstype_;
    unsigned           nx_;
    unsigned           nz_;
    bool               useGlobalCoord_;
};

#endif

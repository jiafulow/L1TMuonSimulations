#ifndef FlatRandomPzGunProducer2_H
#define FlatRandomPzGunProducer2_H

/** \class FlatRandomPzGunProducer2
 *
 * Generates single particle gun in HepMC format
 * Julia Yarba 12/2005
 ***************************************/

#include "L1TMuonSimulations/ParticleGuns/interface/BaseFlatGunProducer2.h"

namespace edm
{

  class FlatRandomPzGunProducer2 : public BaseFlatGunProducer2
  {

  public:
    FlatRandomPzGunProducer2(const ParameterSet & pset);
    virtual ~FlatRandomPzGunProducer2();

    virtual void produce(Event & e, const EventSetup& es) override;

  private:

    // data members

    double            fMinPz   ;
    double            fMaxPz   ;
    double            fMinOneOverPz   ;
    double            fMaxOneOverPz   ;
    double            fXFlatSpread    ;
    double            fYFlatSpread    ;
    double            fZFlatSpread    ;
    double            fRStarForPhi    ;
    double            fRStarForTheta  ;
    bool              fRandomCharge   ;
    std::string       fPzSpectrum     ;

  };
}

#endif

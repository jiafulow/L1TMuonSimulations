#include "FWCore/Framework/interface/MakerMacros.h"
// #include "IOMC/Input/interface/MCFileSource.h"

// Julia Yarba : related to particle gun prototypes
//

#include "L1TMuonSimulations/ParticleGuns/interface/FlatRandomPtGunProducer2.h"
#include "L1TMuonSimulations/ParticleGuns/interface/FlatRandomPzGunProducer2.h"

// particle gun prototypes
//

using edm::FlatRandomPtGunProducer2;
DEFINE_FWK_MODULE(FlatRandomPtGunProducer2);
using edm::FlatRandomPzGunProducer2;
DEFINE_FWK_MODULE(FlatRandomPzGunProducer2);

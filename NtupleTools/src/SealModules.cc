#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "L1TMuonSimulations/NtupleTools/interface/NtupleGenParticles.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleGenEventInfo.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleEventInfo.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleMaker.h"

DEFINE_FWK_MODULE(NtupleGenParticles);
DEFINE_FWK_MODULE(NtupleGenEventInfo);
DEFINE_FWK_MODULE(NtupleEventInfo);
DEFINE_FWK_MODULE(NtupleMaker);


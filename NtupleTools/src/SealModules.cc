#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "L1TMuonSimulations/NtupleTools/interface/NtupleBeamSpot.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleEventInfo.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleGenParticles.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleGenParticlesToMuon.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleGenJets.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleGenMET.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleGenEventInfo.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleSimTracks.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleSimVertices.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleSimHits.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleTrackingParticles.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleTrackingVertices.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleMixedSimHits.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleCSCTriggerPrimitives.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleCSCRegionalCandidates.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleRPCTriggerPrimitives.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleDTTriggerPrimitives.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleMaker.h"

DEFINE_FWK_MODULE(NtupleBeamSpot);
DEFINE_FWK_MODULE(NtupleEventInfo);
DEFINE_FWK_MODULE(NtupleGenParticles);
DEFINE_FWK_MODULE(NtupleGenParticlesToMuon);
DEFINE_FWK_MODULE(NtupleGenJets);
DEFINE_FWK_MODULE(NtupleGenMET);
DEFINE_FWK_MODULE(NtupleGenEventInfo);
DEFINE_FWK_MODULE(NtupleSimTracks);
DEFINE_FWK_MODULE(NtupleSimVertices);
DEFINE_FWK_MODULE(NtupleSimHits);
DEFINE_FWK_MODULE(NtupleTrackingParticles);
DEFINE_FWK_MODULE(NtupleTrackingVertices);
DEFINE_FWK_MODULE(NtupleMixedSimHits);
DEFINE_FWK_MODULE(NtupleCSCTriggerPrimitives);
DEFINE_FWK_MODULE(NtupleCSCRegionalCandidates);
DEFINE_FWK_MODULE(NtupleRPCTriggerPrimitives);
DEFINE_FWK_MODULE(NtupleDTTriggerPrimitives);
DEFINE_FWK_MODULE(NtupleMaker);


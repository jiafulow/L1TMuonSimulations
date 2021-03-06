#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

// Event info
#include "L1TMuonSimulations/NtupleTools/interface/NtupleEventInfo.h"
DEFINE_FWK_MODULE(NtupleEventInfo);

// Gen
#include "L1TMuonSimulations/NtupleTools/interface/NtupleGenParticles.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleGenJets.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleGenMET.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleBeamSpot.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleGenEventInfo.h"
DEFINE_FWK_MODULE(NtupleGenParticles);
DEFINE_FWK_MODULE(NtupleGenJets);
DEFINE_FWK_MODULE(NtupleGenMET);
DEFINE_FWK_MODULE(NtupleBeamSpot);
DEFINE_FWK_MODULE(NtupleGenEventInfo);

// GenExtra
#include "L1TMuonSimulations/NtupleTools/interface/NtupleGenParticlesToMuon.h"
DEFINE_FWK_MODULE(NtupleGenParticlesToMuon);

// Sim
#include "L1TMuonSimulations/NtupleTools/interface/NtupleSimTracks.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleSimVertices.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleSimHits.h"
DEFINE_FWK_MODULE(NtupleSimTracks);
DEFINE_FWK_MODULE(NtupleSimVertices);
DEFINE_FWK_MODULE(NtupleSimHits);

// Digi
#include "L1TMuonSimulations/NtupleTools/interface/NtupleTrackingParticles.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleTrackingVertices.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleMixedSimHits.h"
DEFINE_FWK_MODULE(NtupleTrackingParticles);
DEFINE_FWK_MODULE(NtupleTrackingVertices);
DEFINE_FWK_MODULE(NtupleMixedSimHits);

// HLT
#include "L1TMuonSimulations/NtupleTools/interface/NtupleTriggerResults.h"
DEFINE_FWK_MODULE(NtupleTriggerResults);

// Reco
#include "L1TMuonSimulations/NtupleTools/interface/NtupleRecoTracks.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleRecoVertices.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleRecoPFJets.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleRecoPFMET.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleRecoMuons.h"
DEFINE_FWK_MODULE(NtupleRecoTracks);
DEFINE_FWK_MODULE(NtupleRecoVertices);
DEFINE_FWK_MODULE(NtupleRecoPFJets);
DEFINE_FWK_MODULE(NtupleRecoPFMET);
DEFINE_FWK_MODULE(NtupleRecoMuons);

// L1T
#include "L1TMuonSimulations/NtupleTools/interface/NtupleCSCTriggerPrimitives.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleRPCTriggerPrimitives.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleDTTriggerPrimitives.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleLegacyCSCTFTriggerTracks.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleEMTFTriggerPrimitives.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleEMTFTriggerTracks.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleEMTFRegionalCandidates.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleL1TMuons.h"
DEFINE_FWK_MODULE(NtupleCSCTriggerPrimitives);
DEFINE_FWK_MODULE(NtupleRPCTriggerPrimitives);
DEFINE_FWK_MODULE(NtupleDTTriggerPrimitives);
DEFINE_FWK_MODULE(NtupleLegacyCSCTFTriggerTracks);
DEFINE_FWK_MODULE(NtupleEMTFTriggerPrimitives);
DEFINE_FWK_MODULE(NtupleEMTFTriggerTracks);
DEFINE_FWK_MODULE(NtupleEMTFRegionalCandidates);
DEFINE_FWK_MODULE(NtupleL1TMuons);

// L1TExtra
#include "L1TMuonSimulations/NtupleTools/interface/NtupleEMUTFTriggerPrimitives.h"
#include "L1TMuonSimulations/NtupleTools/interface/NtupleEMUTFTriggerTracks.h"
DEFINE_FWK_MODULE(NtupleEMUTFTriggerPrimitives);
DEFINE_FWK_MODULE(NtupleEMUTFTriggerTracks);

// NtupleMaker
#include "L1TMuonSimulations/NtupleTools/interface/NtupleMaker.h"
DEFINE_FWK_MODULE(NtupleMaker);


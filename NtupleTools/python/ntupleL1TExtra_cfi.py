import FWCore.ParameterSet.Config as cms

ntupleEMUTFTriggerPrimitives = cms.EDProducer('NtupleEMUTFTriggerPrimitives',
    unpStubTag = cms.InputTag('emtfStage2Digis'),
    unpPrefix = cms.string('EMUTFStubs@'),
    unpSuffix = cms.string(''),
    unpCut = cms.string(''),
    emuStubTag = cms.InputTag('simEmtfDigisMC'),
    emuPrefix = cms.string('EMUTFStubExtras@'),
    emuSuffix = cms.string(''),
    emuCut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleEMUTFTriggerTracks = cms.EDProducer('NtupleEMUTFTriggerTracks',
    unpStubTag = cms.InputTag('emtfStag2Digis'),
    unpTrackTag = cms.InputTag('emtfStage2Digis'),
    unpPrefix = cms.string('EMUTFTracks@'),
    unpSuffix = cms.string(''),
    unpCut = cms.string(''),
    emuStubTag = cms.InputTag('simEmtfDigisMC'),
    emuTrackTag = cms.InputTag('simEmtfDigisMC'),
    emuPrefix = cms.string('EMUTFTrackExtras@'),
    emuSuffix = cms.string(''),
    emuCut = cms.string(''),
    maxN = cms.uint32(999999)
)

from L1TriggerSep2016.L1TMuonEndCap.simEmtfDigis_cfi import simEmtfDigisMC

#ntupleL1TExtra = cms.Sequence(ntupleEMUTFTriggerPrimitives * ntupleEMUTFTriggerTracks)
ntupleL1TExtra = cms.Sequence(simEmtfDigisMC * ntupleEMUTFTriggerPrimitives * ntupleEMUTFTriggerTracks)


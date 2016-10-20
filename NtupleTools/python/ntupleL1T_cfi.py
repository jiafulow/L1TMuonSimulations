import FWCore.ParameterSet.Config as cms

ntupleCSCTriggerPrimitives = cms.EDProducer('NtupleCSCTriggerPrimitives',
    corrlctTag = cms.InputTag('simCscTriggerPrimitiveDigis','MPCSORTED'),
    prefix = cms.string('CSCStubs@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleRPCTriggerPrimitives = cms.EDProducer('NtupleRPCTriggerPrimitives',
    inputTag = cms.InputTag('simMuonRPCDigis'),
    prefix = cms.string('RPCStubs@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleDTTriggerPrimitives = cms.EDProducer('NtupleDTTriggerPrimitives',
    dtPhTag = cms.InputTag('simDtTriggerPrimitiveDigis'),
    dtThTag = cms.InputTag('simDtTriggerPrimitiveDigis'),
    prefix = cms.string('DTStubs@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleLegacyCSCTFTriggerTracks = cms.EDProducer('NtupleLegacyCSCTFTriggerTracks',
    stubTag = cms.InputTag('csctfDigis'),
    trackTag = cms.InputTag('csctfDigis'),
    prefix = cms.string('LegCSCTFTracks@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleEMTFTriggerPrimitives = cms.EDProducer('NtupleEMTFTriggerPrimitives',
    unpStubTag = cms.InputTag('emtfStage2Digis'),
    unpPrefix = cms.string('EMTFStubs@'),
    unpSuffix = cms.string(''),
    unpCut = cms.string(''),
    emuStubTag = cms.InputTag('simEmtfDigis','EMTF'),
    emuPrefix = cms.string('EMTFStubExtras@'),
    emuSuffix = cms.string(''),
    emuCut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleEMTFTriggerTracks = cms.EDProducer('NtupleEMTFTriggerTracks',
    unpStubTag = cms.InputTag('emtfStage2Digis'),
    unpTrackTag = cms.InputTag('emtfStage2Digis'),
    unpPrefix = cms.string('EMTFTracks@'),
    unpSuffix = cms.string(''),
    unpCut = cms.string(''),
    emuStubTag = cms.InputTag('simEmtfDigis','EMTF'),
    emuTrackTag = cms.InputTag('simEmtfDigis','EMTF'),
    emuPrefix = cms.string('EMTFTrackExtras@'),
    emuSuffix = cms.string(''),
    emuCut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleEMTFRegionalCandidates = cms.EDProducer('NtupleEMTFRegionalCandidates',
    trackTag = cms.InputTag("simEmtfDigis","EMTF"),
    prefix = cms.string('EMTFCands@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleL1TMuons = cms.EDProducer('NtupleL1TMuons',
    trackTag = cms.InputTag("simGmtStage2Digis"),
    prefix = cms.string('l1tMuons@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)


ntupleL1T = cms.Sequence(ntupleCSCTriggerPrimitives * ntupleRPCTriggerPrimitives * ntupleDTTriggerPrimitives * ntupleLegacyCSCTFTriggerTracks * ntupleEMTFTriggerPrimitives * ntupleEMTFTriggerTracks * ntupleEMTFRegionalCandidates * ntupleL1TMuons)


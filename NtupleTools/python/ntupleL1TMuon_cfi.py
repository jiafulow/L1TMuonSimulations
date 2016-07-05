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
    chambPhTag = cms.InputTag('simDtTriggerPrimitiveDigis'),
    chambThTag = cms.InputTag('simDtTriggerPrimitiveDigis'),
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
    stubTag = cms.InputTag('simEmtfDigis','EMTF'),
    prefix = cms.string('EMTFStubs@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleEMTFTriggerTracks = cms.EDProducer('NtupleEMTFTriggerTracks',
    stubTag = cms.InputTag('simEmtfDigis','EMTF'),
    trackTag = cms.InputTag('simEmtfDigis','EMTF'),
    prefix = cms.string('EMTFTracks@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleEMTFRegionalCandidates = cms.EDProducer('NtupleEMTFRegionalCandidates',
    trackTag = cms.InputTag("simEmtfDigis","EMTF"),
    prefix = cms.string('EMTFCands@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleUnpackedEMTFTriggerPrimitives = cms.EDProducer('NtupleUnpackedEMTFTriggerPrimitives',
    stubTag = cms.InputTag('emtfStage2Digis'),
    prefix = cms.string('UnpEMTFStubs@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleUnpackedEMTFTriggerTracks = cms.EDProducer('NtupleUnpackedEMTFTriggerTracks',
    stubTag = cms.InputTag('emtfStage2Digis'),
    trackTag = cms.InputTag('emtfStage2Digis'),
    prefix = cms.string('UnpEMTFTracks@'),
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


ntupleL1TMuon = cms.Sequence(ntupleCSCTriggerPrimitives * ntupleRPCTriggerPrimitives * ntupleDTTriggerPrimitives * ntupleLegacyCSCTFTriggerTracks * ntupleEMTFTriggerPrimitives * ntupleEMTFTriggerTracks * ntupleEMTFRegionalCandidates * ntupleL1TMuons)
ntupleL1TMuonUnpacker = cms.Sequence(ntupleUnpackedEMTFTriggerPrimitives * ntupleUnpackedEMTFTriggerTracks)


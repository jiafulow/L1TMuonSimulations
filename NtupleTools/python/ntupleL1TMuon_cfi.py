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

ntupleL1TMuon = cms.Sequence(ntupleCSCTriggerPrimitives * ntupleRPCTriggerPrimitives * ntupleDTTriggerPrimitives)

import FWCore.ParameterSet.Config as cms

ntupleCSCTriggerPrimitives = cms.EDProducer('NtupleCSCTriggerPrimitives',
    inputTag = cms.InputTag('simCscTriggerPrimitiveDigis','MPCSORTED'),
    prefix = cms.string('csctf@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleL1TMuon = cms.Sequence(ntupleCSCTriggerPrimitives)

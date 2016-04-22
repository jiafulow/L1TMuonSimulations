import FWCore.ParameterSet.Config as cms

ntupleCSCTriggerPrimitives = cms.EDProducer('NtupleCSCTriggerPrimitives',
    corrlctTag = cms.InputTag('simCscTriggerPrimitiveDigis','MPCSORTED'),
    prefix = cms.string('CSCStubs@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleCSCRegionalCandidates = cms.EDProducer('NtupleCSCRegionalCandidates',
    corrlctTag = cms.InputTag('simCscTriggerPrimitiveDigis','MPCSORTED'),
    trackTag = cms.InputTag("simEmtfDigis","EMTF"),
    prefix = cms.string('CSCTracks@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleL1TMuon = cms.Sequence(ntupleCSCTriggerPrimitives * ntupleCSCRegionalCandidates)

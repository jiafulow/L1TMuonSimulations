import FWCore.ParameterSet.Config as cms

ntupleTrackingParticles = cms.EDProducer('NtupleTrackingParticles',
    inputTag = cms.InputTag('mix', 'MergedTrackTruth'),
    prefix = cms.string('trkParts@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleTrackingVertices = cms.EDProducer('NtupleTrackingVertices',
    inputTag = cms.InputTag('mix', 'MergedTrackTruth'),
    prefix = cms.string('trkVertices@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleDigi = cms.Sequence(ntupleTrackingParticles * ntupleTrackingVertices)

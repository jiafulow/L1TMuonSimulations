import FWCore.ParameterSet.Config as cms

ntupleGenParticles = cms.EDProducer('NtupleGenParticles',
    #inputTag = cms.InputTag('prunedGenParticles'),
    inputTag = cms.InputTag('genParticles'),
    prefix = cms.string('genParts@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleGenEventInfo = cms.EDProducer('NtupleGenEventInfo',
    genEventInfo = cms.InputTag('generator'),
    pileupInfo = cms.InputTag('addPileupInfo'),
    pileupWeight = cms.InputTag(''),
    pdfWeight = cms.InputTag(''),
    randomSeed = cms.InputTag('randomEngineSeedKeeper'),
    prefix = cms.string('gen@'),
    suffix = cms.string(''),
)

ntupleGen = cms.Sequence(ntupleGenParticles * ntupleGenEventInfo)

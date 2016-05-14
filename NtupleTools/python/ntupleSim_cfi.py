import FWCore.ParameterSet.Config as cms

ntupleSimTracks = cms.EDProducer('NtupleSimTracks',
    inputTag = cms.InputTag('g4SimHits'),
    prefix = cms.string('simTracks@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleSimVertices = cms.EDProducer('NtupleSimVertices',
    inputTag = cms.InputTag('g4SimHits'),
    prefix = cms.string('simVertices@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleSimHits = cms.EDProducer('NtupleSimHits',
    inputTag = cms.InputTag('g4SimHits'),
    simHitCollections = cms.PSet(
        muon = cms.VInputTag(
            #cms.InputTag('g4SimHits','MuonDTHits'),
            cms.InputTag('g4SimHits','MuonCSCHits'),
            #cms.InputTag('g4SimHits','MuonRPCHits'),
        ),
    ),
    prefix = cms.string('simHits@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

#ntupleSim = cms.Sequence(ntupleSimTracks * ntupleSimVertices)
ntupleSim = cms.Sequence(ntupleSimTracks * ntupleSimVertices * ntupleSimHits)


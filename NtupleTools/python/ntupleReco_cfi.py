import FWCore.ParameterSet.Config as cms

goodVertices = cms.EDFilter('VertexSelector',
    filter = cms.bool(True),
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
)

ntupleRecoMuons = cms.EDProducer('NtupleRecoMuons',
    inputTag = cms.InputTag('muons'),
    vertexTag = cms.InputTag('goodVertices'),
    prefix = cms.string('recoMuons@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleReco = cms.Sequence(goodVertices * ntupleRecoMuons)


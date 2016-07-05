import FWCore.ParameterSet.Config as cms

goodVertices = cms.EDFilter('VertexSelector',
    filter = cms.bool(True),
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
)

ntupleRecoTracks = cms.EDProducer('NtupleRecoTracks',
    inputTag = cms.InputTag('generalTracks'),
    prefix = cms.string('recoTracks@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleRecoVertices = cms.EDProducer('NtupleRecoVertices',
    #inputTag = cms.InputTag('offlinePrimaryVertices'),
    inputTag = cms.InputTag('goodVertices'),
    prefix = cms.string('recoVertices@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleRecoPFJets = cms.EDProducer('NtupleRecoPFJets',
    inputTag = cms.InputTag('ak4PFJetsCHS'),
    prefix = cms.string('recoPFJets@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleRecoPFMET = cms.EDProducer('NtupleRecoPFMET',
    inputTag = cms.InputTag('pfMet'),
    prefix = cms.string('recoPFMET@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleRecoMuons = cms.EDProducer('NtupleRecoMuons',
    inputTag = cms.InputTag('muons'),
    vertexTag = cms.InputTag('goodVertices'),
    prefix = cms.string('recoMuons@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleReco = cms.Sequence(goodVertices * ntupleRecoTracks * ntupleRecoVertices * ntupleRecoPFJets * ntupleRecoPFMET * ntupleRecoMuons)


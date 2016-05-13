import FWCore.ParameterSet.Config as cms

from L1TMuonSimulations.NtupleTools.simBeamSpot_cfi import simBeamSpot
from L1TMuonSimulations.NtupleTools.propagator_cfi import SteppingHelixPropagatorAlong, MuonDetLayerGeometryESProducer

ntupleGenParticlesToMuon = cms.EDProducer('NtupleGenParticlesToMuon',
    inputTag = cms.InputTag('genParticles'),
    beamSpotTag = cms.InputTag('simBeamSpot', 'BeamSpot'),
    prefix = cms.string('genParts@'),
    suffix = cms.string(''),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleGenExtra = cms.Sequence(simBeamSpot * ntupleGenParticlesToMuon)


import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("Muon")

process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_v14', '')

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

from L1TMuonSimulations.NtupleTools.propagator_cfi import SteppingHelixPropagatorAlong
process.SteppingHelixPropagatorAlong = SteppingHelixPropagatorAlong
#process.SteppingHelixPropagatorAlong.AssumeNoMaterial = True
#process.SteppingHelixPropagatorAlong.NoErrorPropagation = True
#process.SteppingHelixPropagatorAlong.ApplyRadX0Correction = False

process.a1 = cms.EDAnalyzer("AnalyzerPropagateToMuon",
    ptValues = cms.vdouble(1., 2., 3., 5., 10., 20., 50., 100.),
    etaValues = cms.vdouble(1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4),
    phiValues = cms.vdouble(3.14159265359/4),
    out = cms.string('propagate_to_muon.root'),
    verbosity = cms.int32(0),
)

process.p1 = cms.Path(process.a1)
process.schedule = cms.Schedule(process.p1)


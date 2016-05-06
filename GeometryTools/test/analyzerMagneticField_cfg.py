import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("Geometry")

process.load('Configuration.StandardSequences.MagneticField_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_v6', '')

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.a1 = cms.EDAnalyzer("AnalyzerMagneticField",
    out = cms.string('magnetic_field.root'),
    verbosity = cms.int32(0),
)

process.p1 = cms.Path(process.a1)
process.schedule = cms.Schedule(process.p1)


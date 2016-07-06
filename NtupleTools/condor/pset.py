# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: ntupleMC --step L1REPACK:FullMC,RAW2DIGI --mc --no_output --era Run2_2016 --conditions 80X_mcRun2_asymptotic_v14 --geometry Extended2016,Extended2016Reco --customise L1TMuonSimulations/NtupleTools/customiseNtuple.cust_all --filein file:SingleMuon_PositiveEndCap.root --fileout file:SingleMuon_PositiveEndCap_ntuple.root --no_exec -n 100
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RAW2DIGI',eras.Run2_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.SimL1EmulatorRepack_FullMC_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# Input source
process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('file:SingleMuon_PositiveEndCap.root'),
    fileNames = cms.untracked.vstring('root://xrootd2.ihepa.ufl.edu//store/user/jiafulow/L1MuonTrigger/8_0_9/SingleMuon_PositiveEndCap/ParticleGuns/CRAB3/160705_223732/0000/SingleMuon_PositiveEndCap_103.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('ntupleMC nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_v14', '')

# Path and EndPath definitions
process.L1RePack_step = cms.Path(process.SimL1Emulator)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(process.L1RePack_step,process.raw2digi_step,process.endjob_step)

# customisation of the process.

# Automatic addition of the customisation function from L1TMuonSimulations.NtupleTools.customiseNtuple
from L1TMuonSimulations.NtupleTools.customiseNtuple import cust_all 

#call to customisation function cust_all imported from L1TMuonSimulations.NtupleTools.customiseNtuple
process = cust_all(process)

# End of customisation functions


# Configure framework report and summary
#process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.INFO.limit = 1000000

# Dump the full python config
#with open("dump.py", "w") as f:
#    f.write(process.dumpPython())


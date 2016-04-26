import FWCore.ParameterSet.Config as cms

from L1TMuonSimulations.NtupleTools.ntupleGen_cfi import *
from L1TMuonSimulations.NtupleTools.ntupleSim_cfi import *
from L1TMuonSimulations.NtupleTools.ntupleDigi_cfi import *
from L1TMuonSimulations.NtupleTools.ntupleL1TMuon_cfi import *
from L1TMuonSimulations.NtupleTools.ntupleMaker_cfi import *

ntupleSequence = cms.Sequence(ntupleEventInfo * ntupleGen * ntupleSim * ntupleDigi * ntupleL1TMuon * ntupler)

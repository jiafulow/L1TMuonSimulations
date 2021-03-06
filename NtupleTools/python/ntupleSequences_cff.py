import FWCore.ParameterSet.Config as cms

from L1TMuonSimulations.NtupleTools.ntupleGen_cfi import *
from L1TMuonSimulations.NtupleTools.ntupleGenExtra_cfi import *
from L1TMuonSimulations.NtupleTools.ntupleSim_cfi import *
from L1TMuonSimulations.NtupleTools.ntupleDigi_cfi import *
from L1TMuonSimulations.NtupleTools.ntupleL1T_cfi import *
from L1TMuonSimulations.NtupleTools.ntupleL1TExtra_cfi import *
from L1TMuonSimulations.NtupleTools.ntupleHLT_cfi import *
from L1TMuonSimulations.NtupleTools.ntupleReco_cfi import *
from L1TMuonSimulations.NtupleTools.ntupleMaker_cfi import *

ntupleGen += ntupleGenExtra
ntupleL1T += ntupleL1TExtra

ntupleSequence = cms.Sequence(ntupleEventInfo * ntupleGen * ntupleSim * ntupleDigi * ntupleHLT * ntupleReco * ntupleL1T * ntupler)

ntupleSequence_GENSIM = cms.Sequence(ntupleEventInfo * ntupleGen * ntupleSim * ntupler)

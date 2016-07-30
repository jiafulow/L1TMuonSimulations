import FWCore.ParameterSet.Config as cms

def read_hlt_paths(filename):
    import os
    dirname = os.environ["CMSSW_BASE"] + "/src/L1TMuonSimulations/NtupleTools/data/"
    paths = cms.untracked.vstring()
    with open(dirname + filename) as f:
        lines = []
        for line in f:
            line = line.strip()
            if not line.startswith('#'):
                lines.append(line)
        paths.extend(lines)
    #print "Using HLT file: %s with %i paths." % (filename, len(paths))
    return paths

hlt_paths = read_hlt_paths('hlt_paths.txt')
l1t_paths = read_hlt_paths('l1t_paths.txt')

ntupleTriggerResults = cms.EDProducer('NtupleTriggerResults',
    #hltTag = cms.InputTag('TriggerResults', '', 'HLT'),
    hltTag = cms.InputTag('TriggerResults', '', 'RAWSIM'),
    l1tTag = cms.InputTag('gtStage2Digis'),
    #l1tTag = cms.InputTag('simGtStage2Digis'),
    prefix = cms.string(''),
    suffix = cms.string(''),
    hltPaths = cms.vstring(hlt_paths),
    l1tPaths = cms.vstring(l1t_paths),
    cut = cms.string(''),
    maxN = cms.uint32(999999)
)

ntupleHLT = cms.Sequence(ntupleTriggerResults)


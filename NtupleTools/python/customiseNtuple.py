import FWCore.ParameterSet.Config as cms

def add_ntuplization(process):

    process.TFileService = cms.Service("TFileService",
        fileName = cms.string("ntuple.root")
    )

    process.load("L1TMuonSimulations.NtupleTools.ntupleSequences_cff")
    process.p = cms.Path(process.ntupleSequence)
    process.schedule.append(process.p)

    return process


def cust_all(process):

    add_ntuplization(process)

    return process


import FWCore.ParameterSet.Config as cms

def read_pileup_input(filename):
    import os
    dirname = os.environ["CMSSW_BASE"] + "/src/L1TMuonSimulations/Configuration/data/"
    readFiles = cms.untracked.vstring()
    with open(dirname + filename) as f:
         readFiles.extend([line.strip() for line in f])
    print "Using pileup input: %s with %i files." % (filename, len(readFiles))
    return readFiles

def add_pileup(process):

    RunIISummer15GS = True

    if RunIISummer15GS:
        # with cmsDriver: --pileup 2015_25ns_FallMC_matchData_PoissonOOTPU \
        process.load('SimGeneral.MixingModule.mix_2015_25ns_FallMC_matchData_PoissonOOTPU_cfi')

        # with cmsDriver: --pileup_input "dbs:/MinBias_TuneCUETP8M1_13TeV-pythia8/RunIISummer15GS-MCRUN2_71_V1-v2/GEN-SIM" \
        readFiles = read_pileup_input("pileup_input_MinBias_TuneCUETP8M1_13TeV-pythia8.txt")
        process.mix.input.fileNames = readFiles

        # with cmsDriver: --customise_commands "process.mix.input.nbPileupEvents.probValue = cms.vdouble(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0) \n process.simHcalDigis.markAndPass = cms.bool(True) " \
        # PU30
        process.mix.input.nbPileupEvents.probValue = cms.vdouble(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
        process.simHcalDigis.markAndPass = cms.bool(True)

    return process


def cust_all(process):

    add_pileup(process)

    return process


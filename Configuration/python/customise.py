import FWCore.ParameterSet.Config as cms

def read_pileup_input(filename):
    import os
    dirname = os.environ["CMSSW_BASE"] + "/src/L1TMuonSimulations/Configuration/data/"
    readFiles = cms.untracked.vstring()
    with open(dirname + filename) as f:
        lines = []
        for line in f:
            line = line.strip()
            if not line.startswith('#'):
                lines.append(line)
        readFiles.extend(lines)
    print "Using pileup input: %s with %i files." % (filename, len(readFiles))
    return readFiles

def add_pileup(process):
    pileup = "RunIISpring16DR80"

    if pileup == "RunIISummer15GS":
        # with cmsDriver: --pileup 2015_25ns_FallMC_matchData_PoissonOOTPU \
        process.load('SimGeneral.MixingModule.mix_2015_25ns_FallMC_matchData_PoissonOOTPU_cfi')

        # with cmsDriver: --pileup_input "dbs:/MinBias_TuneCUETP8M1_13TeV-pythia8/RunIISummer15GS-MCRUN2_71_V1-v2/GEN-SIM" \
        readFiles = read_pileup_input("pileup_input_MinBias_TuneCUETP8M1_13TeV-pythia8.txt")
        process.mix.input.fileNames = readFiles

        # with cmsDriver: --customise_commands "process.mix.input.nbPileupEvents.probValue = cms.vdouble(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0) \n process.simHcalDigis.markAndPass = cms.bool(True) " \
        # PU30
        process.mix.input.nbPileupEvents.probValue = cms.vdouble(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
        process.simHcalDigis.markAndPass = cms.bool(True)

    elif pileup == "RunIISpring16DR80":
        # with cmsDriver: --pileup 2016_25ns_SpringMC_PUScenarioV1_PoissonOOTPU \
        process.load("SimGeneral.MixingModule.mix_2016_25ns_SpringMC_PUScenarioV1_PoissonOOTPU_cfi")

        # with cmsDriver: --pileup_input "dbs:/MinBias_TuneCUETP8M1_13TeV-pythia8/RunIISummer15GS-MCRUN2_71_V1-v2/GEN-SIM" \
        readFiles = read_pileup_input("pileup_input_MinBias_TuneCUETP8M1_13TeV-pythia8.txt")
        process.mix.input.fileNames = readFiles

        # with cmsDriver: --customise_commands "process.simHcalDigis.markAndPass = cms.bool(True)" \
        process.simHcalDigis.markAndPass = cms.bool(True)

        # PU0
        #process.mix.input.nbPileupEvents.probValue = cms.vdouble(1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
        # PU10
        #process.mix.input.nbPileupEvents.probValue = cms.vdouble(0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
        # PU20
        #process.mix.input.nbPileupEvents.probValue = cms.vdouble(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
        # PU30
        #process.mix.input.nbPileupEvents.probValue = cms.vdouble(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
        # PU40
        #process.mix.input.nbPileupEvents.probValue = cms.vdouble(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0)
        # PU50
        #process.mix.input.nbPileupEvents.probValue = cms.vdouble(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1)

    return process

def cust_event_content(process):
    alist=['RAWSIM','FEVTDEBUG','FEVTDEBUGHLT','GENRAW','RAWSIMHLT','FEVT']
    for a in alist:
        b=a+'output'
        if hasattr(process,b):
            # Keep tracking particles
            getattr(process,b).outputCommands.append('keep *_mix_MergedTrackTruth_*')

    alist=['RAWSIM']
    for a in alist:
        b=a+'output'
        if hasattr(process,b):
           # Add AOD event content
           getattr(process,b).outputCommands.extend(process.AODEventContent.outputCommands[1:])
    return process

def cust_pgun(process):
    cust_event_content(process)
    return process

def cust_pileup(process):
    add_pileup(process)
    cust_event_content(process)
    return process


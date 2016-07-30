import FWCore.ParameterSet.Config as cms

def add_ntuple_seq(process):
    process.TFileService = cms.Service('TFileService',
        fileName = cms.string('ntuple.root')
    )
    process.load('L1TMuonSimulations.NtupleTools.ntupleSequences_cff')
    process.p = cms.Path(process.ntupleSequence)
    process.schedule.append(process.p)
    return process

def cust_ntuple(process):
    process = add_ntuple_seq(process)
    return process

def do_less_please(process):
    #process.schedule = cms.Schedule(*[ process.generation_step, process.genfiltersummary_step, process.simulation_step, process.digitisation_step, process.L1simulation_step, process.digi2raw_step, process.raw2digi_step, process.endjob_step, process.p ])

    if hasattr(process,'generation_step'):
        pass

    if hasattr(process,'genfiltersummary_step'):
        process.schedule.remove(process.genfiltersummary_step)

    if hasattr(process,'simulation_step'):
        pass

    if hasattr(process,'digitisation_step'):
        # Drop calorimeter hits
        for removee in ['mixCH']:
            if hasattr(process.mix.mixObjects,removee):
                delattr(process.mix.mixObjects,removee)

    if hasattr(process,'L1simulation_step'):
        pass

    if hasattr(process,'digi2raw_step'):
        process.schedule.remove(process.digi2raw_step)

    if hasattr(process,'raw2digi_step'):
        process.schedule.remove(process.raw2digi_step)

    if hasattr(process,'endjob_step'):
        process.schedule.remove(process.endjob_step)

    if hasattr(process,'p'):
        process.ntupleSequence.remove(process.ntupleHLT)
        process.ntupleSequence.remove(process.ntupleReco)
        process.ntupleL1T.remove(process.ntupleLegacyCSCTFTriggerTracks)
    return process

import FWCore.ParameterSet.Config as cms

# reference: https://github.com/cms-sw/genproductions/blob/master/python/EightTeV/SingleMuMinusFlatPt0p2To100_cff.py
generator = cms.EDProducer("FlatRandomPtGunProducer2",
    PGunParameters = cms.PSet(
        MaxPt = cms.double(3.0*1.001),
        MinPt = cms.double(3.0),
        PartID = cms.vint32(-13),
        MinEta = cms.double(1.0),
        MaxEta = cms.double(2.5),
        MinPhi = cms.double(-3.14159265359),
        MaxPhi = cms.double(3.14159265359),
        RandomCharge = cms.bool(True),
        PtSpectrum = cms.string('flatPt'),
    ),
    Verbosity = cms.untracked.int32(0),
    psethack = cms.string('single muon+/- pt 3 positive endcap'),
    AddAntiParticle = cms.bool(False),
    firstRun = cms.untracked.uint32(1)
)

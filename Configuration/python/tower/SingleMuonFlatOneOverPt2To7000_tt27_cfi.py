import FWCore.ParameterSet.Config as cms

# reference: https://github.com/cms-sw/genproductions/blob/master/python/EightTeV/SingleMuMinusFlatPt0p2To100_cff.py
generator = cms.EDProducer("FlatRandomPtGunProducer2",
    PGunParameters = cms.PSet(
        MaxPt = cms.double(7000.0),
        MinPt = cms.double(2.0),
        PartID = cms.vint32(-13),
        MinEta = cms.double(0.0),
        MaxEta = cms.double(2.2/3),
        MinPhi = cms.double(3.14159265359/4),
        MaxPhi = cms.double(3.14159265359/2),
        XFlatSpread = cms.double(5./1.41421356),  ## in mm
        YFlatSpread = cms.double(5./1.41421356),  ## in mm
        ZFlatSpread = cms.double(70.), ## in mm
        RStarForPhi = cms.double(58.8889),  ## in cm
        #RStarForEta = cms.double(110.),  ## in cm
        RandomCharge = cms.bool(True),
        PtSpectrum = cms.string('flatOneOverPt'),
    ),
    Verbosity = cms.untracked.int32(0),
    psethack = cms.string('single muon+/- pt 2 to 7000 flat in 1/pt trigger tower 27'),
    AddAntiParticle = cms.bool(False),
    firstRun = cms.untracked.uint32(1)
)

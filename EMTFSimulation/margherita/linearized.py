#!/usr/bin/env python

from ROOT import TFile, TTree, TH1F, TH2F, TProfile, TGraph, gROOT, gStyle, gPad
from math import sqrt, pi, sinh, atan2, tan, exp, log, degrees, radians
from itertools import izip, count
import numpy as np
from numpy import linalg as LA
#from scipy import optimize
#import matplotlib.pyplot as plt
import random
random.seed(2016)

from linregression import *


# ______________________________________________________________________________
# Configurations
#fname = "stubs_flatOneOverPt100to200.1.root"
fname = "/home/jlow/L1MuonTrigger/scratch/work_20160804/stubs_fixBeamSpot_pt10.2.root"
#fname = "/home/jlow/L1MuonTrigger/scratch/work_20160804/stubs.2.root"
imgdir = "figures_linearized/"
nentries = 50000
use_invPt = 1
fix_beamspot = 0
fix_charge = 1
fix_phi = 0
verbose = 1
make_plots = 1

#minInvPt = -1.0/20
#maxInvPt = +1.0/20
minInvPt = -1.0/200
maxInvPt = +1.0/200
minEta = 1.2
maxEta = 2.4
#minEta = 1.9
#maxEta = 2.1

z_center = np.asarray([602.332, 698.693, 828.392, 935.437, 1025.17])

magfield = np.asarray([[
    +3.007, +3.045, +3.063, +3.071, +3.072, +3.069, +3.063, +3.054, +3.041, +3.026,
    +3.006, +2.982, +2.950, +2.909, +2.855, +2.778, +2.682, +2.601, +2.574, +2.592,
    +2.655, +2.627, +1.622, +0.422, -0.311, -0.215, -0.153, -0.114, -0.089, -0.072,
    -0.059, -0.050, -0.041, -0.031, -0.021, -0.010, -1.010, -1.270, -1.317, -0.106,
    -0.006, -0.007, -0.004, -0.344, -0.787, -0.806, -0.804, -0.805, -0.802, -0.518,
    -0.003, -0.005, -0.004, -0.179, -0.795, -0.833, -0.833, -0.836, -0.842, -0.731
    ],[
    +2.888, +2.882, +2.871, +2.859, +2.848, +2.837, +2.827, +2.819, +2.812, +2.806,
    +2.802, +2.801, +2.804, +2.813, +2.829, +2.857, +1.580, +0.974, +0.986, +0.965,
    +0.919, +0.859, +0.787, +0.707, +0.626, +0.545, +0.468, +0.394, +0.325, +0.259,
    +0.195, +0.133, +0.069, +0.004, -0.065, -0.133, -0.189, -0.214, -0.224, -0.234,
    -0.250, -0.276, -0.310, -0.344, -0.365, -0.373, -0.377, -0.377, -0.376, -0.372,
    -0.365, -0.365, -0.375, -0.393, -0.407, -0.414, -0.416, -0.417, -0.414, -0.384
    ],[
    +3.201, +1.991, +1.278, +1.293, +1.286, +1.273, +1.254, +1.231, +1.201, +1.165,
    +1.125, +1.077, +1.024, +0.967, +0.906, +0.843, +0.779, +0.715, +0.653, +0.593,
    +0.535, +0.480, +0.428, +0.378, +0.331, +0.286, +0.243, +0.204, +0.166, +0.132,
    +0.100, +0.071, +0.046, +0.023, +0.004, -0.012, -0.024, -0.034, -0.041, -0.047,
    -0.052, -0.055, -0.058, -0.060, -0.062, -0.063, -0.064, -0.065, -0.065, -0.066,
    -0.066, -0.066, -0.067, -0.068, -0.069, -0.071, -0.074, -0.079, -0.086, -0.087
    ],[
    +1.437, +2.198, +2.210, +1.405, +0.120, +0.125, +0.127, +0.128, +0.129, +0.129,
    +0.129, +0.128, +0.127, +0.126, +0.125, +0.123, +0.121, +0.118, +0.115, +0.112,
    +0.109, +0.105, +0.102, +0.099, +0.096, +0.093, +0.091, +0.089, +0.087, +0.085,
    +0.084, +0.083, +0.082, +0.081, +0.081, +0.080, +0.080, +0.080, +0.079, +0.079,
    +0.079, +0.079, +0.079, +0.078, +0.078, +0.078, +0.078, +0.078, +0.078, +0.078,
    +0.078, +0.078, +0.079, +0.079, +0.080, +0.082, +0.084, +0.088, +0.095, +0.092
    ],[
    +0.182, +1.013, +1.331, +1.310, +1.293, +0.404, +0.025, +0.037, +0.044, +0.048,
    +0.051, +0.053, +0.055, +0.056, +0.057, +0.057, +0.057, +0.058, +0.058, +0.058,
    +0.058, +0.058, +0.058, +0.058, +0.058, +0.058, +0.058, +0.058, +0.058, +0.058,
    +0.058, +0.058, +0.059, +0.059, +0.059, +0.059, +0.059, +0.059, +0.059, +0.059,
    +0.059, +0.059, +0.059, +0.059, +0.060, +0.060, +0.060, +0.060, +0.060, +0.060,
    +0.060, +0.061, +0.061, +0.062, +0.063, +0.064, +0.067, +0.070, +0.076, +0.077
]])

def get_magnetic_field(i, r):
    assert(r > 0)
    rbin = min((r - 100)//10, 699.9)
    return magfield[(i,rbin)]

# ______________________________________________________________________________
def process_step1():

    data_var1 = []
    data_var2 = []
    data_var3 = []
    data_par1 = []
    data_par2 = []

    # __________________________________________________________________________
    # Accumulate data

    for ievt, evt in enumerate(ttree):
        if ievt == nentries:
            break

        # Must have 1 particle
        if len(evt.genParts_pt) != 1:
            continue

        # Must have 4 stubs
        if len(evt.CSCStubs_globalPhi) != 5:
            continue
        if evt.CSCStubs_moduleId[0] == 999999 or evt.CSCStubs_moduleId[2] == 999999 or evt.CSCStubs_moduleId[3] == 999999 or evt.CSCStubs_moduleId[4] == 999999:
            continue

        # Sanity checks
        assert(len(evt.genParts_charge) == 1)
        assert(len(evt.genParts_pt) == 1)
        assert(len(evt.genParts_eta) == 1)
        assert(len(evt.CSCStubs_globalPhi) == 5)
        assert(len(evt.CSCStubs_globalRho) == 5)
        assert(len(evt.CSCStubs_globalZ) == 5)

        # Get track variables
        simInvPt = float(evt.genParts_charge[0])/evt.genParts_pt[0]
        simPhi = evt.genParts_phi[0]
        simEta = evt.genParts_eta[0]
        simCotTheta = sinh(simEta)
        simTanTheta = 1.0/simCotTheta
        simVz = evt.genParts_vz[0]

        parameters1 = np.array([simInvPt, simPhi])
        parameters2 = np.array([simTanTheta, simVz])

        # Must satisfy invPt and eta ranges
        if use_invPt:
            if not ((minInvPt <= simInvPt < maxInvPt) and (minEta <= simEta < maxEta)):
                continue
        else:
            if not ((minInvPt <= simInvPt*simTanTheta < maxInvPt) and (minEta <= simEta < maxEta)):
                continue

        # Fix beamspot
        if fix_beamspot:
            if not (-1 <= simVz <= 1):
                continue

        # Fix charge
        if fix_charge:
            if not (int(evt.genParts_charge[0]) == -1):
                continue

        # Fix phi
        if fix_phi:
            if not ((pi/4)-radians(10) <= simPhi <= (pi/4)+radians(10)):
                continue

        # Get stub variables
        variables1 = np.array(evt.CSCStubs_globalPhi)
        variables2 = np.array(evt.CSCStubs_globalRho)
        #variables3 = np.array(evt.CSCStubs_globalZ) - z_center  # deltaZ
        variables3 = np.array(evt.CSCStubs_globalZ)

        # Mask stub variables
        mask = np.array([True, False, True, True, True], dtype=bool)
        variables1 = variables1[mask]
        variables2 = variables2[mask]
        variables3 = variables3[mask]

        data_var1.append(variables1)
        data_var2.append(variables2)
        data_var3.append(variables3)
        data_par1.append(parameters1)
        data_par2.append(parameters2)
        continue

    # __________________________________________________________________________
    # Make plots
    if make_plots:
        histos = {}

        n = len(data_var1)
        assert(n == len(data_var2))

        mask = np.array([True, False, True, True, True], dtype=bool)
        tmp_z_center = z_center[mask]

        def label_st(j):
            if j < 4:
                s = "ME%i" % (j+1)
            elif j == 4:
                s = "pol0"
            elif j == 5:
                s = "pol1"
            return s

        phimin, phimax = -40e-3, 40e-3
        rmin, rmax = 0, 800
        zmin, zmax = 0, 1200

        # phi
        for j in xrange(6):
            hname = "h_phi%i_vs_p0" % j
            histos[hname] = TH2F(hname, "; q/p_{T} [1/GeV]; #phi_{%s} [rad]" % label_st(j), 100, -0.02, 0.02, 200, phimin, phimax)
            #histos[hname] = TH2F(hname, "; q/p_{T} [1/GeV]; #phi_{%s} [rad]" % label_st(j), 100, minInvPt, maxInvPt, 200, phimin, phimax)
            histos[hname].g = TGraph(n)
            hname = "h_phi%i_vs_p1" % j
            histos[hname] = TH2F(hname, "; #phi_{0} [rad]; #phi_{%s} [rad]" % label_st(j), 100, radians(15), radians(75), 200, phimin, phimax)
            histos[hname].g = TGraph(n)
            hname = "h_phi%i_vs_p2" % j
            histos[hname] = TH2F(hname, "; tan #theta; #phi_{%s} [rad]" % label_st(j), 100, 0.1, 0.9, 200, phimin, phimax)
            histos[hname].g = TGraph(n)
            hname = "h_phi%i_vs_p3" % j
            histos[hname] = TH2F(hname, "; z_{0} [cm]; #phi_{%s} [rad]" % label_st(j), 100, -30., 30., 200, phimin, phimax)
            histos[hname].g = TGraph(n)
            hname = "h_phi%i_vs_p4" % j
            histos[hname] = TH2F(hname, "; q/p_{z} [1/GeV]; #phi_{%s} [rad]" % label_st(j), 100, -0.02, 0.02, 200, phimin, phimax)
            #histos[hname] = TH2F(hname, "; q/p_{z} [1/GeV]; #phi_{%s} [rad]" % label_st(j), 100, minInvPt, maxInvPt, 200, phimin, phimax)
            histos[hname].g = TGraph(n)

            hname = "h_phi%i_corr0" % j
            histos[hname] = TH1F(hname, "; #phi_{%s} [rad]; Entries" % label_st(j), 200, phimin, phimax)
            hname = "h_phi%i_corr1" % j
            histos[hname] = TH1F(hname, "; #phi_{%s} [rad]; Entries" % label_st(j), 200, phimin, phimax)
            hname = "h_phi%i_f_corr0" % j
            histos[hname] = TH1F(hname, "; #phi_{%s} [rad]; Entries" % label_st(j), 200, phimin, phimax)
            hname = "h_phi%i_f_corr1" % j
            histos[hname] = TH1F(hname, "; #phi_{%s} [rad]; Entries" % label_st(j), 200, phimin, phimax)
            hname = "h_phi%i_r_corr0" % j
            histos[hname] = TH1F(hname, "; #phi_{%s} [rad]; Entries" % label_st(j), 200, phimin, phimax)
            hname = "h_phi%i_r_corr1" % j
            histos[hname] = TH1F(hname, "; #phi_{%s} [rad]; Entries" % label_st(j), 200, phimin, phimax)

        hname = "h_phi_of_z_corr0"
        histos[hname] = TH2F(hname, "; z [cm]; #phi [rad]", 200, zmin, zmax, 200, phimin, phimax)
        histos[hname].g = TGraph(n*4)
        hname = "h_phi_of_z_corr1"
        histos[hname] = TH2F(hname, "; z [cm]; #phi [rad]", 200, zmin, zmax, 200, phimin, phimax)
        histos[hname].g = TGraph(n*4)
        hname = "h_phi_of_zt_corr0"
        histos[hname] = TH2F(hname, "; z*tan #theta [cm]; #phi [rad]", 200, rmin, rmax, 200, phimin, phimax)
        histos[hname].g = TGraph(n*4)
        hname = "h_phi_of_zt_corr1"
        histos[hname] = TH2F(hname, "; z*tan #theta [cm]; #phi [rad]", 200, rmin, rmax, 200, phimin, phimax)
        histos[hname].g = TGraph(n*4)

        # rho
        for j in xrange(6):
            hname = "h_rho%i_vs_p0" % j
            histos[hname] = TH2F(hname, "; q/p_{T} [1/GeV]; r_{%s} [cm]" % label_st(j), 100, -0.02, 0.02, 200, rmin, rmax)
            #histos[hname] = TH2F(hname, "; q/p_{T} [1/GeV]; r_{%s} [cm]" % label_st(j), 100, minInvPt, maxInvPt, 200, rmin, rmax)
            histos[hname].g = TGraph(n)
            hname = "h_rho%i_vs_p1" % j
            histos[hname] = TH2F(hname, "; #phi_{0} [rad]; r_{%s} [cm]" % label_st(j), 100, radians(15), radians(75), 200, rmin, rmax)
            histos[hname].g = TGraph(n)
            hname = "h_rho%i_vs_p2" % j
            histos[hname] = TH2F(hname, "; tan #theta; r_{%s} [cm]" % label_st(j), 100, 0.1, 0.9, 200, rmin, rmax)
            histos[hname].g = TGraph(n)
            hname = "h_rho%i_vs_p3" % j
            histos[hname] = TH2F(hname, "; z_{0} [cm]; r_{%s} [cm]" % label_st(j), 100, -30., 30., 200, rmin, rmax)
            histos[hname].g = TGraph(n)
            hname = "h_rho%i_vs_p4" % j
            histos[hname] = TH2F(hname, "; q/p_{z} [1/GeV]; r_{%s} [cm]" % label_st(j), 100, -0.02, 0.02, 200, rmin, rmax)
            #histos[hname] = TH2F(hname, "; q/p_{z} [1/GeV]; r_{%s} [cm]" % label_st(j), 100, minInvPt, maxInvPt, 200, rmin, rmax)
            histos[hname].g = TGraph(n)

            hname = "h_rho%i_corr0" % j
            histos[hname] = TH1F(hname, "; r_{%s} [cm]; Entries" % label_st(j), 200, rmin, rmax)
            hname = "h_rho%i_corr1" % j
            histos[hname] = TH1F(hname, "; r_{%s} [cm]; Entries" % label_st(j), 200, rmin, rmax)
            hname = "h_rho%i_f_corr0" % j
            histos[hname] = TH1F(hname, "; r_{%s} [cm]; Entries" % label_st(j), 200, rmin, rmax)
            hname = "h_rho%i_f_corr1" % j
            histos[hname] = TH1F(hname, "; r_{%s} [cm]; Entries" % label_st(j), 200, rmin, rmax)
            hname = "h_rho%i_r_corr0" % j
            histos[hname] = TH1F(hname, "; r_{%s} [cm]; Entries" % label_st(j), 200, rmin, rmax)
            hname = "h_rho%i_r_corr1" % j
            histos[hname] = TH1F(hname, "; r_{%s} [cm]; Entries" % label_st(j), 200, rmin, rmax)

        hname = "h_rho_of_z_corr0"
        histos[hname] = TH2F(hname, "; z [cm]; r [cm]", 200, zmin, zmax, 200, rmin, rmax)
        histos[hname].g = TGraph(n*4)
        hname = "h_rho_of_z_corr1"
        histos[hname] = TH2F(hname, "; z [cm]; r [cm]", 200, zmin, zmax, 200, rmin, rmax)
        histos[hname].g = TGraph(n*4)
        hname = "h_rho_of_zt_corr0"
        histos[hname] = TH2F(hname, "; z*tan #theta [cm]; r [cm]", 200, rmin, rmax, 200, rmin, rmax)
        histos[hname].g = TGraph(n*4)
        hname = "h_rho_of_zt_corr1"
        histos[hname] = TH2F(hname, "; z*tan #theta [cm]; r [cm]", 200, rmin, rmax, 200, rmin, rmax)
        histos[hname].g = TGraph(n*4)


        # Fill histograms
        for ievt, variables1, variables2, variables3, parameters1, parameters2 in izip(count(), data_var1, data_var2, data_var3, data_par1, data_par2):

            simInvPt, simPhi = parameters1
            simTanTheta, simVz = parameters2
            simInvPz = simInvPt * simTanTheta

            variables1 -= simPhi # force phi0 to be zero

            tmp_variables3 = variables3 - tmp_z_center

            #print ievt, variables1, variables2

            # ______________________________________________________________________
            # Scatter plots
            for j in xrange(4):
                histos["h_phi%i_vs_p0" % j].Fill(simInvPt   , variables1[j])
                histos["h_phi%i_vs_p1" % j].Fill(simPhi     , variables1[j])
                histos["h_phi%i_vs_p2" % j].Fill(simTanTheta, variables1[j])
                histos["h_phi%i_vs_p3" % j].Fill(simVz      , variables1[j])
                histos["h_phi%i_vs_p4" % j].Fill(simInvPz   , variables1[j])
                histos["h_rho%i_vs_p0" % j].Fill(simInvPt   , variables2[j])
                histos["h_rho%i_vs_p1" % j].Fill(simPhi     , variables2[j])
                histos["h_rho%i_vs_p2" % j].Fill(simTanTheta, variables2[j])
                histos["h_rho%i_vs_p3" % j].Fill(simVz      , variables2[j])
                histos["h_rho%i_vs_p4" % j].Fill(simInvPz   , variables2[j])
                histos["h_phi%i_vs_p0" % j].g.SetPoint(ievt, simInvPt   , variables1[j])
                histos["h_phi%i_vs_p1" % j].g.SetPoint(ievt, simPhi     , variables1[j])
                histos["h_phi%i_vs_p2" % j].g.SetPoint(ievt, simTanTheta, variables1[j])
                histos["h_phi%i_vs_p3" % j].g.SetPoint(ievt, simVz      , variables1[j])
                histos["h_phi%i_vs_p4" % j].g.SetPoint(ievt, simInvPz   , variables1[j])
                histos["h_rho%i_vs_p0" % j].g.SetPoint(ievt, simInvPt   , variables2[j])
                histos["h_rho%i_vs_p1" % j].g.SetPoint(ievt, simPhi     , variables2[j])
                histos["h_rho%i_vs_p2" % j].g.SetPoint(ievt, simTanTheta, variables2[j])
                histos["h_rho%i_vs_p3" % j].g.SetPoint(ievt, simVz      , variables2[j])
                histos["h_rho%i_vs_p4" % j].g.SetPoint(ievt, simInvPz   , variables2[j])

            # With regression
            reg_phi = LinearRegression()
            reg_rho = LinearRegression()
            reg_phi.add_n(variables3, variables1)
            reg_rho.add_n(variables3, variables2)

            j = 4
            histos["h_phi%i_vs_p0" % j].g.SetPoint(ievt, simInvPt   , reg_phi.alpha())
            histos["h_phi%i_vs_p1" % j].g.SetPoint(ievt, simPhi     , reg_phi.alpha())
            histos["h_phi%i_vs_p2" % j].g.SetPoint(ievt, simTanTheta, reg_phi.alpha())
            histos["h_phi%i_vs_p3" % j].g.SetPoint(ievt, simVz      , reg_phi.alpha())
            histos["h_phi%i_vs_p4" % j].g.SetPoint(ievt, simInvPz   , reg_phi.alpha())
            histos["h_rho%i_vs_p0" % j].g.SetPoint(ievt, simInvPt   , reg_rho.alpha())
            histos["h_rho%i_vs_p1" % j].g.SetPoint(ievt, simPhi     , reg_rho.alpha())
            histos["h_rho%i_vs_p2" % j].g.SetPoint(ievt, simTanTheta, reg_rho.alpha())
            histos["h_rho%i_vs_p3" % j].g.SetPoint(ievt, simVz      , reg_rho.alpha())
            histos["h_rho%i_vs_p4" % j].g.SetPoint(ievt, simInvPz   , reg_rho.alpha())

            j = 5
            histos["h_phi%i_vs_p0" % j].g.SetPoint(ievt, simInvPt   , reg_phi.beta())
            histos["h_phi%i_vs_p1" % j].g.SetPoint(ievt, simPhi     , reg_phi.beta())
            histos["h_phi%i_vs_p2" % j].g.SetPoint(ievt, simTanTheta, reg_phi.beta())
            histos["h_phi%i_vs_p3" % j].g.SetPoint(ievt, simVz      , reg_phi.beta())
            histos["h_phi%i_vs_p4" % j].g.SetPoint(ievt, simInvPz   , reg_phi.beta())
            histos["h_rho%i_vs_p0" % j].g.SetPoint(ievt, simInvPt   , reg_rho.beta())
            histos["h_rho%i_vs_p1" % j].g.SetPoint(ievt, simPhi     , reg_rho.beta())
            histos["h_rho%i_vs_p2" % j].g.SetPoint(ievt, simTanTheta, reg_rho.beta())
            histos["h_rho%i_vs_p3" % j].g.SetPoint(ievt, simVz      , reg_rho.beta())
            histos["h_rho%i_vs_p4" % j].g.SetPoint(ievt, simInvPz   , reg_rho.beta())

            # ______________________________________________________________________
            # Before corrections
            for j in xrange(4):
                histos["h_phi_of_z_corr0"].Fill(variables3[j], variables1[j])
                histos["h_rho_of_z_corr0"].Fill(variables3[j], variables2[j])
                histos["h_phi_of_z_corr0"].g.SetPoint(ievt*4 + j, variables3[j], variables1[j])
                histos["h_rho_of_z_corr0"].g.SetPoint(ievt*4 + j, variables3[j], variables2[j])

                histos["h_phi_of_zt_corr0"].Fill(variables3[j]*simTanTheta, variables1[j])
                histos["h_rho_of_zt_corr0"].Fill(variables3[j]*simTanTheta, variables2[j])
                histos["h_phi_of_zt_corr0"].g.SetPoint(ievt*4 + j, variables3[j]*simTanTheta, variables1[j])
                histos["h_rho_of_zt_corr0"].g.SetPoint(ievt*4 + j, variables3[j]*simTanTheta, variables2[j])

                histos["h_phi%i_corr0" % j].Fill(variables1[j])
                histos["h_rho%i_corr0" % j].Fill(variables2[j])
                if tmp_variables3[j] < 0.:
                    histos["h_phi%i_f_corr0" % j].Fill(variables1[j])
                    histos["h_rho%i_f_corr0" % j].Fill(variables2[j])
                else:
                    histos["h_phi%i_r_corr0" % j].Fill(variables1[j])
                    histos["h_rho%i_r_corr0" % j].Fill(variables2[j])

            # ______________________________________________________________________
            # Apply deltaZ correction
            bz = np.array([get_magnetic_field(i,r) for i, r in enumerate(variables2)])
            simC = (-0.5 * (0.003 * 1.0 * simInvPt)) * bz
            simT = 1.0
            simC *= simTanTheta
            simT *= simTanTheta
            variables1 -= simC * tmp_variables3
            variables2 -= simT * tmp_variables3
            variables3 -= 1.0 * tmp_variables3

            # ______________________________________________________________________
            # Apply non-pointing strip correction
            #ext_variables2 = variables3 * simTanTheta
            #variables1 += np.divide(ext_variables2 - variables2, variables2) * -1 * np.tan(variables1)
            #variables2 = ext_variables2

            # ______________________________________________________________________
            # After corrections
            for j in xrange(4):
                histos["h_phi_of_z_corr1"].Fill(variables3[j], variables1[j])
                histos["h_rho_of_z_corr1"].Fill(variables3[j], variables2[j])
                histos["h_phi_of_z_corr1"].g.SetPoint(ievt*4 + j, variables3[j], variables1[j])
                histos["h_rho_of_z_corr1"].g.SetPoint(ievt*4 + j, variables3[j], variables2[j])

                histos["h_phi_of_zt_corr1"].Fill(variables3[j]*simTanTheta, variables1[j])
                histos["h_rho_of_zt_corr1"].Fill(variables3[j]*simTanTheta, variables2[j])
                histos["h_phi_of_zt_corr1"].g.SetPoint(ievt*4 + j, variables3[j]*simTanTheta, variables1[j])
                histos["h_rho_of_zt_corr1"].g.SetPoint(ievt*4 + j, variables3[j]*simTanTheta, variables2[j])

                histos["h_phi%i_corr1" % j].Fill(variables1[j])
                histos["h_rho%i_corr1" % j].Fill(variables2[j])
                if tmp_variables3[j] < 0.:
                    histos["h_phi%i_f_corr1" % j].Fill(variables1[j])
                    histos["h_rho%i_f_corr1" % j].Fill(variables2[j])
                else:
                    histos["h_phi%i_r_corr1" % j].Fill(variables1[j])
                    histos["h_rho%i_r_corr1" % j].Fill(variables2[j])

        print "Number of events: ", n

        # Write histograms
        outfile = TFile.Open("histos_linearized.root", "RECREATE")
        for hname, h in sorted(histos.iteritems()):
            h.Write()
            if hasattr(h, "g"):
                g = getattr(h, "g")
                g.SetName("g_"+h.GetName()[2:])
                g.Write()
        outfile.Close()

    return


# ______________________________________________________________________________
def main():
    process_step1()

    return

# ______________________________________________________________________________
if __name__ == '__main__':

    tfile = TFile.Open(fname)
    ttree = tfile.Get("ntupler/tree")
    gROOT.LoadMacro("tdrstyle.C")
    gROOT.ProcessLine("setTDRStyle()")
    gROOT.SetBatch(True)
    gStyle.SetOptStat(111110)

    main()

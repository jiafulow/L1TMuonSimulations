#!/usr/bin/env python

from ROOT import TFile, TTree, TH1F, TH2F, TProfile, TGraph, gROOT, gStyle, gPad
from math import sqrt, pi, sinh, atan2, tan, exp, log
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
fname = "stubs_flatPt100to200.2.root"
#fname = "stubs_flatPt100to200.1.root"
#fname = "/home/jlow/L1MuonTrigger/scratch/work_20160804/stubs_flatPt100to200.0.root"
imgdir = "figures_linearized/"
nentries = 100000
fix_beamspot = 0
fix_charge = 1
fix_phi = 1
verbose = 1
make_plots = 1

minInvPt = -1.0/190
maxInvPt = +1.0/190
minEta = 1.2
maxEta = 2.4
#minEta = 1.9
#maxEta = 2.1

z_center = np.asarray([602.332, 698.693, 828.392, 935.437, 1025.17])


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
        if not ((minInvPt <= simInvPt < maxInvPt) and (minEta <= simEta < maxEta)):
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
            if not ((pi/4)-0.087266 <= simPhi <= (pi/4)+0.087266):
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

        # phi
        phimin, phimax = -40e-3, 40e-3
        hname = "h_phi_of_z"
        histos[hname] = TH2F(hname, "; z [cm]; #phi [rad]", 110, 0, 1100, 200, phimin, phimax)
        histos[hname].g = TGraph(n*4)

        for j in xrange(6):
            hname = "h_phi%i_vs_p0" % j
            histos[hname] = TH2F(hname, "; q/p_{T} [1/GeV]; #phi_{%s} - #phi_{0} [rad]" % label_st(j), 100, -0.02, 0.02, 200, phimin, phimax)
            histos[hname].g = TGraph(n)
            hname = "h_phi%i_vs_p1" % j
            histos[hname] = TH2F(hname, "; #phi_{0} [rad]; #phi_{%s} - #phi_{0} [rad]" % label_st(j), 100, pi*15/180, pi*75/180, 200, phimin, phimax)
            histos[hname].g = TGraph(n)
            hname = "h_phi%i_vs_p2" % j
            histos[hname] = TH2F(hname, "; tan #theta; #phi_{%s} - #phi_{0} [rad]" % label_st(j), 100, 0.1, 0.9, 200, phimin, phimax)
            histos[hname].g = TGraph(n)
            hname = "h_phi%i_vs_p3" % j
            histos[hname] = TH2F(hname, "; z_{0} [cm]; #phi_{%s} - #phi_{0} [rad]" % label_st(j), 100, -30., 30., 200, phimin, phimax)
            histos[hname].g = TGraph(n)
            hname = "h_phi%i_vs_p4" % j
            histos[hname] = TH2F(hname, "; q/p_{z} [1/GeV]; #phi_{%s} - #phi_{0} [rad]" % label_st(j), 100, -0.02, 0.02, 200, phimin, phimax)
            histos[hname].g = TGraph(n)

            hname = "h_phi%i_corr0" % j
            histos[hname] = TH1F(hname, "; #phi_{%s} - #phi_{0} [rad]; Entries" % label_st(j), 200, phimin, phimax)
            hname = "h_phi%i_corr1" % j
            histos[hname] = TH1F(hname, "; #phi_{%s} - #phi_{0} [rad]; Entries" % label_st(j), 200, phimin, phimax)

        # rho
        rmin, rmax = 0, 500
        hname = "h_rho_of_z"
        histos[hname] = TH2F(hname, "; z [cm]; r [cm]", 110, 0, 1100, 200, rmin, rmax)
        histos[hname].g = TGraph(n*4)

        for j in xrange(6):
            hname = "h_rho%i_vs_p0" % j
            histos[hname] = TH2F(hname, "; q/p_{T} [1/GeV]; r_{%s} [cm]" % label_st(j), 100, -0.02, 0.02, 200, rmin, rmax)
            histos[hname].g = TGraph(n)
            hname = "h_rho%i_vs_p1" % j
            histos[hname] = TH2F(hname, "; #phi_{0} [rad]; r_{%s} [cm]" % label_st(j), 100, pi*15/180, pi*75/180, 200, rmin, rmax)
            histos[hname].g = TGraph(n)
            hname = "h_rho%i_vs_p2" % j
            histos[hname] = TH2F(hname, "; tan #theta; r_{%s} [cm]" % label_st(j), 100, 0.1, 0.9, 200, rmin, rmax)
            histos[hname].g = TGraph(n)
            hname = "h_rho%i_vs_p3" % j
            histos[hname] = TH2F(hname, "; z_{0} [cm]; r_{%s} [cm]" % label_st(j), 100, -30., 30., 200, rmin, rmax)
            histos[hname].g = TGraph(n)
            hname = "h_rho%i_vs_p4" % j
            histos[hname] = TH2F(hname, "; q/p_{z} [1/GeV]; r_{%s} [cm]" % label_st(j), 100, -0.02, 0.02, 200, rmin, rmax)
            histos[hname].g = TGraph(n)

            hname = "h_rho%i_corr0" % j
            histos[hname] = TH1F(hname, "; r_{%s} [cm]; Entries" % label_st(j), 200, rmin, rmax)
            hname = "h_rho%i_corr1" % j
            histos[hname] = TH1F(hname, "; r_{%s} [cm]; Entries" % label_st(j), 200, rmin, rmax)

        # Fill histograms
        i = 0
        for ievt, variables1, variables2, variables3, parameters1, parameters2 in izip(count(), data_var1, data_var2, data_var3, data_par1, data_par2):

            simInvPt, simPhi = parameters1
            simTanTheta, simVz = parameters2
            simInvPz = simInvPt * simTanTheta

            variables1 -= simPhi # force phi0 to be zero

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

            for j in xrange(4):
                histos["h_phi_of_z"].Fill(variables3[j], variables1[j])
                histos["h_rho_of_z"].Fill(variables3[j], variables2[j])
                histos["h_phi_of_z"].g.SetPoint(i, variables3[j], variables1[j])
                histos["h_rho_of_z"].g.SetPoint(i, variables3[j], variables2[j])
                i += 1

            # ______________________________________________________________________
            # Regression
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
                if variables3[j] < tmp_z_center[j]:  # FIXME
                    histos["h_phi%i_corr0" % j].Fill(variables1[j])
                    histos["h_rho%i_corr0" % j].Fill(variables2[j])

            # ______________________________________________________________________
            # Apply deltaZ correction
            #simC = -0.5 * (0.003 * 3.8 * simInvPt)  # 1/(2 x radius of curvature)
            #simT = 1.0
            #simC *= simTanTheta
            #simT *= simTanTheta
            #variables1 -= simC * variables3
            #variables2 -= simT * variables3

            # ______________________________________________________________________
            # Apply non-pointing strip correction
            ext_variables2 = variables3 * simTanTheta
            variables1 += np.divide(ext_variables2 - variables2, variables2) * -1 * np.tan(variables1)
            variables2 = ext_variables2

            # ______________________________________________________________________
            # After corrections
            for j in xrange(4):
                if variables3[j] > tmp_z_center[j]:  # FIXME
                    histos["h_phi%i_corr1" % j].Fill(variables1[j])
                    histos["h_rho%i_corr1" % j].Fill(variables2[j])

        print n, i

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

#!/usr/bin/env python

from rootdrawing import *
from math import sinh, atan2


# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0

l1t_l1pt_vec = (0, 1, 2)  # 3 l1pt bins
l1t_l1pt_thresholds = (0, 12, 18)
l1t_mode_vec = (0, 1, 2, 3)  # MuOpen, DoubleMu, SingleMu, Mode15
l1t_mode_integers = ((3,5,6,7,9,10,11,12,13,14,15), (7,10,11,12,13,14,15), (11,13,14,15), (15,))
l1t_bx_vec = (0, 1, 2)  # BX=any, seBX=0, BX=0
l1t_bx_integers = (range(-12,3), range(-12,3), (0,))
l1t_eta_vec = (0, 1, 2, 3)  # 3 eta bins + inclusive
l1t_eta_floats = ((1.2,1.6), (1.6,2.0), (2.0,2.4), (-99999999.,99999999.))
l1t_genpt_vec = (0, 1, 2)  # 2 genpt bins + inclusive
l1t_genpt_floats = ((20.,80.), (150.,2000.), (-99999999.,99999999.))
l1t_pu_vec = (0, 1, 2, 3)  # 4 pu bins
l1t_pu_floats = ((-0.1,0.1), (20.-1,20.+2), (30.-1,30.+2), (45.-1,45.+2))
l1t_pu_labeling = (0, 20, 30, 45)


# ______________________________________________________________________________
# Helper
def in_l1t_l1pt_bin(b, l1pt):
    a = (l1pt >= l1t_l1pt_thresholds[b])
    return a

def get_l1t_l1pt_label(b):
    label = "L1T p_{T} #geq %i" % l1t_l1pt_thresholds[b]
    return label

def in_l1t_mode_bin(b, mode):
    a = (mode in l1t_mode_integers[b])
    return a

def get_l1t_mode_label(b):
    if b == 0:
        label = "quality #geq 4"
    elif b == 1:
        label = "quality #geq 8"
    elif b == 2:
        label = "quality = 12"
    elif b == 3:
        label = "mode = 15"
    else:
        label = "ERROR"
    return label

def in_l1t_bx_bin(b, bx):
    a = (bx in l1t_bx_integers[b])
    return a

def get_l1t_bx_label(b):
    if b == l1t_bx_vec[-1]:
        label = ""
    elif b == 0:
        label = "any BX"
    elif b == 1:
        label = "seBX = 0"
    else:
        label = "ERROR"
    return label

def in_l1t_eta_bin(b, eta):
    a = (l1t_eta_floats[b][0] <= abs(eta) < l1t_eta_floats[b][1])
    return a

def get_l1t_eta_label(b):
    if b == l1t_eta_vec[-1]:
        label = ""
    else:
        label = "%.1f #leq |#eta| < %.1f" % (l1t_eta_floats[b][0], l1t_eta_floats[b][1])
    return label

def in_l1t_genpt_bin(b, genpt):
    a = (l1t_genpt_floats[b][0] <= genpt < l1t_genpt_floats[b][1])
    return a

def get_l1t_genpt_label(b):
    if b == l1t_genpt_vec[-1]:
        label = ""
    else:
        label = "%.0f #leq gen p_{T} < %.0f" % (l1t_genpt_floats[b][0], l1t_genpt_floats[b][1])
    return label

def in_l1t_pu_bin(b, pu):
    a = (l1t_pu_floats[b][0] <= pu < l1t_pu_floats[b][1])
    return a

def get_l1t_pu_label(b):
    label = "# PU = %.0f" % (l1t_pu_labeling[b])
    return label

# ______________________________________________________________________________
# Drawer
def drawer_book(histos, options):
    ##mybins = [200.0]+[100.0/x for x in xrange(1,100)]
    #mybins = [200.0]+[100.0/x for x in xrange(1,80)]
    #mybins = list(reversed(mybins))

    # Bins from Benjamin (Seoul National University) but modified
    #mybins = [0.0, 50.0, 53.0, 55.0, 60.0, 80.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 800.0, 1000.0]
    mybins = [0.0, 0.5, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 12.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60.0, 70.0, 80.0, 100.0, 125.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 800.0, 1000.0, 2000.0]

    # Efficiency
    for l1ptbin in l1t_l1pt_vec:
        for modebin in l1t_mode_vec:
            for bxbin in l1t_bx_vec:
                for etabin in l1t_eta_vec:
                    hname = "efficiency_of_pt_in_eta%i_bx%i_mode%i_l1pt%i" % (etabin, bxbin, modebin, l1ptbin)
                    histos[hname] = TEfficiency(hname, "; gen p_{T} [GeV]; #varepsilon", len(mybins)-1, array('d', mybins))
                    histos[hname].indices = (etabin, bxbin, modebin, l1ptbin)
                    hname = "efficiency_of_pu_in_eta%i_bx%i_mode%i_l1pt%i" % (etabin, bxbin, modebin, l1ptbin)
                    histos[hname] = TEfficiency(hname, "; gen # PU; #varepsilon", 56, 0, 56)
                    histos[hname].indices = (etabin, bxbin, modebin, l1ptbin)

                for genptbin in l1t_genpt_vec:
                    hname = "efficiency_of_eta_in_genpt%i_bx%i_mode%i_l1pt%i" % (genptbin, bxbin, modebin, l1ptbin)
                    #histos[hname] = TEfficiency(hname, "; gen #eta; #varepsilon", 50, -2.5, 2.5)
                    histos[hname] = TEfficiency(hname, "; gen |#eta|; #varepsilon", 52, 1.2, 2.5)
                    histos[hname].indices = (genptbin, bxbin, modebin, l1ptbin)
                    hname = "efficiency_of_phi_in_genpt%i_bx%i_mode%i_l1pt%i" % (genptbin, bxbin, modebin, l1ptbin)
                    histos[hname] = TEfficiency(hname, "; gen #phi; #varepsilon", 50, -3.2, 3.2)
                    histos[hname].indices = (genptbin, bxbin, modebin, l1ptbin)

    for pubin in l1t_pu_vec:
        for modebin in l1t_mode_vec:
            for bxbin in l1t_bx_vec:
                for etabin in l1t_eta_vec:
                    hname = "efficiency_of_pt_in_eta%i_bx%i_mode%i_pu%i" % (etabin, bxbin, modebin, pubin)
                    histos[hname] = TEfficiency(hname, "; gen p_{T} [GeV]; #varepsilon", len(mybins)-1, array('d', mybins))
                    histos[hname].indices = (etabin, bxbin, modebin, pubin)

                for genptbin in l1t_genpt_vec:
                    hname = "efficiency_of_eta_in_genpt%i_bx%i_mode%i_pu%i" % (genptbin, bxbin, modebin, pubin)
                    #histos[hname] = TEfficiency(hname, "; gen #eta; #varepsilon", 50, -2.5, 2.5)
                    histos[hname] = TEfficiency(hname, "; gen |#eta|; #varepsilon", 52, 1.2, 2.5)
                    histos[hname].indices = (genptbin, bxbin, modebin, pubin)

    # Resolution (2D)
    for etabin in l1t_eta_vec:
        hname = "l1invPt_vs_invPt_in_eta%i" % (etabin)
        histos[hname] = TH2F(hname, "; gen q/p_{T} [1/GeV]; L1T 1/p_{T} [1/GeV]", 100, -0.5, 0.5, 50, 0, 0.5)
        histos[hname].indices = (etabin,)
        hname = "dR_vs_invPt_in_eta%i" % (etabin)
        histos[hname] = TH2F(hname, "; gen q/p_{T} [1/GeV]; L1T #DeltaR @ME2", 100, -0.5, 0.5, 50, 0, 0.3)
        histos[hname].indices = (etabin,)

    # Style
    for hname, h in histos.iteritems():
        if h.ClassName() == "TH1F":
            h.style = 0; h.logx = options.logx; h.logy = options.logy
        elif h.ClassName() == "TH2F":
            h.style = 0; h.logx = False; h.logy = False
        elif h.ClassName() == "TProfile":
            h.style = 0; h.logx = False; h.logy = False
        elif h.ClassName() == "TEfficiency":
            h.style = 0; h.logx = False; h.logy = False
            h.SetConfidenceLevel(0.682689492137)  # 1-sigma
            h.SetStatisticOption(0)  # kFCP
            #h.SetStatisticOption(6)  # kBUniform
    return

# ______________________________________________________________________________
def drawer_project(tree, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("gen_trueNPV", 1)
    tree.SetBranchStatus("genParts_*", 1)
    tree.SetBranchStatus("EMTFTracks_*", 1)

    # __________________________________________________________________________
    # Loop over events
    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break
        if (ievt % 1000 == 0):  print "Processing event: %i" % ievt

        trueNPV = evt.gen_trueNPV

        part_pt = evt.genParts_pt[0]
        part_phi = evt.genParts_phi[0]
        part_eta = evt.genParts_eta[0]
        #part_cottheta = sinh(evt.genParts_eta[0])
        part_theta = atan2(evt.genParts_pt[0], evt.genParts_pz[0])
        part_charge = evt.genParts_charge[0]
        #part_isector = (part_phi * (180.0/pi) - 15.0) / 60.

        part_globalPhiME2 = evt.genParts_globalPhiME[0][2]
        part_globalEtaME2 = evt.genParts_globalEtaME[0][2]
        part_globalThetaME2 = evt.genParts_globalThetaME[0][2]

        # Select only endcap
        is_endcap = 1.25 < abs(part_eta) < 2.4

        if options.verbose:
            print ".. %i # PU: %i ntracks: %i is_endcap: %i" % (ievt, trueNPV, len(evt.EMTFTracks_pt), is_endcap)

        if part_pt > 0 and is_endcap:
            best_mode, best_dR = 0, 0.
            best_trig_pt, best_trig_phi, best_trig_eta = 0., 0., 0.

            # This is a 3D array: trigger_decisions[l1ptbin][modebin][bxbin]
            trigger_decisions = [[[False for bxbin in l1t_bx_vec] for modebin in l1t_mode_vec] for l1ptbin in l1t_l1pt_vec]

            # Loop over tracks
            for (itrack, mode, bx, sebx, trig_pt, trig_phi, trig_eta) in izip(count(), evt.EMTFTracks_mode, evt.EMTFTracks_firstBX, evt.EMTFTracks_secondBX, evt.EMTFTracks_pt, evt.EMTFTracks_phiGlbRad, evt.EMTFTracks_eta):

                if options.verbose:
                    print ".... %i gen pt: %f phi: %f eta: %f" % (itrack, part_pt, part_phi, part_eta)
                    print ".... %i l1t pt: %f phi: %f eta: %f mode: %i bx: %i sebx: %i" % (itrack, trig_pt, trig_phi, trig_eta, mode, int(bx), int(sebx))

                dR = deltaR(trig_eta, trig_phi, part_globalEtaME2, part_globalPhiME2)

                # Get trigger decisions
                for l1ptbin in l1t_l1pt_vec:
                    for modebin in l1t_mode_vec:
                        for bxbin in l1t_bx_vec:

                            trigger_mc = (dR < 0.3)
                            trigger = trigger_mc and (in_l1t_l1pt_bin(l1ptbin, trig_pt)) and (in_l1t_mode_bin(modebin, mode)) and (in_l1t_bx_bin(bxbin, bx))
                            if bxbin == 1:  # Special rule regarding sebx
                                trigger = trigger and (sebx == 0)
                            if trigger:
                                trigger_decisions[l1ptbin][modebin][bxbin] = True

                # Get best match
                if (int(bx) == 0):
                    if (best_mode < mode):
                        best_mode, best_dR = mode, dR
                        best_trig_pt, best_trig_phi, best_trig_eta = trig_pt, trig_phi, trig_eta
                    elif (best_mode == mode and best_dR > dR):
                        best_mode, best_dR = mode, dR
                        best_trig_pt, best_trig_phi, best_trig_eta = trig_pt, trig_phi, trig_eta
                continue

            this_l1t_eta_vec = filter(lambda x: in_l1t_eta_bin(x, part_eta), l1t_eta_vec)
            this_l1t_genpt_vec = filter(lambda x: in_l1t_genpt_bin(x, part_pt), l1t_genpt_vec)
            this_l1t_pu_vec = filter(lambda x: in_l1t_pu_bin(x, trueNPV), l1t_pu_vec)

            # Efficiency
            for l1ptbin in l1t_l1pt_vec:
                for modebin in l1t_mode_vec:
                    for bxbin in l1t_bx_vec:
                        trigger = trigger_decisions[l1ptbin][modebin][bxbin]

                        for etabin in this_l1t_eta_vec:
                            hname = "efficiency_of_pt_in_eta%i_bx%i_mode%i_l1pt%i" % (etabin, bxbin, modebin, l1ptbin)
                            histos[hname].Fill(trigger, part_pt)

                        if part_pt > 20.0:  # cut on denominator
                            for etabin in this_l1t_eta_vec:
                                hname = "efficiency_of_pu_in_eta%i_bx%i_mode%i_l1pt%i" % (etabin, bxbin, modebin, l1ptbin)
                                histos[hname].Fill(trigger, trueNPV)

                            for genptbin in this_l1t_genpt_vec:
                                hname = "efficiency_of_eta_in_genpt%i_bx%i_mode%i_l1pt%i" % (genptbin, bxbin, modebin, l1ptbin)
                                #histos[hname].Fill(trigger, part_eta)
                                histos[hname].Fill(trigger, abs(part_eta))
                                hname = "efficiency_of_phi_in_genpt%i_bx%i_mode%i_l1pt%i" % (genptbin, bxbin, modebin, l1ptbin)
                                histos[hname].Fill(trigger, part_phi)

            for pubin in this_l1t_pu_vec:
                for modebin in l1t_mode_vec:
                    for bxbin in l1t_bx_vec:
                        trigger = trigger_decisions[-1][modebin][bxbin]

                        for etabin in this_l1t_eta_vec:
                            hname = "efficiency_of_pt_in_eta%i_bx%i_mode%i_pu%i" % (etabin, bxbin, modebin, pubin)
                            histos[hname].Fill(trigger, part_pt)

                        if part_pt > 20.0:  # cut on denominator
                            for genptbin in this_l1t_genpt_vec:
                                hname = "efficiency_of_eta_in_genpt%i_bx%i_mode%i_pu%i" % (genptbin, bxbin, modebin, pubin)
                                #histos[hname].Fill(trigger, part_eta)
                                histos[hname].Fill(trigger, abs(part_eta))

            # Resolution (2D)
            if best_trig_pt > 0:
                for etabin in this_l1t_eta_vec:
                    hname = "l1invPt_vs_invPt_in_eta%i" % (etabin)
                    histos[hname].Fill(part_charge/part_pt, 1.0/best_trig_pt)
                    hname = "dR_vs_invPt_in_eta%i" % (etabin)
                    histos[hname].Fill(part_charge/part_pt, best_dR)
        continue
    return

# ______________________________________________________________________________
def drawer_draw(histos, options):
    for hname, h in histos.iteritems():

        if h.ClassName() == "TH1F":
            if True:
                # Style
                if h.style == 0:  # filled
                    h.SetLineWidth(2); h.SetMarkerSize(0)
                    h.SetLineColor(col); h.SetFillColor(fcol)
                elif h.style == 1:  # marker
                    h.SetLineWidth(2); h.SetMarkerStyle(20); h.SetFillStyle(0)
                    h.SetLineColor(col); h.SetMarkerColor(col)
                if h.logy:
                    h.SetMaximum(h.GetMaximum() * 14); #h.SetMinimum(0.5)
                else:
                    h.SetMaximum(h.GetMaximum() * 1.4); h.SetMinimum(0.)
                h.Draw("hist")
                gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
                CMS_label()
                save(options.outdir, hname)

        elif h.ClassName() == "TH2F":
            if True:
                h.SetStats(0); h.Draw("COLZ")
                gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
                CMS_label()
                save(options.outdir, hname)

        elif h.ClassName() == "TEfficiency":
            if True:
                ymax = 1.1
                h1 = h.GetCopyTotalHisto().Clone(hname+"_frame"); h1.Reset()
                h1.SetMinimum(0); h1.SetMaximum(ymax)
                h1.SetStats(0); h1.Draw()

                xmin, xmax = h1.GetXaxis().GetXmin(), h1.GetXaxis().GetXmax()
                tline2 = TLine(); tline2.SetLineColor(1)
                tline2.DrawLine(xmin, 1.0, xmax, 1.0)

                h.gr = h.CreateGraph()
                h.gr.SetMarkerStyle(20); h.gr.SetMarkerSize(0.8)
                h.gr.Draw("p")
                gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
                CMS_label()
                h.additional = [h.GetCopyPassedHisto(), h.GetCopyTotalHisto(), h.gr]
                save(options.outdir, hname)
    return

# ______________________________________________________________________________
def drawer_sitrep(histos, options):
    print "--- SITREP --------------------------------------------------------"
    print

    pack_histos(options.outdir, histos, options)
    return


# ______________________________________________________________________________
# Main function
def main(histos, options):
    # Init
    drawer = MyDrawer()
    tchain = TChain("ntupler/tree", "")
    tchain.AddFileInfoList(options.tfilecoll.GetList())

    # Process
    if not options.pronto:
        drawer_book(histos, options)
        drawer_project(tchain, histos, options)
        drawer_draw(histos, options)
        drawer_sitrep(histos, options)

    # Pronto
    prog = sys.argv[0]
    prog = prog.replace(".py", "_pronto.py")
    if os.path.isfile(prog):
        import subprocess
        subprocess.check_call(["python", prog, options.outdir])

# ______________________________________________________________________________
if __name__ == '__main__':

    # Setup argument parser
    parser = MyParser()
    options = parser.parse_args()
    histos = {}

    # Call the main function
    main(histos, options)

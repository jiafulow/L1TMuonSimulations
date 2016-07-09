#!/usr/bin/env python

from rootdrawing import *
from math import sinh, atan2


# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0

l1t_l1pt_vec = [0, 1, 2]  # 3 trigger pt bins
l1t_l1pt_thresholds = (10, 16, 20)
l1t_mode_vec = [0, 1, 2]  # 3-station, 4-station, 3-or-4-station
l1t_mode_values = ((11,13,14), (15,), (11,13,14,15))
l1t_eta_vec = [0, 1, 2]  # 2 eta bins + 1 inclusive eta bin
l1t_eta_bounds = (1.2, 1.6, 2.4)
l1t_pu_vec = [0, 1, 2, 3]  # 4 pu bins
l1t_pu_values = (0, 20, 30, 50)


# ______________________________________________________________________________
# Helper
def get_l1t_l1pt_bins():
    bins = l1t_l1pt_vec
    return bins

def get_l1t_l1pt_label(l1ptbin):
    label = "L1T p_{T} > %i" % l1t_l1pt_thresholds[l1ptbin]
    return label

def get_l1t_mode_bins():
    bins = l1t_mode_vec
    return bins

def get_l1t_mode_label(modebin):
    myrepr = lambda xx: '['+(','.join([str(x) for x in xx]))+']'
    label = "mode #in [%s]" % (myrepr(l1t_mode_values[modebin]))
    return label

def get_l1t_eta_bin(eta):
    eta = abs(eta)
    for i in xrange(len(l1t_eta_bounds)-1):
        if l1t_eta_bounds[i] <= abs(eta) < l1t_eta_bounds[i+1]:
            return i
    return -1

def get_l1t_eta_label(etabin):
    if etabin == len(l1t_eta_bounds)-1:  # inclusive
        label = ""
    else:
        label = "%.1f #leq |#eta| < %.1f" % (l1t_eta_bounds[etabin], l1t_eta_bounds[etabin+1])
    return label

def get_l1t_pu_bin(pu):
    for i in xrange(len(l1t_pu_values)):
        if (l1t_pu_values[i]-1) <= pu < (l1t_pu_values[i]+2):
            return i
    return -1

def get_l1t_pu_label(pubin):
    label = "# PU = %.0f" % (l1t_pu_values[pubin])
    return label

# ______________________________________________________________________________
# Drawer
def drawer_book(options):
    histos = {}

    ##mybins = [200.0]+[100.0/x for x in xrange(1,100)]
    #mybins = [200.0]+[100.0/x for x in xrange(1,80)]
    #mybins = list(reversed(mybins))

    # Bins from Benjamin (Seoul National University) but modified
    #mybins = [0.0, 50.0, 53.0, 55.0, 60.0, 80.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 800.0, 1000.0]
    mybins = [0.0, 0.5, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 12.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60.0, 80.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 800.0, 1000.0, 2000.0]

    # Efficiency
    for l1ptbin in l1t_l1pt_vec:
        for modebin in l1t_mode_vec:
            for etabin in l1t_eta_vec:
                hname = "efficiency_of_pt_in_eta%i_mode%i_l1pt%i" % (etabin, modebin, l1ptbin)
                histos[hname] = TEfficiency(hname, "; gen p_{T} [GeV]; #varepsilon", len(mybins)-1, array('d', mybins))
                histos[hname].etabin = etabin
                histos[hname].modebin = modebin
                histos[hname].l1ptbin = l1ptbin
                hname = "efficiency_of_pu_in_eta%i_mode%i_l1pt%i" % (etabin, modebin, l1ptbin)
                histos[hname] = TEfficiency(hname, "; gen # PU; #varepsilon", 65, 0, 65)
                histos[hname].etabin = etabin
                histos[hname].modebin = modebin
                histos[hname].l1ptbin = l1ptbin
            hname = "efficiency_of_eta_mode%i_l1pt%i" % (modebin, l1ptbin)
            histos[hname] = TEfficiency(hname, "; gen #eta; #varepsilon", 50, -2.5, 2.5)
            histos[hname].modebin = modebin
            histos[hname].l1ptbin = l1ptbin
            hname = "efficiency_of_phi_mode%i_l1pt%i" % (modebin, l1ptbin)
            histos[hname] = TEfficiency(hname, "; gen #phi; #varepsilon", 64, -3.2, 3.2)
            histos[hname].modebin = modebin
            histos[hname].l1ptbin = l1ptbin

    for pubin in l1t_pu_vec:
        for etabin in l1t_eta_vec:
            hname = "efficiency_of_pt_in_eta%i_pu%i" % (etabin, pubin)
            histos[hname] = TEfficiency(hname, "; gen p_{T} [GeV]; #varepsilon", len(mybins)-1, array('d', mybins))
            histos[hname].etabin = etabin
            histos[hname].pubin = pubin
        hname = "efficiency_of_eta_pu%i" % (pubin)
        histos[hname] = TEfficiency(hname, "; gen #eta; #varepsilon", 50, -2.5, 2.5)
        histos[hname].pubin = pubin
        hname = "efficiency_of_phi_pu%i" % (pubin)
        histos[hname] = TEfficiency(hname, "; gen #phi; #varepsilon", 64, -3.2, 3.2)
        histos[hname].pubin = pubin

    # Resolution (2D)
    for etabin in l1t_eta_vec:
        hname = "l1invPt_vs_invPt_in_eta%i" % (etabin)
        histos[hname] = TH2F(hname, "; gen q/p_{T} [1/GeV]; L1T 1/p_{T} [1/GeV]", 100, -0.5, 0.5, 50, 0, 0.5)
        histos[hname].etabin = etabin
        hname = "dR_vs_invPt_in_eta%i" % (etabin)
        histos[hname] = TH2F(hname, "; gen q/p_{T} [1/GeV]; L1T #DeltaR @ME2", 100, -0.5, 0.5, 50, 0, 0.3)
        histos[hname].etabin = etabin

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
    donotdelete.append(histos)
    return histos

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

        gen_trueNPV = evt.gen_trueNPV

        part_pt = evt.genParts_pt[0]
        part_phi = evt.genParts_phi[0]
        part_eta = evt.genParts_eta[0]
        #part_cottheta = sinh(evt.genParts_eta[0])
        part_theta = atan2(evt.genParts_pt[0], evt.genParts_pz[0])
        part_charge = evt.genParts_charge[0]
        part_isector = (part_phi * (180.0/pi) - 15.0) / 60.

        part_globalPhiME2 = evt.genParts_globalPhiME[0][2]
        part_globalEtaME2 = evt.genParts_globalEtaME[0][2]
        part_globalThetaME2 = evt.genParts_globalThetaME[0][2]

        # Select only endcap
        is_endcap = 1.24 < abs(part_eta) < 2.4

        if options.verbose:
            print ".. %i # PU: %i ntracks: %i is_endcap: %i" % (ievt, gen_trueNPV, len(evt.EMTFTracks_pt), is_endcap)

        if part_pt > 0 and is_endcap:
            best_mode, best_dR = 0, 0.
            best_trig_pt, best_trig_phi, best_trig_eta = 0., 0., 0.

            trigger_decisions = [[False for modebin in l1t_mode_vec] for l1ptbin in l1t_l1pt_vec]

            # Loop over tracks
            for (itrack, mode, trig_pt, trig_phi, trig_eta) in izip(count(), evt.EMTFTracks_mode, evt.EMTFTracks_pt, evt.EMTFTracks_phiGlbRad, evt.EMTFTracks_eta):

                if options.verbose:
                    print ".... %i gen pt: %f phi: %f eta: %f" % (itrack, part_pt, part_phi, part_eta)
                    print ".... %i l1t pt: %f phi: %f eta: %f" % (itrack, trig_pt, trig_phi, trig_eta)

                dR = deltaR(trig_eta, trig_phi, part_globalEtaME2, part_globalPhiME2)

                # Get trigger decisions
                for l1ptbin in l1t_l1pt_vec:
                    for modebin in l1t_mode_vec:
                        if (trig_pt > l1t_l1pt_thresholds[l1ptbin]) and (mode in l1t_mode_values[modebin]) and (dR < 0.3):
                            trigger_decisions[l1ptbin][modebin] = True

                # Get best match
                if (best_mode < mode):
                    best_mode, best_dR = mode, dR
                    best_trig_pt, best_trig_phi, best_trig_eta = trig_pt, trig_phi, trig_eta
                elif (best_mode == mode and best_dR > dR):
                    best_mode, best_dR = mode, dR
                    best_trig_pt, best_trig_phi, best_trig_eta = trig_pt, trig_phi, trig_eta
                continue

            l1ptbins = get_l1t_l1pt_bins()
            modebins = get_l1t_mode_bins()
            etabin = get_l1t_eta_bin(part_eta)
            pubin = get_l1t_pu_bin(gen_trueNPV)

            # Efficiency
            for l1ptbin in l1ptbins:
                for modebin in modebins:
                    trigger = trigger_decisions[l1ptbin][modebin]

                    hname = "efficiency_of_pt_in_eta%i_mode%i_l1pt%i" % (etabin, modebin, l1ptbin)
                    histos[hname].Fill(trigger, part_pt)
                    hname = "efficiency_of_pt_in_eta%i_mode%i_l1pt%i" % (l1t_eta_vec[-1], modebin, l1ptbin)  # inclusive eta
                    histos[hname].Fill(trigger, part_pt)
                    if part_pt > 20.0:  # cut on denominator
                        hname = "efficiency_of_pu_in_eta%i_mode%i_l1pt%i" % (etabin, modebin, l1ptbin)
                        histos[hname].Fill(trigger, gen_trueNPV)
                        hname = "efficiency_of_pu_in_eta%i_mode%i_l1pt%i" % (l1t_eta_vec[-1], modebin, l1ptbin)  # inclusive eta
                        histos[hname].Fill(trigger, gen_trueNPV)
                        hname = "efficiency_of_eta_mode%i_l1pt%i" % (modebin, l1ptbin)
                        histos[hname].Fill(trigger, part_eta)
                        hname = "efficiency_of_phi_mode%i_l1pt%i" % (modebin, l1ptbin)
                        histos[hname].Fill(trigger, part_phi)
                    continue
                continue

            trigger = trigger_decisions[-1][-1]  # last l1ptbin, last modebin

            if pubin != -1:
                hname = "efficiency_of_pt_in_eta%i_pu%i" % (etabin, pubin)
                histos[hname].Fill(trigger, part_pt)
                hname = "efficiency_of_pt_in_eta%i_pu%i" % (l1t_eta_vec[-1], pubin)  # inclusive eta
                histos[hname].Fill(trigger, part_pt)
                if part_pt > 20.0:  # cut on denominator
                    hname = "efficiency_of_eta_pu%i" % (pubin)
                    histos[hname].Fill(trigger, part_eta)
                    hname = "efficiency_of_phi_pu%i" % (pubin)
                    histos[hname].Fill(trigger, part_phi)

            # Resolution (2D)
            if best_trig_pt > 0:
                hname = "l1invPt_vs_invPt_in_eta%i" % (etabin)
                histos[hname].Fill(part_charge/part_pt, 1.0/best_trig_pt)
                hname = "l1invPt_vs_invPt_in_eta%i" % (l1t_eta_vec[-1])  # inclusive eta
                histos[hname].Fill(part_charge/part_pt, 1.0/best_trig_pt)
                hname = "dR_vs_invPt_in_eta%i" % (etabin)
                histos[hname].Fill(part_charge/part_pt, best_dR)
                hname = "dR_vs_invPt_in_eta%i" % (l1t_eta_vec[-1])  # inclusive eta
                histos[hname].Fill(part_charge/part_pt, best_dR)
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
                    h.SetLineColor(col); h.SetMarkerColor(col);
                if h.logy:
                    h.SetMaximum(h.GetMaximum() * 14); h.SetMinimum(0.5)
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
                ymax = 1.2
                h1 = h.GetCopyTotalHisto().Clone(hname+"_frame"); h1.Reset()
                h1.SetMinimum(0); h1.SetMaximum(ymax)
                h1.SetStats(0); h1.Draw()

                h.gr = h.CreateGraph()
                h.gr.SetMarkerStyle(20); h.gr.SetMarkerSize(0.7)

                h.gr.Draw("p")
                gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
                CMS_label()
                h.additional = [h.GetCopyPassedHisto(), h.GetCopyTotalHisto(), h.gr]
                save(options.outdir, hname)

    drawer_draw_more(histos, options)
    return

# ______________________________________________________________________________
def drawer_draw_more(histos, options):

    # __________________________________________________________________________
    # Specialized: overlay different pt
    special_hnames = [
        "efficiency_of_pt_in_eta0_mode0",
        "efficiency_of_pt_in_eta0_mode1",
        "efficiency_of_pt_in_eta0_mode2",
        "efficiency_of_pt_in_eta1_mode0",
        "efficiency_of_pt_in_eta1_mode1",
        "efficiency_of_pt_in_eta1_mode2",
        "efficiency_of_pt_in_eta2_mode0",
        "efficiency_of_pt_in_eta2_mode1",
        "efficiency_of_pt_in_eta2_mode2",
        "efficiency_of_pu_in_eta0_mode0",
        "efficiency_of_pu_in_eta0_mode1",
        "efficiency_of_pu_in_eta0_mode2",
        "efficiency_of_pu_in_eta1_mode0",
        "efficiency_of_pu_in_eta1_mode1",
        "efficiency_of_pu_in_eta1_mode2",
        "efficiency_of_pu_in_eta2_mode0",
        "efficiency_of_pu_in_eta2_mode1",
        "efficiency_of_pu_in_eta2_mode2",
        "efficiency_of_eta_mode0",
        "efficiency_of_eta_mode1",
        "efficiency_of_eta_mode2",
        "efficiency_of_phi_mode0",
        "efficiency_of_phi_mode1",
        "efficiency_of_phi_mode2",
    ]

    for special_hname in special_hnames:
        suffix = "_l1pt%i" % (l1t_l1pt_vec[0])
        hname = special_hname + suffix
        h = histos[hname]

        ymax = 1.2
        h1 = h.GetCopyTotalHisto().Clone(hname+"_frame"); h1.Reset()
        h1.SetMinimum(0); h1.SetMaximum(ymax)
        h1.SetStats(0); h1.Draw()

        moveLegend(tlegend,0.66,0.20,0.90,0.32); tlegend.Clear()
        for i, l1ptbin in enumerate(l1t_l1pt_vec):
            suffix = "_l1pt%i" % (l1ptbin)
            hname = special_hname + suffix
            h = histos[hname]

            assert(hasattr(h, "gr"))
            h.gr.SetMarkerColor(ctapalette[i]); h.gr.SetLineColor(ctapalette[i])
            h.gr.Draw("p")
            tlegend.AddEntry(h.gr, get_l1t_l1pt_label(h.l1ptbin), "p")
        tlegend.Draw()
        if hasattr(h, "etabin"):
            tlatex.DrawLatex(0.72, 0.34, get_l1t_eta_label(h.etabin))
        if not "efficiency_of_pt" in special_hname:  # cut on denominator
            tlatex.DrawLatex(0.72, 0.38, "gen p_{T} > 20")

        gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
        CMS_label()
        save(options.outdir, hname[:hname.find("_l1pt")]+"_l1pt")

        if "efficiency_of_pt" in special_hname:
            h1.GetXaxis().SetRangeUser(0, 200)
            save(options.outdir, hname[:hname.find("_l1pt")]+"_l1pt_zoom")

    # __________________________________________________________________________
    # Specialized: overlay different pu
    special_hnames = [
        "efficiency_of_pt_in_eta0",
        "efficiency_of_pt_in_eta1",
        "efficiency_of_pt_in_eta2",
        "efficiency_of_eta",
        "efficiency_of_phi",
    ]

    for special_hname in special_hnames:
        suffix = "_pu%i" % (l1t_pu_vec[0])
        hname = special_hname + suffix
        h = histos[hname]

        ymax = 1.2
        h1 = h.GetCopyTotalHisto().Clone(hname+"_frame"); h1.Reset()
        h1.SetMinimum(0); h1.SetMaximum(ymax)
        h1.SetStats(0); h1.Draw()

        moveLegend(tlegend,0.66,0.16,0.90,0.32); tlegend.Clear()
        for i, pubin in enumerate(l1t_pu_vec):
            suffix = "_pu%i" % (pubin)
            hname = special_hname + suffix
            h = histos[hname]

            assert(hasattr(h, "gr"))
            h.gr.SetMarkerColor(ctapalette[i]); h.gr.SetLineColor(ctapalette[i])
            h.gr.Draw("p")
            tlegend.AddEntry(h.gr, get_l1t_pu_label(h.pubin), "p")
        tlegend.Draw()
        if hasattr(h, "etabin"):
            tlatex.DrawLatex(0.72, 0.34, get_l1t_eta_label(h.etabin))
        if not "efficiency_of_pt" in special_hname:  # cut on denominator
            tlatex.DrawLatex(0.72, 0.38, "gen p_{T} > 20")

        gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
        CMS_label()
        save(options.outdir, hname[:hname.find("_pu")]+"_pu")

        if "efficiency_of_pt" in special_hname:
            h1.GetXaxis().SetRangeUser(0, 200)
            save(options.outdir, hname[:hname.find("_pu")]+"_pu_zoom")
    return

# ______________________________________________________________________________
def drawer_sitrep(histos, options):
    print "--- SITREP --------------------------------------------------------"

    for l1ptbin in l1t_l1pt_vec:
        for modebin in l1t_mode_vec:
            for etabin in l1t_eta_vec:
                hname = "efficiency_of_pt_in_eta%i_mode%i_l1pt%i" % (etabin, modebin, l1ptbin)
                h = histos[hname]
                print hname, h.GetCopyTotalHisto().GetEntries()


# ______________________________________________________________________________
# Main function
def main(options):

    # Init
    drawer = MyDrawer()
    gStyle.SetPadGridX(True)
    gStyle.SetPadGridY(True)
    tchain = TChain("ntupler/tree", "")
    tchain.AddFileInfoList(options.tfilecoll.GetList())

    # Process
    histos = drawer_book(options)
    drawer_project(tchain, histos, options)
    drawer_draw(histos, options)
    drawer_sitrep(histos, options)

# ______________________________________________________________________________
if __name__ == '__main__':

    # Setup argument parser
    parser = MyParser()
    options = parser.parse_args()

    # Call the main function
    main(options)

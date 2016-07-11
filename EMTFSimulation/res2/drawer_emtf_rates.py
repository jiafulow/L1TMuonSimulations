#!/usr/bin/env python

from rootdrawing import *
from math import sinh, atan2


# Configurations
col  = TColor.GetColor("#e31a1c")  # nu140
fcol = TColor.GetColor("#fb9a99")  # nu140

l1t_l1pt_vec = [0, 1, 2]  # 3 trigger pt bins
l1t_l1pt_thresholds = (10, 16, 20)
l1t_mode_vec = [0, 1, 2]  # 3-station, 4-station, 3-or-4-station
l1t_mode_values = ((11,13,14), (15,), (11,13,14,15))
l1t_eta_vec = [0, 1, 2]  # 2 eta bins + 1 inclusive eta bin
l1t_eta_bounds = (1.2, 1.6, 2.4)
l1t_pu_vec = [0, 1, 2, 3]  # 4 pu bins
l1t_pu_values = (10, 20, 30, 40)


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

    # Rates
    for modebin in l1t_mode_vec:
        for etabin in l1t_eta_vec:
            hname = "pt_in_eta%i_mode%i_all" % (etabin, modebin)
            histos[hname] = TH1F(hname, "; p_{T} [GeV]; entries", 100, 0, 200)
            histos[hname].etabin = etabin
            histos[hname].modebin = modebin
            hname = "ptcut_in_eta%i_mode%i_all" % (etabin, modebin)
            histos[hname] = TH1F(hname, "; cutoff p_{T} [GeV]; entries", 100, 0, 200)
            histos[hname].etabin = etabin
            histos[hname].modebin = modebin

    for pubin in l1t_pu_vec:
        for modebin in l1t_mode_vec:
            for etabin in l1t_eta_vec:
                hname = "ptcut_in_eta%i_mode%i_pu%i" % (etabin, modebin, pubin)
                histos[hname] = TH1F(hname, "; cutoff p_{T} [GeV]; arb. unit", 100, 0, 200)
                histos[hname].etabin = etabin
                histos[hname].modebin = modebin
                histos[hname].pubin = pubin

    hname = "eta"
    histos[hname] = TH1F(hname, "; #eta; entries", 50, -2.5, 2.5)
    hname = "phi"
    histos[hname] = TH1F(hname, "; #phi; entries", 64, -3.2, 3.2)

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

        if hname.startswith("pt_") or hname.startswith("ptcut_"):
            h.logy = True
    donotdelete.append(histos)
    return histos

# ______________________________________________________________________________
def drawer_project(tree, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("gen_trueNPV", 1)
    #tree.SetBranchStatus("genParts_*", 1)
    tree.SetBranchStatus("EMTFTracks_*", 1)

    def modify_ptcut(h):
        nbinsx = h.GetNbinsX()
        entries = 0
        for i in xrange(nbinsx, 0, -1):
            entries += h.GetBinContent(i)
            h.SetBinContent(i, entries)
        h.SetEntries(h.GetEntries() - nbinsx)
        return

    def normalize_by_entries(h, entries):
        if entries > 0:
            h.Scale(1.0/entries)

    # __________________________________________________________________________
    # Loop over events

    entries_in_pubin = [0 for pubin in l1t_pu_vec]

    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break
        if (ievt % 1000 == 0):  print "Processing event: %i" % ievt

        gen_trueNPV = evt.gen_trueNPV

        if options.verbose:
            print ".. %i # PU: %i ntracks: %i" % (ievt, gen_trueNPV, len(evt.EMTFTracks_pt))

        # Loop over tracks
        for (itrack, mode, bx, trig_pt, trig_phi, trig_eta) in izip(count(), evt.EMTFTracks_mode, evt.EMTFTracks_bx, evt.EMTFTracks_pt, evt.EMTFTracks_phiGlbRad, evt.EMTFTracks_eta):

            if options.verbose:
                print ".... %i l1t pt: %f phi: %f eta: %f mode: %i bx: %i" % (itrack, trig_pt, trig_phi, trig_eta, mode, int(bx))

            # Select only endcap
            is_endcap = 1.24 < abs(trig_eta) < 2.4

            if trig_pt > 0 and is_endcap:
                for modebin in l1t_mode_vec:
                    for etabin in l1t_eta_vec:
                        select = (mode in l1t_mode_values[modebin]) and ((etabin == l1t_eta_vec[-1]) or (etabin == get_l1t_eta_bin(trig_eta))) and (int(bx) == 0)
                        if select:
                            hname = "pt_in_eta%i_mode%i_all" % (etabin, modebin)
                            histos[hname].Fill(trig_pt)

                            hname = "ptcut_in_eta%i_mode%i_all" % (etabin, modebin)
                            histos[hname].Fill(trig_pt)

                        if (etabin == l1t_eta_vec[-1]) and (modebin == l1t_mode_vec[-1]):
                            hname = "eta"
                            histos[hname].Fill(trig_eta)
                            hname = "phi"
                            histos[hname].Fill(trig_phi)
                        continue
                    continue

                for pubin in l1t_pu_vec:
                    for modebin in l1t_mode_vec:
                        for etabin in l1t_eta_vec:
                            select = (mode in l1t_mode_values[modebin]) and ((etabin == l1t_eta_vec[-1]) or (etabin == get_l1t_eta_bin(trig_eta))) and (int(bx) == 0) and (pubin == get_l1t_pu_bin(gen_trueNPV))
                            if select:
                                hname = "ptcut_in_eta%i_mode%i_pu%i" % (etabin, modebin, pubin)
                                histos[hname].Fill(trig_pt)
                            continue
                        continue
                    continue

        entries_in_pubin[get_l1t_pu_bin(gen_trueNPV)] += 1

    # __________________________________________________________________________
    # Modify ptcut histograms
    for modebin in l1t_mode_vec:
        for etabin in l1t_eta_vec:
            hname = "ptcut_in_eta%i_mode%i_all" % (etabin, modebin)
            modify_ptcut(histos[hname])

    for pubin in l1t_pu_vec:
        for modebin in l1t_mode_vec:
            for etabin in l1t_eta_vec:
                hname = "ptcut_in_eta%i_mode%i_pu%i" % (etabin, modebin, pubin)
                modify_ptcut(histos[hname])

    # __________________________________________________________________________
    # Normalize histograms by entries in pubin
    for pubin in l1t_pu_vec:
        for modebin in l1t_mode_vec:
            for etabin in l1t_eta_vec:
                hname = "ptcut_in_eta%i_mode%i_pu%i" % (etabin, modebin, pubin)
                normalize_by_entries(histos[hname], entries_in_pubin[pubin])
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
                    h.SetMaximum(h.GetMaximum() * 14); #h.SetMinimum(0.5)
                else:
                    h.SetMaximum(h.GetMaximum() * 1.4); h.SetMinimum(0.)
                h.Draw("hist")

                # Customize
                if hname.startswith("pt_"):
                    for j, l1ptbin in enumerate(l1t_l1pt_vec):
                        l1pt = l1t_l1pt_thresholds[l1ptbin]
                        rate = h.Integral(h.FindBin(l1pt), h.GetNbinsX())
                        tlatex.DrawLatex(0.67, 0.70-j*0.04, "Entries w/ p_{T}>%3i: %.0f" % (l1pt, rate))
                if hname.startswith("ptcut_"):
                    for j, l1ptbin in enumerate(l1t_l1pt_vec):
                        l1pt = l1t_l1pt_thresholds[l1ptbin]
                        rate = h.GetBinContent(h.FindBin(l1pt))
                        tlatex.DrawLatex(0.67, 0.70-j*0.04, "Entries w/ p_{T}>%3i: %.0f" % (l1pt, rate))

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

                xmin, xmax = h1.GetXaxis().GetXmin(), h1.GetXaxis().GetXmax()
                tline2 = TLine(); tline.SetLineColor(1)
                tline2.DrawLine(xmin, 1.0, xmax, 1.0)

                h.gr = h.CreateGraph()
                h.gr.SetMarkerStyle(20); h.gr.SetMarkerSize(0.7)

                h.gr.Draw("p")
                gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
                CMS_label()
                h.additional = [h.GetCopyPassedHisto(), h.GetCopyTotalHisto(), h.gr]
                save(options.outdir, hname)

    drawer_draw_more(histos, options)
    save_histos(options.outdir, histos)
    return

# ______________________________________________________________________________
def drawer_draw_more(histos, options):

    # __________________________________________________________________________
    # Specialized: overlay different pu
    special_hnames = [
        "ptcut_in_eta0_mode0",
        "ptcut_in_eta0_mode1",
        "ptcut_in_eta0_mode2",
        "ptcut_in_eta1_mode0",
        "ptcut_in_eta1_mode1",
        "ptcut_in_eta1_mode2",
        "ptcut_in_eta2_mode0",
        "ptcut_in_eta2_mode1",
        "ptcut_in_eta2_mode2",
    ]

    statistics = []

    for special_hname in special_hnames:
        suffix = "_pu%i" % (l1t_pu_vec[-1])
        hname = special_hname + suffix
        h = histos[hname]
        h.SetStats(0); h.Draw("hist")

        statistics_ = [[0. for pu in l1t_pu_vec] for l1pt in l1t_l1pt_vec]

        moveLegend(tlegend,0.66,0.16,0.90,0.32); tlegend.Clear()
        for i, pubin in enumerate(l1t_pu_vec):
            suffix = "_pu%i" % (pubin)
            hname = special_hname + suffix
            h = histos[hname]
            h.SetLineWidth(2); h.SetMarkerStyle(20); h.SetFillStyle(0)
            h.SetLineColor(ctapalette[i]); h.SetMarkerColor(ctapalette[i])
            h.Draw("same hist")
            tlegend.AddEntry(h, get_l1t_pu_label(h.pubin), "l")

            for j, l1ptbin in enumerate(l1t_l1pt_vec):
                l1pt = l1t_l1pt_thresholds[l1ptbin]
                rate = h.GetBinContent(h.FindBin(l1pt))
                statistics_[l1ptbin][pubin] = rate
        tlegend.Draw()
        if hasattr(h, "etabin"):
            tlatex.DrawLatex(0.72, 0.34, get_l1t_eta_label(h.etabin))

        gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
        CMS_label()
        save(options.outdir, hname[:hname.find("_pu")]+"_pu")

        statistics.append(statistics_)

    # __________________________________________________________________________
    # Specialized: graphs

    for k in xrange(len(statistics)):
        special_hname = special_hnames[k]
        statistics_ = statistics[k]

        hname = special_hname + "_l1pt"
        h = TH1F(hname, "; # PU; arb. unit", 55, 0, 55)
        h.etabin = int(hname[:hname.find("ptcut_in_eta")+len("ptcut_in_eta")+1][-1])
        ymax = max([y for yy in statistics_ for y in yy])
        h.SetMaximum(ymax * 1.4); h.SetMinimum(0.)
        h.SetStats(0); h.Draw()

        h.graphs = []
        moveLegend(tlegend,0.16,0.80,0.40,0.92); tlegend.Clear()
        for j, l1ptbin in enumerate(l1t_l1pt_vec):
            gr = TGraph(len(statistics_[l1ptbin]))
            for i, pubin in enumerate(l1t_pu_vec):
                gr.SetPoint(i, l1t_pu_values[pubin], statistics_[l1ptbin][pubin])
            gr.SetMarkerColor(ctapalette[j]); gr.SetLineColor(ctapalette[j])
            gr.Draw("p")
            tlegend.AddEntry(gr, get_l1t_l1pt_label(l1ptbin), "p")
            h.graphs.append(gr)
        tlegend.Draw()
        if hasattr(h, "etabin"):
            tlatex.DrawLatex(0.72, 0.34, get_l1t_eta_label(h.etabin))

        gPad.SetLogx(False); gPad.SetLogy(False)
        CMS_label()
        save(options.outdir, hname)
    return

# ______________________________________________________________________________
def drawer_sitrep(histos, options):
    print "--- SITREP --------------------------------------------------------"

    for pubin in l1t_pu_vec:
        for modebin in l1t_mode_vec:
            for etabin in l1t_eta_vec:
                hname = "ptcut_in_eta%i_mode%i_pu%i" % (etabin, modebin, pubin)
                h = histos[hname]
                print hname, h.GetEntries()
    return


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

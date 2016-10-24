#!/usr/bin/env python

from rootdrawing import *
from math import sinh, atan2


# Configurations
col  = TColor.GetColor("#e31a1c")  # nu140
fcol = TColor.GetColor("#fb9a99")  # nu140

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
l1t_pu_vec = (0,)  # inclusive
l1t_pu_floats = ((-99999999.,99999999.),)
l1t_pu_labeling = (-1,)


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
    # Rates
    for pubin in l1t_pu_vec:
        for modebin in l1t_mode_vec:
            for bxbin in l1t_bx_vec:
                for etabin in l1t_eta_vec:
                    hname = "rate_of_pt_in_eta%i_bx%i_mode%i_pu%i" % (etabin, bxbin, modebin, pubin)
                    histos[hname] = TH1F(hname, "; p_{T} [GeV]; entries", 100, 0, 200)
                    histos[hname].indices = (etabin, bxbin, modebin, pubin)
                    hname = "rate_of_ptcut_in_eta%i_bx%i_mode%i_pu%i" % (etabin, bxbin, modebin, pubin)
                    histos[hname] = TH1F(hname, "; cutoff p_{T} [GeV]; entries", 100, 0, 200)
                    histos[hname].indices = (etabin, bxbin, modebin, pubin)

    for l1ptbin in l1t_l1pt_vec:
        for modebin in l1t_mode_vec:
            for bxbin in l1t_bx_vec:
                for etabin in l1t_eta_vec:
                    hname = "rate_of_pu_in_eta%i_bx%i_mode%i_l1pt%i" % (etabin, bxbin, modebin, l1ptbin)
                    histos[hname] = TH1F(hname, "; gen # PU; entries", 56, 0, 56)
                    histos[hname].indices = (etabin, bxbin, modebin, l1ptbin)

                hname = "rate_of_eta_in_bx%i_mode%i_l1pt%i" % (bxbin, modebin, l1ptbin)
                #histos[hname] = TH1F(hname, "; #eta; entries", 50, -2.5, 2.5)
                histos[hname] = TH1F(hname, "; |#eta|; entries", 52, 1.2, 2.5)
                histos[hname].indices = (bxbin, modebin, l1ptbin)
                hname = "rate_of_phi_in_bx%i_mode%i_l1pt%i" % (bxbin, modebin, l1ptbin)
                histos[hname] = TH1F(hname, "; #phi; entries", 64, -3.2, 3.2)
                histos[hname].indices = (bxbin, modebin, l1ptbin)

            hname = "rate_of_bx_in_mode%i_l1pt%i" % (modebin, l1ptbin)
            histos[hname] = TH1F(hname, "; BX; entries", 5, -2.5, 2.5)
            histos[hname].indices = (bxbin, modebin, l1ptbin)
            hname = "rate_of_sebx_in_mode%i_l1pt%i" % (modebin, l1ptbin)
            histos[hname] = TH1F(hname, "; seBX; entries", 5, -2.5, 2.5)
            histos[hname].indices = (bxbin, modebin, l1ptbin)

    hname = "trueNPV"
    histos[hname] = TH1F(hname, "; gen # PU; entries", 56, 0, 56)

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

        if hname.startswith("rate_of_pt_") or hname.startswith("rate_of_ptcut_"):
            h.logy = True
    return

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

    # __________________________________________________________________________
    # Loop over events
    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break
        if (ievt % 1000 == 0):  print "Processing event: %i" % ievt

        trueNPV = evt.gen_trueNPV

        if options.verbose:
            print ".. %i # PU: %i ntracks: %i" % (ievt, trueNPV, len(evt.EMTFTracks_pt))

        # Loop over tracks
        for (itrack, mode, bx, sebx, trig_pt, trig_phi, trig_eta) in izip(count(), evt.EMTFTracks_mode, evt.EMTFTracks_firstBX, evt.EMTFTracks_secondBX, evt.EMTFTracks_pt, evt.EMTFTracks_phiGlbRad, evt.EMTFTracks_eta):

            if options.verbose:
                print ".... %i l1t pt: %f phi: %f eta: %f mode: %i bx: %i sebx: %i" % (itrack, trig_pt, trig_phi, trig_eta, mode, int(bx), int(sebx))

            # Select only endcap
            is_endcap = 1.25 < abs(trig_eta) < 2.4

            if trig_pt > 0 and is_endcap:
                for pubin in l1t_pu_vec:
                    for modebin in l1t_mode_vec:
                        for bxbin in l1t_bx_vec:
                            for etabin in l1t_eta_vec:
                                select = (in_l1t_pu_bin(pubin, trueNPV)) and (in_l1t_mode_bin(modebin, mode)) and (in_l1t_bx_bin(bxbin, bx)) and (in_l1t_eta_bin(etabin, trig_eta))
                                if bxbin == 1:  # Special rule regarding sebx
                                    select = select and (sebx == 0)
                                if select:
                                    hname = "rate_of_pt_in_eta%i_bx%i_mode%i_pu%i" % (etabin, bxbin, modebin, pubin)
                                    histos[hname].Fill(trig_pt)
                                    hname = "rate_of_ptcut_in_eta%i_bx%i_mode%i_pu%i" % (etabin, bxbin, modebin, pubin)
                                    histos[hname].Fill(trig_pt)

                for l1ptbin in l1t_l1pt_vec:
                    for modebin in l1t_mode_vec:
                        for bxbin in l1t_bx_vec:
                            for etabin in l1t_eta_vec:
                                select = (in_l1t_l1pt_bin(l1ptbin, trig_pt)) and (in_l1t_mode_bin(modebin, mode)) and (in_l1t_bx_bin(bxbin, bx)) and (in_l1t_eta_bin(etabin, trig_eta))
                                if bxbin == 1:  # Special rule regarding sebx
                                    select = select and (sebx == 0)
                                if select:
                                    hname = "rate_of_pu_in_eta%i_bx%i_mode%i_l1pt%i" % (etabin, bxbin, modebin, l1ptbin)
                                    histos[hname].Fill(trueNPV)

                                    if etabin == l1t_eta_vec[-1]:  # inclusive
                                        hname = "rate_of_eta_in_bx%i_mode%i_l1pt%i" % (bxbin, modebin, l1ptbin)
                                        #histos[hname].Fill(trig_eta)
                                        histos[hname].Fill(abs(trig_eta))
                                        hname = "rate_of_phi_in_bx%i_mode%i_l1pt%i" % (bxbin, modebin, l1ptbin)
                                        histos[hname].Fill(trig_phi)

                                        if bxbin == 0:  # inclusive
                                            hname = "rate_of_bx_in_mode%i_l1pt%i" % (modebin, l1ptbin)
                                            histos[hname].Fill(bx)
                                            hname = "rate_of_sebx_in_mode%i_l1pt%i" % (modebin, l1ptbin)
                                            histos[hname].Fill(sebx)
                continue

        hname = "trueNPV"
        histos[hname].Fill(trueNPV)
        continue

    # __________________________________________________________________________
    # Modify ptcut histograms
    for pubin in l1t_pu_vec:
        for modebin in l1t_mode_vec:
            for bxbin in l1t_bx_vec:
                for etabin in l1t_eta_vec:
                    hname = "rate_of_ptcut_in_eta%i_bx%i_mode%i_pu%i" % (etabin, bxbin, modebin, pubin)
                    modify_ptcut(histos[hname])
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

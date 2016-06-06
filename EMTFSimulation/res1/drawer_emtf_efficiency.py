#!/usr/bin/env python

from rootdrawing import *
from math import sinh, atan2


# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0

mode_vec = [11, 13, 14, 15] # excluded 7

l1t_pt_vec = [10, 16, 20]  # trigger pt
l1t_eta_vec = [0, 1, 2, 3]  # 4 trigger eta bins
l1t_eta_bounds = (1.2, 1.5, 1.8, 2.1, 2.4)


# ______________________________________________________________________________
# Helper
def get_l1t_eta_bin(eta):
    if eta < l1t_eta_bounds[0] or eta >= l1t_eta_bounds[-1]:
        return -1
    for i in xrange(1,len(l1t_eta_bounds)):
        if eta < l1t_eta_bounds[i]:
            return i-1

# ______________________________________________________________________________
# Drawer
def drawer_book(options):
    histos = {}

    #mybins = [200.0]+[100.0/x for x in xrange(1,100)]
    mybins = [200.0]+[100.0/x for x in xrange(1,80)]
    mybins = list(reversed(mybins))

    # Efficiency
    for l1eta in l1t_eta_vec:
        for l1pt in l1t_pt_vec:
            hname = "emtf_efficiency_pt_l1eta%i_l1pt%i" % (l1eta, l1pt)
            histos[hname] = TEfficiency(hname, "; gen p_{T} [GeV]; #varepsilon", len(mybins)-1, array('d', mybins))

    # Resolution (2D)
    for l1eta in l1t_eta_vec:
        hname = "emtf_resolution_2d_pt_l1eta%i" % (l1eta)
        #histos[hname] = TH2F(hname, "; q/p_{T} [GeV]; L1T q/p_{T} [GeV]", 100, -0.5, 0.5, 100, -0.5, 0.5)
        histos[hname] = TH2F(hname, "; gen q/p_{T} [1/GeV]; L1T q/p_{T} [1/GeV]", 100, -0.5, 0.5, 50, 0, 0.5)

        hname = "emtf_resolution_2d_dR_l1eta%i" % (l1eta)
        histos[hname] = TH2F(hname, "; gen q/p_{T} [1/GeV]; L1T #DeltaR @ME2", 100, -0.5, 0.5, 50, 0, 0.3)

    # Style
    for hname, h in histos.iteritems():
        if h.ClassName() == "TH1F":
            h.style = 0; h.logx = options.logx; h.logy = options.logy
        elif h.ClassName() == "TH2F":
            h.style = 0; h.logx = False; h.logy = False
        elif h.ClassName() == "TProfile":
            h.style = 0; h.logx = False; h.logy = False
        elif h.ClassName() == "TEfficiency":
            h.style = 0; h.logx = True; h.logy = False
            h.SetConfidenceLevel(0.682689492137)  # 1-sigma
            h.SetStatisticOption(0)  # kFCP
            #h.SetStatisticOption(6)  # kBUniform
    donotdelete.append(histos)
    return histos

# ______________________________________________________________________________
def drawer_project(tree, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("genParts_*", 1)
    tree.SetBranchStatus("CSCTracks_itracks_*", 1)

    # Loop over events
    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break
        if (ievt % 1000 == 0):  print "Processing event: %i" % ievt

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

        is_endcap = 1.24 < part_eta < 2.4

        if part_pt > 0 and is_endcap:

            for (mode, pt, globalPhi, globalEta) in izip(evt.CSCTracks_itracks_mode, evt.CSCTracks_itracks_pt, evt.CSCTracks_itracks_globalPhi, evt.CSCTracks_itracks_globalEta):

                #is_endcap = 1.24 < globalEta < 2.4
                #is_ok = pt > 2
                #trigger = (mode in mode_vec) and ((1.0/pt - 1.0/part_pt) < (0.05*5))

                dR = deltaR(globalEta, globalPhi, part_globalEtaME2, part_globalPhiME2)
                trigger = (mode in mode_vec) and (dR < 0.3)

                l1eta = get_l1t_eta_bin(part_eta)
                for l1pt in l1t_pt_vec:
                    if pt > l1pt:
                        hname = "emtf_efficiency_pt_l1eta%i_l1pt%i" % (l1eta, l1pt)
                        histos[hname].Fill(trigger, part_pt)

                # Only one trigger track
                if len(evt.CSCTracks_itracks_pt) == 1:
                    hname = "emtf_resolution_2d_pt_l1eta%i" % (l1eta)
                    histos[hname].Fill(part_charge/part_pt, 1.0/pt)

                    hname = "emtf_resolution_2d_dR_l1eta%i" % (l1eta)
                    histos[hname].Fill(part_charge/part_pt, dR)
    return

# ______________________________________________________________________________
def drawer_draw(histos, options):
    for hname, h in histos.iteritems():

        if h.ClassName() == "TH1F":
            if True:
                # Draw TH1 plots
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
                h1 = h.GetCopyTotalHisto().Clone(hname+"_frame")
                h1.Reset()
                h1.SetMinimum(0); h1.SetMaximum(ymax)
                h1.SetStats(0); h1.Draw()

                # Reference lines for 0.9, 0.95 and 1.0
                xmin, xmax = h1.GetXaxis().GetXmin(), h1.GetXaxis().GetXmax()
                for y in [0.5, 0.8, 0.9, 0.95, 1.0]:
                    tline.DrawLine(xmin, y, xmax, y)

                h.gr = h.CreateGraph()
                h.gr.SetMarkerStyle(20)
                for i in xrange(len(l1t_pt_vec)):
                    if ("l1pt%i" % l1t_pt_vec[i]) in hname:
                        h.gr.SetMarkerColor(ctapalette[i]); h.gr.SetLineColor(ctapalette[i])
                h.gr.Draw("p")

                gPad.SetLogx(h.logx)
                CMS_label()
                h.additional = [h.GetCopyPassedHisto(), h.GetCopyTotalHisto(), h.gr]
                save(options.outdir, hname)

    # Specialized: overlay different pt
    for l1eta in l1t_eta_vec:
        hname = "emtf_efficiency_pt_l1eta%i_l1pt%i" % (l1eta, l1t_pt_vec[0])
        h = histos[hname]
        h1 = h.GetCopyTotalHisto().Clone(h1.GetName()+"_frame")
        h1.Reset()
        h1.SetMinimum(0); h1.SetMaximum(ymax)
        h1.SetStats(0); h1.Draw()

        # Reference lines for 0.9, 0.95 and 1.0
        xmin, xmax = h1.GetXaxis().GetXmin(), h1.GetXaxis().GetXmax()
        for y in [0.5, 0.8, 0.9, 0.95, 1.0]:
            tline.DrawLine(xmin, y, xmax, y)

        moveLegend(tlegend,0.66,0.20,0.90,0.32); tlegend.Clear()
        for l1pt in l1t_pt_vec:
            hname = "emtf_efficiency_pt_l1eta%i_l1pt%i" % (l1eta, l1pt)
            h = histos[hname]
            h.gr.Draw("p")
            tlegend.AddEntry(h.gr, "trigger p_{T} > %i" % l1pt, "p")
        tlegend.Draw()
        tlatex.DrawLatex(0.72, 0.34, "%.1f #leq #eta < %.1f" % (l1t_eta_bounds[l1eta], l1t_eta_bounds[l1eta+1]))

        gPad.SetLogx(h.logx)
        CMS_label()
        save(options.outdir, hname[:hname.find("_l1pt")])
    return

# ______________________________________________________________________________
def drawer_sitrep(histos, options):
    print "--- SITREP --------------------------------------------------------"
    print


# ______________________________________________________________________________
# Main function
def main(options):

    # Init
    drawer = MyDrawer()
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

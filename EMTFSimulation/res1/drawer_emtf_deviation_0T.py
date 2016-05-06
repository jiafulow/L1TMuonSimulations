#!/usr/bin/env python

from rootdrawing import *
from math import sinh, atan2


# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0

palette = map(lambda x: TColor.GetColor(x), ("#66DD66", "#3333FF", "#990099", "#FFBB44", "#EE4477"))

pt_vec = [3,5,10,20,50]
st_vec = [11,12,14,21,22,31,32,41,42]  # excluded ME+1/3


# ______________________________________________________________________________
# Drawer
def drawer_book(options):
    histos = {}

    def label_st(st):
        d = {10: 'ME+1/1', 11: 'ME+1/1b', 12: 'ME+1/2', 13: 'ME+1/3', 14: 'ME+1/1a', 20: 'ME+2', 21: 'ME+2/1', 22: 'ME+2/2', 30: 'ME+3', 31: 'ME+3/1', 32: 'ME+3/2', 40: 'ME+4', 41: 'ME+4/1', 42: 'ME+4/2'}
        ret = d[st]
        if options.east:
            ret = d[st].replace("ME+", "ME-")
        return ret

    for pt in pt_vec:
        for st in st_vec:
            nbinsx, xmin, xmax = 60, -0.15, 0.15
            hname = "emtf_dphi_st%i_pt%i" % (st, pt)
            histos[hname] = TH1F(hname, "; #phi(muon) - #phi(%s) [rad]" % (label_st(st)), nbinsx, xmin, xmax)
            nbinsx, xmin, xmax = 50, -0.1, 0.1
            hname = "emtf_dtheta_st%i_pt%i" % (st, pt)
            histos[hname] = TH1F(hname, "; #theta(muon) - #theta(%s) [rad]" % (label_st(st)), nbinsx, xmin, xmax)
            nbinsx, xmin, xmax = 50, -0.2, 0.2
            hname = "emtf_deta_st%i_pt%i" % (st, pt)
            histos[hname] = TH1F(hname, "; #eta(muon) - #eta(%s)" % (label_st(st)), nbinsx, xmin, xmax)

    # Style
    for hname, h in histos.iteritems():
        h.style = 1; h.logx = options.logx; h.logy = options.logy
        h.logy = True  # always log-y
    donotdelete.append(histos)
    return histos

# ______________________________________________________________________________
def drawer_project(tree, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("genParts_*", 1)
    tree.SetBranchStatus("CSCStubs_*", 1)

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

        pt = 0
        for x in pt_vec:
            if 0.98 < part_pt/float(x) < 1.02:
                pt = x

        is_endcap = 1.24 < part_eta < 2.4

        if pt > 0 and is_endcap:
            for (istation, iring, globalPhi, globalTheta, globalEta) in izip(evt.CSCStubs_istation, evt.CSCStubs_iring, evt.CSCStubs_globalPhi, evt.CSCStubs_globalTheta, evt.CSCStubs_globalEta):

                if istation == 1 and iring == 3:  # skip ME+1/3
                    continue

                st = istation * 10 + iring

                dphi = deltaPhi(part_phi, globalPhi)
                dtheta = deltaPhi(part_theta, globalTheta)
                deta = deltaEta(part_eta, globalEta)

                hname = "emtf_dphi_st%i_pt%i" % (st, pt)
                histos[hname].Fill(dphi)
                hname = "emtf_dtheta_st%i_pt%i" % (st, pt)
                histos[hname].Fill(dtheta)
                hname = "emtf_deta_st%i_pt%i" % (st, pt)
                histos[hname].Fill(deta)
    return

# ______________________________________________________________________________
def drawer_draw(histos, options):

    tlegend2 = TLegend(0.70,0.74,0.96,0.94)
    tlegend2.SetFillStyle(0)
    tlegend2.SetLineColor(0)
    tlegend2.SetShadowColor(0)
    tlegend2.SetBorderSize(0)
    tlegend2.SetTextFont(42)

    def display_fit(h):
        h.Fit("gaus","q")
        h.fit = h.GetFunction("gaus")
        h.fit.SetLineWidth(2); h.fit.SetLineColor(darken_color(h.GetLineColor(),20))

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
                    h.SetMaximum(h.GetMaximum() * 100); h.SetMinimum(0.5)
                else:
                    h.SetMaximum(h.GetMaximum() * 1.5); h.SetMinimum(0.)

                h.Draw("E")
                display_fit(h)
                gPad.SetLogy(h.logy)
                CMS_0T_label()
                save(options.outdir, hname)


    # Specialized: overlay different pT
    if True:
        for var in ["dphi", "dtheta", "deta"]:
            for st in st_vec:
                pt = 50
                hname = "emtf_%s_st%i_pt%i" % (var, st, pt)
                h = histos[hname]
                hframe = h.Clone(h.GetName().replace("_pt%i" % pt, "_overlay"))
                hframe.Reset()
                hframe.SetStats(0); hframe.Draw()

                moveLegend(tlegend,0.12,0.68,0.55,0.92); tlegend.Clear()
                for i, pt in enumerate(pt_vec):
                    hname = "emtf_%s_st%i_pt%i" % (var, st, pt)
                    h = histos[hname]
                    h.SetLineColor(palette[i]); h.SetMarkerColor(palette[i]); h.fit.SetLineColor(palette[i])
                    h.SetStats(0); h.Draw("same E")
                    tlegend.AddEntry(h, "#color[%i]{#sigma(p_{T} = %i GeV) = %.4f}" % (palette[i], pt, h.fit.GetParameter(2)), "")
                tlegend.Draw()

                gPad.SetLogy(h.logy)
                CMS_0T_label()
                save(options.outdir, hframe.GetName())
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
    parser.add_argument("--east", action="store_true", help="Draw for East (i.e. negative) endcap (default: %(default)s)")
    options = parser.parse_args()

    # Call the main function
    main(options)

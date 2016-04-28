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
        d = {11: 'ME+1/1b', 12: 'ME+1/2', 13: 'ME+1/3', 14: 'ME+1/1a', 21: 'ME+2/1', 22: 'ME+2/2', 31: 'ME+3/1', 32: 'ME+3/2', 41: 'ME+4/1', 42: 'ME+4/2'}
        return d[st]

    for pt in pt_vec:
        for st in st_vec:
            nbinsx, xmin, xmax = 50, -0.1, 0.1

            hname = "stubs_dphi_st%i_pt%i" % (st, pt)
            histos[hname] = TH1F(hname, "; #phi(Gen) - #phi(Emu) [rad], %s" % (label_st(st)), nbinsx, xmin, xmax)

            hname = "stubs_deta_st%i_pt%i" % (st, pt)
            histos[hname] = TH1F(hname, "; #eta(Gen) - #eta(Emu), %s" % (label_st(st)), nbinsx, xmin, xmax)

    # Style
    for hname, h in histos.iteritems():
        h.style = 1; h.logx = options.logx; h.logy = options.logy
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

        pt = 0
        for x in pt_vec:
            if 0.98 < part_pt/float(x) < 1.02:
                pt = x

        is_endcap = 1.25 < part_eta < 2.4

        if pt > 0 and is_endcap:
            for (istation, iring, globalPhi, globalEta) in izip(evt.CSCStubs_istation, evt.CSCStubs_iring, evt.CSCStubs_globalPhi, evt.CSCStubs_globalEta):
                st = istation * 10 + iring

                dphi = deltaPhi(part_phi, globalPhi)
                deta = part_eta - globalEta

                if st in st_vec and pt in pt_vec:
                    hname = "stubs_dphi_st%i_pt%i" % (st, pt)
                    histos[hname].Fill(dphi)
                    hname = "stubs_deta_st%i_pt%i" % (st, pt)
                    histos[hname].Fill(deta)
    return

# ______________________________________________________________________________
def drawer_draw(histos, options):
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
                    h.SetMaximum(h.GetMaximum() * 1.4); h.SetMinimum(0.)

                h.Draw("E")
                display_fit(h)
                gPad.SetLogy(h.logy)

                CMS_0T_label()
                save(options.outdir, hname)


    # Specialized: overlay different pT
    for st in st_vec:

        # phi residuals
        pt = 50
        hname = "stubs_dphi_st%i_pt%i" % (st, pt)
        h = histos[hname]
        hframe = h.Clone(h.GetName().replace("_pt%i" % pt, "_overlay"))
        hframe.Reset()
        hframe.SetStats(0); hframe.Draw()

        moveLegend(0.12,0.68,0.55,0.92); tlegend.Clear()
        for i, pt in enumerate(pt_vec):
            hname = "stubs_dphi_st%i_pt%i" % (st, pt)
            h = histos[hname]
            h.SetLineColor(palette[i]); h.SetMarkerColor(palette[i]); h.fit.SetLineColor(palette[i])

            h.SetStats(0); h.Draw("same E")
            tlegend.AddEntry(h, "#color[%i]{#sigma(p_{T} = %i GeV) = %.4f}" % (palette[i], pt, h.fit.GetParameter(2)), "")
        tlegend.Draw()

        gPad.SetLogy(h.logy)
        CMS_0T_label()
        save(options.outdir, hframe.GetName())

        # eta residuals
        pt = 50
        hname = "stubs_deta_st%i_pt%i" % (st, pt)
        h = histos[hname]
        hframe = h.Clone(h.GetName().replace("_pt%i" % pt, "_overlay"))
        hframe.Reset()
        hframe.SetStats(0); hframe.Draw()

        moveLegend(0.12,0.68,0.55,0.92); tlegend.Clear()
        for i, pt in enumerate(pt_vec):
            hname = "stubs_deta_st%i_pt%i" % (st, pt)
            h = histos[hname]
            h.SetLineColor(palette[i]); h.SetMarkerColor(palette[i]); h.fit.SetLineColor(palette[i])

            h.SetStats(0); h.Draw("same E")
            tlegend.AddEntry(h, "#color[%i]{#sigma(p_{T} = %i GeV) = %.5f}" % (palette[i], pt, h.fit.GetParameter(2)), "")
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
    parser.set_defaults(logy=True)

    options = parser.parse_args()

    # Call the main function
    main(options)

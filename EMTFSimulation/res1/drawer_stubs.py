#!/usr/bin/env python

from rootdrawing import *
from math import sinh, atan2
from ROOT import Double


# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0
mcol = TColor.GetColor("#600000")
#ecol = TColor.GetColor("#FFD600")
ecol = TColor.GetColor("#322D8C")

palette = map(lambda x: TColor.GetColor(x), ("#66DD66", "#3333FF", "#990099", "#FFBB44", "#EE4477"))

pt_vec = [3,5,10,20,50]
st_vec = [11,12,14,21,22,31,32,41,42]  # excluded ME+1/3
std_vec = [210,211,212,230,240,310,311,312,340,410,411,412]


# ______________________________________________________________________________
# Helper
def get_integrated(h, forward=True, normalize=False):
    hintegrated = h.Clone(h.GetName() + "_integrated" + ("f" if forward else "b"))
    hintegrated.Reset()
    nbins = h.GetNbinsX()
    if forward:
        entries = 0
        for i in xrange(1, nbins+1):
            entries += h.GetBinContent(i)
            hintegrated.SetBinContent(i, entries)
        if normalize and entries > 0:
            hintegrated.Scale(1.0/entries)
    else:
        entries = 0
        for i in xrange(nbins, 0, -1):
            entries += h.GetBinContent(i)
            hintegrated.SetBinContent(i, entries)
        if normalize and entries > 0:
            hintegrated.Scale(1.0/entries)
    return hintegrated

def get_percentile(h, perc):
    assert(0 <= perc <= 1)
    forward = perc >= 0.5
    hintegrated = get_integrated(h, forward=forward, normalize=True)
    nbins = h.GetNbinsX()
    if forward:
        for i in xrange(nbins, 0, -1):
            if hintegrated.GetBinContent(i) < perc:
                break
        q = hintegrated.GetBinLowEdge(i+1)
        dint = hintegrated.GetBinContent(i+1) - hintegrated.GetBinContent(i)
        if dint > 0:
            q += hintegrated.GetBinWidth(i+1) * (perc - hintegrated.GetBinContent(i))/dint
    else:
        for i in xrange(1, nbins+1):
            if hintegrated.GetBinContent(i) < (1-perc):
                break
        q = hintegrated.GetBinLowEdge(i)
        dint = hintegrated.GetBinContent(i-1) - hintegrated.GetBinContent(i)
        if dint > 0:
            q -= hintegrated.GetBinWidth(i) * ((1-perc) - hintegrated.GetBinContent(i))/dint
    return q

# ______________________________________________________________________________
# Drawer
def drawer_book(options):
    histos = {}

    def label_st(st):
        d = {10: 'ME+1/1', 11: 'ME+1/1b', 12: 'ME+1/2', 13: 'ME+1/3', 14: 'ME+1/1a', 20: 'ME+2', 21: 'ME+2/1', 22: 'ME+2/2', 30: 'ME+3', 31: 'ME+3/1', 32: 'ME+3/2', 40: 'ME+4', 41: 'ME+4/1', 42: 'ME+4/2'}
        return d[st]

    def label_std_i(std):
        d = {210: 'ME+2', 211: 'ME+2', 212: 'ME+2', 230: 'ME+2', 240: 'ME+2', 310: 'ME+3', 311: 'ME+3', 312: 'ME+3', 340: 'ME+3', 410: 'ME+4', 411: 'ME+4', 412: 'ME+4'}
        return d[std]

    def label_std_j(std):
        d = {210: 'ME+1', 211: 'ME+1/1', 212: 'ME+1/2', 230: 'ME+3', 240: 'ME+4', 310: 'ME+1', 311: 'ME+1/1', 312: 'ME+1/2', 340: 'ME+4', 410: 'ME+1', 411: 'ME+1/1', 412: 'ME+1/2'}
        return d[std]

    # TH1F
    for pt in pt_vec:
        for st in st_vec:
            nbinsx, xmin, xmax = 60, -1.1, 0.1
            hname = "stubs_dphi_st%i_pt%i" % (st, pt)
            histos[hname] = TH1F(hname, "; #phi(Gen) - #phi(Emu) [rad], %s" % (label_st(st)), nbinsx, xmin, xmax)
            nbinsx, xmin, xmax = 60, -0.6, 0.6
            hname = "stubs_deta_st%i_pt%i" % (st, pt)
            histos[hname] = TH1F(hname, "; #eta(Gen) - #eta(Emu), %s" % (label_st(st)), nbinsx, xmin, xmax)

        for std in std_vec:

            if pt == 3:
                nbinsx, xmin, xmax = 60, -0.3, 0.3
            elif pt == 5:
                nbinsx, xmin, xmax = 60, -0.2, 0.2
            else:
                nbinsx, xmin, xmax = 60, -0.1, 0.1

            hname = "stubs_dphi_std%i_pt%i" % (std, pt)
            histos[hname] = TH1F(hname, "; #phi(%s) - #phi(%s) [rad]" % (label_std_i(std), label_std_j(std)), nbinsx, xmin, xmax)
            hname = "stubs_deta_std%i_pt%i" % (std, pt)
            histos[hname] = TH1F(hname, "; #eta(%s) - #eta(%s)" % (label_std_i(std), label_std_j(std)), nbinsx, xmin, xmax)

    # TH2F
    for pt in pt_vec:
        for std in std_vec:

            if pt == 3:
                nbinsx, xmin, xmax = 60, -0.3, 0.3
            elif pt == 5:
                nbinsx, xmin, xmax = 60, -0.2, 0.2
            else:
                nbinsx, xmin, xmax = 60, -0.1, 0.1
            nbinsy, ymin, ymax = 16, 0.9, 2.5

            hname = "stubs_dphi_vs_eta_std%i_pt%i" % (std, pt)
            histos[hname] = TH2F(hname, "; #eta(Gen); #phi(%s) - #phi(%s) [rad]" % (label_std_i(std), label_std_j(std)), nbinsy, ymin, ymax, nbinsx, xmin, xmax)
            hname = "stubs_deta_vs_eta_std%i_pt%i" % (std, pt)
            histos[hname] = TH2F(hname, "; #eta(Gen); #eta(%s) - #eta(%s) [rad]" % (label_std_i(std), label_std_j(std)), nbinsy, ymin, ymax, nbinsx, xmin, xmax)

            # error bar indicates std dev
            hname = "stubs_dphi_pr_eta_std%i_pt%i" % (std, pt)
            histos[hname] = TProfile(hname, "; #eta(Gen); #phi(%s) - #phi(%s) [rad]" % (label_std_i(std), label_std_j(std)), nbinsy, ymin, ymax, xmin, xmax, 's')
            hname = "stubs_deta_pr_eta_std%i_pt%i" % (std, pt)
            histos[hname] = TProfile(hname, "; #eta(Gen); #eta(%s) - #eta(%s) [rad]" % (label_std_i(std), label_std_j(std)), nbinsy, ymin, ymax, xmin, xmax, 's')

    # Style
    for hname, h in histos.iteritems():
        if h.ClassName() == "TH1F":
            h.style = 0; h.logx = options.logx; h.logy = options.logy
        elif h.ClassName() == "TH2F":
            h.style = 0; h.logx = False; h.logy = False
        elif h.ClassName() == "TProfile":
            h.style = 0; h.logx = False; h.logy = False
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

        is_endcap = 1.25 < part_eta < 2.4

        if pt > 0 and is_endcap:
            for (istation, iring, globalPhi, globalEta) in izip(evt.CSCStubs_istation, evt.CSCStubs_iring, evt.CSCStubs_globalPhi, evt.CSCStubs_globalEta):
                st = istation * 10 + iring

                dphi = deltaPhi(part_phi, globalPhi)
                deta = part_eta - globalEta

                if part_charge == 1:  # flip dphi for antimuons
                    dphi = -dphi

                if st in st_vec and pt in pt_vec:
                    hname = "stubs_dphi_st%i_pt%i" % (st, pt)
                    histos[hname].Fill(dphi)
                    hname = "stubs_deta_st%i_pt%i" % (st, pt)
                    histos[hname].Fill(deta)

            for imuon1, (istation1, iring1, globalPhi1, globalEta1) in enumerate(izip(evt.CSCStubs_istation, evt.CSCStubs_iring, evt.CSCStubs_globalPhi, evt.CSCStubs_globalEta)):
                for imuon2, (istation2, iring2, globalPhi2, globalEta2) in enumerate(izip(evt.CSCStubs_istation, evt.CSCStubs_iring, evt.CSCStubs_globalPhi, evt.CSCStubs_globalEta)):

                    if imuon1 >= imuon2:
                        continue
                    if istation1 == istation2:  # only if different station
                        continue

                    # If station 2 is available, use as the 'key' station,
                    # else if station 3 is avail, use it,
                    # else if station 4 is avail, use it.
                    if istation1 == 2:
                        dphi = deltaPhi(globalPhi1, globalPhi2)
                        deta = globalEta1 - globalEta2
                        std = istation1 * 100 + istation2 * 10
                    elif istation2 == 2:
                        dphi = deltaPhi(globalPhi2, globalPhi1)
                        deta = globalEta2 - globalEta1
                        std = istation2 * 100 + istation1 * 10
                    elif istation1 == 3:
                        dphi = deltaPhi(globalPhi1, globalPhi2)
                        deta = globalEta1 - globalEta2
                        std = istation1 * 100 + istation2 * 10
                    elif istation2 == 3:
                        dphi = deltaPhi(globalPhi2, globalPhi1)
                        deta = globalEta2 - globalEta1
                        std = istation2 * 100 + istation1 * 10
                    elif istation1 == 4:
                        dphi = deltaPhi(globalPhi1, globalPhi2)
                        deta = globalEta1 - globalEta2
                        std = istation1 * 100 + istation2 * 10
                    elif istation2 == 4:
                        dphi = deltaPhi(globalPhi2, globalPhi1)
                        deta = globalEta2 - globalEta1
                        std = istation2 * 100 + istation1 * 10
                    else:
                        raise ValueError("Unexpected pair: istation1=%i istation2=%i" % (istation1, istation2))

                    std_more = [std]
                    if istation1 == 1 and iring1 in [1,2]:
                        std_1 = std + iring1
                        std_more.append(std_1)
                    elif istation2 == 1 and iring2 in [1,2]:
                        std_1 = std + iring2
                        std_more.append(std_1)

                    if part_charge == 1:  # flip dphi for antimuons
                        dphi = -dphi

                    for std in std_more:
                        if std in std_vec and pt in pt_vec:
                            hname = "stubs_dphi_std%i_pt%i" % (std, pt)
                            histos[hname].Fill(dphi)
                            hname = "stubs_deta_std%i_pt%i" % (std, pt)
                            histos[hname].Fill(deta)

                            hname = "stubs_dphi_vs_eta_std%i_pt%i" % (std, pt)
                            histos[hname].Fill(part_eta, dphi)
                            hname = "stubs_deta_vs_eta_std%i_pt%i" % (std, pt)
                            histos[hname].Fill(part_eta, deta)

                            hname = "stubs_dphi_pr_eta_std%i_pt%i" % (std, pt)
                            histos[hname].Fill(part_eta, dphi)
                            hname = "stubs_deta_pr_eta_std%i_pt%i" % (std, pt)
                            histos[hname].Fill(part_eta, deta)
    return

# ______________________________________________________________________________
def drawer_draw(histos, options):

    average = lambda x, y: 0.5*(x+y)
    absdiff = lambda x, y: abs(x-y)

    tlatex2 = TLatex()
    tlatex2.SetNDC(False)
    tlatex2.SetTextFont(62)
    tlatex2.SetTextSize(0.03)
    tlatex2.SetTextColor(0)
    tlatex2.SetTextAngle(90)
    tlatex2.SetTextAlign(22)

    def display_percentile_y(h2):
        frame90 = h2.Clone(h2.GetName() + "_frame90")
        frame95 = h2.Clone(h2.GetName() + "_frame95")
        frame99 = h2.Clone(h2.GetName() + "_frame99")
        frame90.Reset()
        frame95.Reset()
        frame99.Reset()
        frame90.GetYaxis().SetTitle("90% cov on " + frame90.GetYaxis().GetTitle())
        frame95.GetYaxis().SetTitle("95% cov on " + frame95.GetYaxis().GetTitle())
        frame99.GetYaxis().SetTitle("99% cov on " + frame99.GetYaxis().GetTitle())

        nbinsx = h2.GetNbinsX()
        gr90 = TGraphErrors(nbinsx)
        gr95 = TGraphErrors(nbinsx)
        gr99 = TGraphErrors(nbinsx)
        gr90.SetName(h2.GetName() + "_gr90")
        gr95.SetName(h2.GetName() + "_gr95")
        gr99.SetName(h2.GetName() + "_gr99")

        for i in xrange(1, nbinsx+1):
            hpy = h2.ProjectionY(h2.GetName() + "_py", i, i)
            # 90% coverage
            up90 = get_percentile(hpy, 0.95)
            low90 = get_percentile(hpy, 0.05)
            # 95% coverage
            up95 = get_percentile(hpy, 0.975)
            low95 = get_percentile(hpy, 0.025)
            # 99% coverage
            up99 = get_percentile(hpy, 0.995)
            low99 = get_percentile(hpy, 0.005)

            if hpy.GetEntries() > 20:
                gr90.SetPoint(i, h2.GetXaxis().GetBinCenter(i), average(up90,low90))
                gr90.SetPointError(i, 0.5*h2.GetXaxis().GetBinWidth(i), 0.5*absdiff(up90,low90))
                gr95.SetPoint(i, h2.GetXaxis().GetBinCenter(i), average(up95,low95))
                gr95.SetPointError(i, 0.5*h2.GetXaxis().GetBinWidth(i), 0.5*absdiff(up95,low95))
                gr99.SetPoint(i, h2.GetXaxis().GetBinCenter(i), average(up99,low99))
                gr99.SetPointError(i, 0.5*h2.GetXaxis().GetBinWidth(i), 0.5*absdiff(up99,low99))
        h2.additional = [frame90, frame95, frame99, gr90, gr95, gr99]
        return

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
                gPad.SetLogy(h.logy)
                CMS_label()
                save(options.outdir, hname)

        elif h.ClassName() == "TH2F":
            pass

        elif h.ClassName() == "TProfile":
            if True:
                # Draw profile plots on top of TH2
                h2 = histos[hname.replace("_pr_", "_vs_")]
                if h.style == 0:
                    h.SetMarkerStyle(20); h.SetMarkerColor(mcol); h.SetLineColor(mcol)

                h2.SetStats(0); h2.Draw("COLZ")
                h.SetStats(0); h.Draw("same")
                gPad.SetLogy(h.logy)
                CMS_label()
                save(options.outdir, hname)

            if True:
                # Draw coverage plots
                h2 = histos[hname.replace("_pr_", "_vs_")]
                display_percentile_y(h2)

                frame90, frame95, frame99, gr90, gr95, gr99 = h2.additional

                for (frame, gr) in [(frame90, gr90), (frame95, gr95), (frame99, gr99)]:
                    frame.GetYaxis().SetTitleSize(0.05)
                    gr.SetFillColor(ecol)

                    frame.SetStats(0); frame.Draw()
                    gr.Draw("2")  # filled error band

                    nbinsx = gr.GetN()
                    for i in xrange(nbinsx):
                        x, y = Double(1), Double(2)
                        gr.GetPoint(i, x, y)
                        ey = gr.GetErrorY(i)
                        if ey > 0:
                            tlatex2.DrawLatex(x, y, "%.3f" % (ey*2))

                    gPad.SetLogy(h.logy)
                    CMS_label()
                    save(options.outdir, gr.GetName())

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
    #parser.set_defaults(logy=True)

    options = parser.parse_args()

    # Call the main function
    main(options)

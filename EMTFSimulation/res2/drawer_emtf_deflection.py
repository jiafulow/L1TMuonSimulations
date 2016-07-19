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

palette = map(lambda x: TColor.GetColor(x), ("#66DD66", "#3333FF", "#990099", "#FFBB44", "#EE4477", "#56D3DB", "#454545"))

pt_vec = [3,5,10,20,50,200,1000]
st_vec = [11,12,14,21,22,31,32,41,42]  # excluded ME+1/3
std_vec = [2010,2011,2012,2030,2040,3010,3011,3012,3040,4010,4011,4012]


# ______________________________________________________________________________
# Helper
def label_st(st):
    d = {10: 'ME+1/1', 11: 'ME+1/1b', 12: 'ME+1/2', 13: 'ME+1/3', 14: 'ME+1/1a', 20: 'ME+2', 21: 'ME+2/1', 22: 'ME+2/2', 30: 'ME+3', 31: 'ME+3/1', 32: 'ME+3/2', 40: 'ME+4', 41: 'ME+4/1', 42: 'ME+4/2'}
    ret = d[st]
    if options.east:
        ret = d[st].replace("ME+", "ME-")
    return ret

def label_std_i(std):
    d = {2010: 'ME+2', 2011: 'ME+2', 2012: 'ME+2', 2030: 'ME+2', 2040: 'ME+2', 3010: 'ME+3', 3011: 'ME+3', 3012: 'ME+3', 3040: 'ME+3', 4010: 'ME+4', 4011: 'ME+4', 4012: 'ME+4'}
    ret = d[std]
    if options.east:
        ret = d[std].replace("ME+", "ME-")
    return ret

def label_std_j(std):
    d = {2010: 'ME+1', 2011: 'ME+1/1', 2012: 'ME+1/2', 2030: 'ME+3', 2040: 'ME+4', 3010: 'ME+1', 3011: 'ME+1/1', 3012: 'ME+1/2', 3040: 'ME+4', 4010: 'ME+1', 4011: 'ME+1/1', 4012: 'ME+1/2'}
    ret = d[std]
    if options.east:
        ret = d[std].replace("ME+", "ME-")
    return ret

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
def drawer_book(histos, options):
    # TH1F
    for pt in pt_vec:
        for st in st_vec:
            nbinsx, xmin, xmax = 60, -1.1, 0.1
            hname = "emtf_dphi_st%i_pt%i" % (st, pt)
            histos[hname] = TH1F(hname, "; #phi(muon) - #phi(%s) [rad]" % (label_st(st)), nbinsx, xmin, xmax)
            nbinsx, xmin, xmax = 60, -0.6, 0.6
            hname = "emtf_dtheta_st%i_pt%i" % (st, pt)
            histos[hname] = TH1F(hname, "; #theta(muon) - #theta(%s) [rad]" % (label_st(st)), nbinsx, xmin, xmax)
            nbinsx, xmin, xmax = 60, -0.6, 0.6
            hname = "emtf_deta_st%i_pt%i" % (st, pt)
            histos[hname] = TH1F(hname, "; #eta(muon) - #eta(%s)" % (label_st(st)), nbinsx, xmin, xmax)

        for std in std_vec:
            if options.high_pt:
                nbinsx, xmin, xmax = 50, -0.02, 0.02
            else:
                nbinsx, xmin, xmax = 100, -0.2, 0.2
            hname = "emtf_dphi_std%i_pt%i" % (std, pt)
            histos[hname] = TH1F(hname, "; #phi(%s) - #phi(%s) [rad]" % (label_std_i(std), label_std_j(std)), nbinsx, xmin, xmax)
            if options.high_pt:
                nbinsx, xmin, xmax = 50, -0.02, 0.02
            else:
                nbinsx, xmin, xmax = 50, -0.1, 0.1
            hname = "emtf_dtheta_std%i_pt%i" % (std, pt)
            histos[hname] = TH1F(hname, "; #theta(%s) - #theta(%s) [rad]" % (label_std_i(std), label_std_j(std)), nbinsx, xmin, xmax)
            if options.high_pt:
                nbinsx, xmin, xmax = 50, -0.04, 0.04
            else:
                nbinsx, xmin, xmax = 50, -0.2, 0.2
            hname = "emtf_deta_std%i_pt%i" % (std, pt)
            histos[hname] = TH1F(hname, "; #eta(%s) - #eta(%s)" % (label_std_i(std), label_std_j(std)), nbinsx, xmin, xmax)

    # TH2F and TProfile
    for pt in pt_vec:
        for std in std_vec:
            nbinsy, ymin, ymax = 16, 0.9, 2.5
            nbinsx, xmin, xmax = 100, -0.2, 0.2
            hname = "emtf_dphi_vs_eta_std%i_pt%i" % (std, pt)
            histos[hname] = TH2F(hname, "; #eta(Gen); #phi(%s) - #phi(%s) [rad]" % (label_std_i(std), label_std_j(std)), nbinsy, ymin, ymax, nbinsx, xmin, xmax)
            nbinsx, xmin, xmax = 50, -0.1, 0.1
            hname = "emtf_dtheta_vs_eta_std%i_pt%i" % (std, pt)
            histos[hname] = TH2F(hname, "; #eta(Gen); #theta(%s) - #theta(%s) [rad]" % (label_std_i(std), label_std_j(std)), nbinsy, ymin, ymax, nbinsx, xmin, xmax)
            nbinsx, xmin, xmax = 50, -0.2, 0.2
            hname = "emtf_deta_vs_eta_std%i_pt%i" % (std, pt)
            histos[hname] = TH2F(hname, "; #eta(Gen); #eta(%s) - #eta(%s)" % (label_std_i(std), label_std_j(std)), nbinsy, ymin, ymax, nbinsx, xmin, xmax)

            # error bar indicates std dev
            nbinsy, ymin, ymax = 16, 0.9, 2.5
            nbinsx, xmin, xmax = 100, -0.2, 0.2
            hname = "emtf_dphi_pr_eta_std%i_pt%i" % (std, pt)
            histos[hname] = TProfile(hname, "; #eta(Gen); #phi(%s) - #phi(%s) [rad]" % (label_std_i(std), label_std_j(std)), nbinsy, ymin, ymax, xmin, xmax, 's')
            nbinsx, xmin, xmax = 50, -0.1, 0.1
            hname = "emtf_dtheta_pr_eta_std%i_pt%i" % (std, pt)
            histos[hname] = TProfile(hname, "; #eta(Gen); #theta(%s) - #theta(%s) [rad]" % (label_std_i(std), label_std_j(std)), nbinsy, ymin, ymax, xmin, xmax, 's')
            nbinsx, xmin, xmax = 50, -0.2, 0.2
            hname = "emtf_deta_pr_eta_std%i_pt%i" % (std, pt)
            histos[hname] = TProfile(hname, "; #eta(Gen); #eta(%s) - #eta(%s)" % (label_std_i(std), label_std_j(std)), nbinsy, ymin, ymax, xmin, xmax, 's')

    # Style
    for hname, h in histos.iteritems():
        if h.ClassName() == "TH1F":
            h.style = 0; h.logx = options.logx; h.logy = options.logy
        elif h.ClassName() == "TH2F":
            h.style = 0; h.logx = False; h.logy = False
        elif h.ClassName() == "TProfile":
            h.style = 0; h.logx = False; h.logy = False

        if hname.startswith("emtf_dphi") or hname.startswith("emtf_dtheta") or hname.startswith("emtf_deta"):
            h.logy = True
    return

# ______________________________________________________________________________
def drawer_project(tree, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("genParts_*", 1)
    tree.SetBranchStatus("CSCStubs_*", 1)

    # __________________________________________________________________________
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

        # Select only endcap
        is_endcap = 1.25 < abs(part_eta) < 2.4

        if pt > 0 and is_endcap:
            for (istation, iring, strip, globalPhi, globalTheta, globalEta) in izip(evt.CSCStubs_istation, evt.CSCStubs_iring, evt.CSCStubs_strip, evt.CSCStubs_globalPhi, evt.CSCStubs_globalTheta, evt.CSCStubs_globalEta):
                st = istation * 10 + iring

                dphi = deltaPhi(part_phi, globalPhi)
                dtheta = deltaPhi(part_theta, globalTheta)
                deta = deltaEta(part_eta, globalEta)

                if part_charge == 1:  # flip dphi for antimuons
                    dphi = -dphi

                if st in st_vec and pt in pt_vec:
                    hname = "emtf_dphi_st%i_pt%i" % (st, pt)
                    histos[hname].Fill(dphi)
                    hname = "emtf_dtheta_st%i_pt%i" % (st, pt)
                    histos[hname].Fill(dtheta)
                    hname = "emtf_deta_st%i_pt%i" % (st, pt)
                    histos[hname].Fill(deta)

            for imuon1, t1 in enumerate(izip(evt.CSCStubs_istation, evt.CSCStubs_iring, evt.CSCStubs_strip, evt.CSCStubs_globalPhi, evt.CSCStubs_globalTheta, evt.CSCStubs_globalEta)):
                for imuon2, t2 in enumerate(izip(evt.CSCStubs_istation, evt.CSCStubs_iring, evt.CSCStubs_strip, evt.CSCStubs_globalPhi, evt.CSCStubs_globalTheta, evt.CSCStubs_globalEta)):

                    if imuon1 >= imuon2:
                        continue

                    (istation1, iring1, strip1, globalPhi1, globalTheta1, globalEta1) = t1
                    (istation2, iring2, strip2, globalPhi2, globalTheta2, globalEta2) = t2
                    if istation1 == istation2:  # only if different station
                        continue
                    if istation1 == 1 and iring1 == 3:  # skip ME+1/3
                        continue
                    if istation2 == 1 and iring2 == 3:  # skip ME+1/3
                        continue

                    # If station 2 is available, use as the 'key' station,
                    # else if station 3 is avail, use it,
                    # else if station 4 is avail, use it.
                    std1 = istation1 * 1000 + istation2 * 10
                    std2 = istation2 * 1000 + istation1 * 10
                    if std1 in std_vec:
                        pass
                    elif std2 in std_vec:
                        (istation2, iring2, strip2, globalPhi2, globalTheta2, globalEta2) = t1
                        (istation1, iring1, strip1, globalPhi1, globalTheta1, globalEta1) = t2
                    else:
                        raise Exception("Unexpected station1: %i, station2: %i" % (istation1, istation2))

                    std = istation1 * 1000 + istation2 * 10
                    std_more = [std]
                    if istation2 == 1 and iring2 in [1,2]:
                        std_1 = std + iring2
                        std_more.append(std_1)

                    dphi = deltaPhi(globalPhi1, globalPhi2)
                    dtheta = deltaPhi(globalTheta1, globalTheta2)
                    deta = deltaEta(globalEta1, globalEta2)

                    if part_charge == 1:  # flip dphi for antimuons
                        dphi = -dphi

                    for std in std_more:
                        hname = "emtf_dphi_std%i_pt%i" % (std, pt)
                        histos[hname].Fill(dphi)
                        hname = "emtf_dtheta_std%i_pt%i" % (std, pt)
                        histos[hname].Fill(dtheta)
                        hname = "emtf_deta_std%i_pt%i" % (std, pt)
                        histos[hname].Fill(deta)

                        hname = "emtf_dphi_vs_eta_std%i_pt%i" % (std, pt)
                        histos[hname].Fill(part_eta, dphi)
                        hname = "emtf_dtheta_vs_eta_std%i_pt%i" % (std, pt)
                        histos[hname].Fill(part_eta, dtheta)
                        hname = "emtf_deta_vs_eta_std%i_pt%i" % (std, pt)
                        histos[hname].Fill(part_eta, deta)

                        hname = "emtf_dphi_pr_eta_std%i_pt%i" % (std, pt)
                        histos[hname].Fill(part_eta, dphi)
                        hname = "emtf_dtheta_pr_eta_std%i_pt%i" % (std, pt)
                        histos[hname].Fill(part_eta, dtheta)
                        hname = "emtf_deta_pr_eta_std%i_pt%i" % (std, pt)
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

    tlegend2 = TLegend(0.70,0.74,0.96,0.94)
    tlegend2.SetFillStyle(0)
    tlegend2.SetLineColor(0)
    tlegend2.SetShadowColor(0)
    tlegend2.SetBorderSize(0)
    tlegend2.SetTextFont(42)

    def display_fit(h):
        if h.Integral() > 0:
            h.Fit("gaus","q")
            h.fit = h.GetFunction("gaus")
            h.fit.SetLineWidth(2); h.fit.SetLineColor(darken_color(h.GetLineColor(),20))
        else:
            h.fit = TF1("fa1", "gaus(0)")

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

    # __________________________________________________________________________
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
                    h.SetMaximum(h.GetMaximum() * 100); h.SetMinimum(0.5)
                else:
                    h.SetMaximum(h.GetMaximum() * 1.4); h.SetMinimum(0.)
                h.Draw("E")
                display_fit(h)
                gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
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

    # Specialized: overlay different pT
    if True:
        for var in ["dphi", "dtheta", "deta"]:
            for std in std_vec:
                pt = 50
                hname = "emtf_%s_std%i_pt%i" % (var, std, pt)
                h = histos[hname]
                hframe = h.Clone(h.GetName().replace("_pt%i" % pt, "_overlay"))
                hframe.Reset()
                hframe.SetStats(0); hframe.Draw()

                moveLegend(tlegend,0.1,0.68,0.5,0.92); tlegend.Clear()
                moveLegend(tlegend2,0.5,0.68,0.92,0.92); tlegend2.Clear()
                for i, pt in enumerate(pt_vec):
                    if options.high_pt and pt < 50:  continue
                    if (not options.high_pt) and pt >50:  continue

                    hname = "emtf_%s_std%i_pt%i" % (var, std, pt)
                    h = histos[hname]
                    h.SetLineColor(palette[i]); h.SetMarkerColor(palette[i]); h.fit.SetLineColor(palette[i])
                    h.SetStats(0); h.Draw("same E")
                    tlegend.AddEntry(h, "#color[%i]{#mu(p_{T} = %i GeV) = %.4f}" % (palette[i], pt, h.fit.GetParameter(1)), "")
                    tlegend2.AddEntry(h, "#color[%i]{#sigma(p_{T} = %i GeV) = %.4f}" % (palette[i], pt, h.fit.GetParameter(2)), "")
                tlegend.Draw()
                tlegend2.Draw()

                gPad.SetLogy(h.logy)
                CMS_label()
                save(options.outdir, hframe.GetName())

    # Specialized: coverage plots
    if False:
        for hname, h2 in histos.iteritems():
            if h2.ClassName() == "TH2F":
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
    parser.add_argument("--east", action="store_true", help="Draw for East (i.e. negative) endcap (default: %(default)s)")
    parser.add_argument("--high-pt", action="store_true", help="Draw for high-pt (i.e. 50 GeV and above) (default: %(default)s)")
    options = parser.parse_args()
    histos = {}

    # Call the main function
    main(histos, options)

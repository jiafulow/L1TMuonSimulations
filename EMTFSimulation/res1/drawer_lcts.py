#!/usr/bin/env python

from rootdrawing import *
from math import sinh, atan2


# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0
mcol = TColor.GetColor("#600000")

palette = map(lambda x: TColor.GetColor(x), ("#66DD66", "#3333FF", "#990099", "#FFBB44", "#EE4477"))

pt_vec = [3,5,10,20,50]
st_vec = [11,12,14,21,22,31,32,41,42]  # excluded ME+1/3
std_vec = [210,211,212,230,240,310,311,312,340,410,411,412]


# ______________________________________________________________________________
# Drawer
def drawer_book(options):
    histos = {}

    def label_st(st):
        d = {11: 'ME+1/1b', 12: 'ME+1/2', 13: 'ME+1/3', 14: 'ME+1/1a', 21: 'ME+2/1', 22: 'ME+2/2', 31: 'ME+3/1', 32: 'ME+3/2', 41: 'ME+4/1', 42: 'ME+4/2'}
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
            hname = "lcts_dphi_st%i_pt%i" % (st, pt)
            histos[hname] = TH1F(hname, "; #phi(Gen) - #phi(Emu) [rad], %s" % (label_st(st)), nbinsx, xmin, xmax)
            nbinsx, xmin, xmax = 60, -0.6, 0.6
            hname = "lcts_deta_st%i_pt%i" % (st, pt)
            histos[hname] = TH1F(hname, "; #eta(Gen) - #eta(Emu), %s" % (label_st(st)), nbinsx, xmin, xmax)

        for std in std_vec:

            if pt == 3:
                nbinsx, xmin, xmax = 60, -0.3, 0.3
            elif pt == 5:
                nbinsx, xmin, xmax = 60, -0.2, 0.2
            else:
                nbinsx, xmin, xmax = 60, -0.1, 0.1

            hname = "lcts_dphi_std%i_pt%i" % (std, pt)
            histos[hname] = TH1F(hname, "; #phi(%s) - #phi(%s) [rad]" % (label_std_i(std), label_std_j(std)), nbinsx, xmin, xmax)
            hname = "lcts_deta_std%i_pt%i" % (std, pt)
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

            hname = "lcts_dphi_vs_eta_std%i_pt%i" % (std, pt)
            histos[hname] = TH2F(hname, "; #eta(Gen); #phi(%s) - #phi(%s) [rad]" % (label_std_i(std), label_std_j(std)), nbinsy, ymin, ymax, nbinsx, xmin, xmax)
            hname = "lcts_deta_vs_eta_std%i_pt%i" % (std, pt)
            histos[hname] = TH2F(hname, "; #eta(Gen); #eta(%s) - #eta(%s) [rad]" % (label_std_i(std), label_std_j(std)), nbinsy, ymin, ymax, nbinsx, xmin, xmax)

            # error bar indicates std dev
            hname = "lcts_dphi_pr_eta_std%i_pt%i" % (std, pt)
            histos[hname] = TProfile(hname, "; #eta(Gen); #phi(%s) - #phi(%s) [rad]" % (label_std_i(std), label_std_j(std)), nbinsy, ymin, ymax, xmin, xmax, 's')
            hname = "lcts_deta_pr_eta_std%i_pt%i" % (std, pt)
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
    #tree.SetBranchStatus("*", 0)

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
            for (istation, iring, globalPhi, globalEta) in izip(evt.cscCLCTs_istation, evt.cscCLCTs_iring, evt.cscCLCTs_globalPhi, evt.cscCLCTs_globalEta):
                st = istation * 10 + iring

                dphi = deltaPhi(part_phi, globalPhi)
                deta = part_eta - globalEta

                if part_charge == 1:  # flip dphi for antimuons
                    dphi = -dphi

                if st in st_vec and pt in pt_vec:
                    hname = "lcts_dphi_st%i_pt%i" % (st, pt)
                    histos[hname].Fill(dphi)
                    hname = "lcts_deta_st%i_pt%i" % (st, pt)
                    histos[hname].Fill(deta)

            for imuon1, (istation1, iring1, globalPhi1, globalEta1) in enumerate(izip(evt.cscCLCTs_istation, evt.cscCLCTs_iring, evt.cscCLCTs_globalPhi, evt.cscCLCTs_globalEta)):
                for imuon2, (istation2, iring2, globalPhi2, globalEta2) in enumerate(izip(evt.cscCLCTs_istation, evt.cscCLCTs_iring, evt.cscCLCTs_globalPhi, evt.cscCLCTs_globalEta)):

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
                            hname = "lcts_dphi_std%i_pt%i" % (std, pt)
                            histos[hname].Fill(dphi)
                            hname = "lcts_deta_std%i_pt%i" % (std, pt)
                            histos[hname].Fill(deta)

                            hname = "lcts_dphi_vs_eta_std%i_pt%i" % (std, pt)
                            histos[hname].Fill(part_eta, dphi)
                            hname = "lcts_deta_vs_eta_std%i_pt%i" % (std, pt)
                            histos[hname].Fill(part_eta, deta)

                            hname = "lcts_dphi_pr_eta_std%i_pt%i" % (std, pt)
                            histos[hname].Fill(part_eta, dphi)
                            hname = "lcts_deta_pr_eta_std%i_pt%i" % (std, pt)
                            histos[hname].Fill(part_eta, deta)

    #tree.SetBranchStatus("*", 1)
    return

# ______________________________________________________________________________
def drawer_draw(histos, options):

    for hname, h in histos.iteritems():

        if h.ClassName() == "TH1F":
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
            h2 = histos[hname.replace("_pr_", "_vs_")]
            h2.SetStats(0); h2.Draw("COLZ")

            if h.style == 0:
                h.SetMarkerStyle(20); h.SetMarkerColor(mcol); h.SetLineColor(mcol)

            h.SetStats(0); h.Draw("same")
            gPad.SetLogy(h.logy)

            CMS_label()
            save(options.outdir, hname)

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

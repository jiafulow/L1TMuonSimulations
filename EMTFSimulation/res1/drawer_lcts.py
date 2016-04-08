#!/usr/bin/env python

from rootdrawing import *
from math import sinh, atan2


# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0

palette = map(lambda x: TColor.GetColor(x), ("#66DD66", "#3333FF", "#990099", "#FFBB44", "#EE4477"))

pt_vec = [3,5,10,20,50]
st_vec = [11,12,14,21,22,31,32,41,42]  # excluded ME+1/3
std_vec = [12,13,14,23,24,34]


# ______________________________________________________________________________
# Drawer
def drawer_book(options):
    histos = {}

    def label_st(st):
        d = {11: 'ME+1/1b', 12: 'ME+1/2', 13: 'ME+1/3', 14: 'ME+1/1a', 21: 'ME+2/1', 22: 'ME+2/2', 31: 'ME+3/1', 32: 'ME+3/2', 41: 'ME+4/1', 42: 'ME+4/2'}
        return d[st]

    def label_std_i(std):
        d = {12: 'ME2', 13: 'ME3', 14: 'ME4', 23: 'ME2', 24: 'ME2', 34: 'ME3'}
        return d[std]

    def label_std_j(std):
        d = {12: 'ME1', 13: 'ME1', 14: 'ME1', 23: 'ME3', 24: 'ME4', 34: 'ME4'}
        return d[std]

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


    #FIXME: make 2D plots

    # Style
    for hname, h in histos.iteritems():
        h.style = 0; h.logx = options.logx; h.logy = options.logy
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
                    std_1 = istation1 * 10 + istation2
                    if std_1 in [21,23,24] + [31,34] + [41]:
                        dphi = deltaPhi(globalPhi1, globalPhi2)
                        deta = globalEta1 - globalEta2
                    elif std_1 in [12,13,14] + [32] + [42,43]:
                        dphi = deltaPhi(globalPhi2, globalPhi1)
                        deta = globalEta2 - globalEta1
                    else:
                        raise ValueError("Unexpected 'std': %i" % std)

                    if std_1 in [21,31,41,32,42,43]:
                        std = ((std_1%10) * 10) + (std_1/10)
                    else:
                        std = std_1

                    if part_charge == 1:  # flip dphi for antimuons
                        dphi = -dphi

                    hname = "lcts_dphi_std%i_pt%i" % (std, pt)
                    histos[hname].Fill(dphi)
                    hname = "lcts_deta_std%i_pt%i" % (std, pt)
                    histos[hname].Fill(deta)

    #tree.SetBranchStatus("*", 1)
    return

# ______________________________________________________________________________
def drawer_draw(histos, options):

    for hname, h in histos.iteritems():

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

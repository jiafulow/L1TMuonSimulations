#!/usr/bin/env python

from rootdrawing import *
from math import sinh, atan2


# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0

#pt_vec = [3,5,10,20,50]
#st_vec = [11,12,14,21,22,31,32,41,42]  # excluded ME+1/3
st_vec_1 = [10, 12, 20, 30, 40]  # excluded ME+1/3
std_vec = [210,211,212,230,240,310,311,312,340,410,411,412]


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
    for st in st_vec_1:
        nbinsx, xmin, xmax = 50, -0.012, -0.004
        hname = "emtf_residual_convGlobalPhi_st%i" % (st)
        histos[hname] = TH1F(hname, "; #phi(%s) - FW #phi(%s) [rad]" % (label_st(st), label_st(st)), nbinsx, xmin, xmax)

        nbinsx, xmin, xmax = 50, -0.015, 0.065
        hname = "emtf_residual_convGlobalTheta_st%i" % (st)
        histos[hname] = TH1F(hname, "; #theta(%s) - FW #theta(%s) [rad]" % (label_st(st), label_st(st)), nbinsx, xmin, xmax)

    hname = "genParts_pt"
    histos[hname] = TH1F(hname, "; gen p_{T} [GeV]", 100, 0, 2000)
    hname = "genParts_invPt"
    histos[hname] = TH1F(hname, "; gen q/p_{T} [1/GeV]", 100, -0.5, 0.5)
    hname = "genParts_eta"
    histos[hname] = TH1F(hname, "; gen #eta", 60, -3, 3)
    hname = "genParts_phi"
    histos[hname] = TH1F(hname, "; gen #phi [rad]", 64, -3.2, 3.2)


    # Style
    for hname, h in histos.iteritems():
        if h.ClassName() == "TH1F":
            h.style = 0; h.logx = options.logx; h.logy = options.logy
            if hname in ["genParts_pt"]:
                h.logy = True
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

        is_endcap = 1.25 < part_eta < 2.4

        if part_pt > 0 and is_endcap:

            hname = "genParts_pt"
            histos[hname].Fill(part_pt)
            hname = "genParts_invPt"
            histos[hname].Fill(part_charge/part_pt)
            hname = "genParts_eta"
            histos[hname].Fill(part_eta)
            hname = "genParts_phi"
            histos[hname].Fill(part_phi)

            for t in izip(evt.CSCStubs_istation, evt.CSCStubs_iring, evt.CSCStubs_globalPhi, evt.CSCStubs_globalTheta, evt.CSCStubs_globalEta, evt.CSCStubs_convGlobalPhi, evt.CSCStubs_convGlobalTheta, evt.CSCStubs_convGlobalEta):

                (istation, iring, globalPhi, globalTheta, globalEta, convGlobalPhi, convGlobalTheta, convGlobalEta) = t

                #st = istation * 10 + iring
                st = istation * 10
                if istation == 1 and iring == 2:
                    st = istation * 10 + iring

                dphi = deltaPhi(globalPhi, convGlobalPhi)
                dtheta = deltaPhi(globalTheta, convGlobalTheta)
                #deta = deltaEta(globalEta, convGlobalEta)

                if st in st_vec_1:
                    hname = "emtf_residual_convGlobalPhi_st%i" % (st)
                    histos[hname].Fill(dphi)
                    hname = "emtf_residual_convGlobalTheta_st%i" % (st)
                    histos[hname].Fill(dtheta)
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
    options = parser.parse_args()

    # Call the main function
    main(options)

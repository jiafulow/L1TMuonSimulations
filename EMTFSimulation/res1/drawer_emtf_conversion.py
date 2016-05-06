#!/usr/bin/env python

from rootdrawing import *
from math import sinh, atan2


# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0

#pt_vec = [3,5,10,20,50]
st_vec = [11,12,14,21,22,31,32,41,42]  # excluded ME+1/3
st0_vec = [11,12,20,30,40]  # excluded ME+1/3


# ______________________________________________________________________________
# Helper

def getConvGlobalPhi(isector, iphi):
    if iphi == -999:  return -999.
    fphi = ((iphi)*0.0166666) + (isector%6)*60.0 + 13.0
    fphi *= pi / 180.0
    if fphi > pi:  fphi -= 2*pi  # convert from [0,2pi] to [-pi,pi]
    return fphi

def getConvGlobalTheta(isector, itheta):
    if itheta == -999:  return -999.
    ftheta = (itheta*0.2851562) + 8.5
    ftheta *= pi / 180.0
    if isector/6 == 1:  ftheta = -ftheta
    return ftheta

def getConvGlobalEta(isector, itheta):
    if itheta == -999:  return -999.
    ftheta = getConvGlobalTheta(isector, itheta)
    feta = - log(tan(ftheta/2.0))
    return feta

def make_tgraph(hname, htitle, nbinsx=100, xmin=-1, xmax=1):
    tgraph = TGraph(nbinsx)
    tgraph.h = TH1F(hname+"_frame", htitle, nbinsx, xmin, xmax)
    tgraph.SetName(hname)
    tgraph.xx = []
    tgraph.yy = []
    return tgraph

def fill_tgraph(tgraph, x, y):
    tgraph.xx.append(x)
    tgraph.yy.append(y)
    return

def freeze_tgraph(tgraph):
    n = len(tgraph.xx)
    tgraph.Set(n)
    for i in xrange(n):
        tgraph.SetPoint(i, tgraph.xx[i], tgraph.yy[i])
    return

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

    def label_st0(st):
        d = {11: 'ME+1/1b', 12: 'ME+1/2', 20: 'ME+2', 30: 'ME+3', 40: 'ME+4'}
        ret = d[st]
        if options.east:
            ret = d[st].replace("ME+", "ME-")
        return ret

    # TH1F
    for st in st_vec:
        nbinsx, xmin, xmax = 50, -0.012, -0.004
        hname = "emtf_error_convGlobalPhi_st%i" % (st)
        histos[hname] = TH1F(hname, "; #phi(%s) - emu #phi(%s) [rad]" % (label_st(st), label_st(st)), nbinsx, xmin, xmax)

        nbinsx, xmin, xmax = 50, -0.01, 0.01
        if st in [11, 14]:  # ME1/1a, ME1/1b
            nbinsx, xmin, xmax = 50, -0.02, 0.02
        hname = "emtf_error_convGlobalTheta_st%i" % (st)
        histos[hname] = TH1F(hname, "; #theta(%s) - emu #theta(%s) [rad]" % (label_st(st), label_st(st)), nbinsx, xmin, xmax)

    hname = "genParts_pt"
    histos[hname] = TH1F(hname, "; gen p_{T} [GeV]", 100, 0, 2000)
    hname = "genParts_invPt"
    histos[hname] = TH1F(hname, "; gen q/p_{T} [1/GeV]", 100, -0.5, 0.5)
    hname = "genParts_eta"
    histos[hname] = TH1F(hname, "; gen #eta", 60, -3, 3)
    hname = "genParts_phi"
    histos[hname] = TH1F(hname, "; gen #phi [rad]", 64, -3.2, 3.2)

    # TGraph
    for st0 in st0_vec:
        hname = "emtf_error_scatter_globalPhi_st%i" % (st0)
        histos[hname] = make_tgraph(hname, "; emu integer #phi(%s); #phi(%s) [rad]" % (label_st(st0), label_st(st0)))
        hname = "emtf_error_scatter_globalTheta_st%i" % (st0)
        histos[hname] = make_tgraph(hname, "; emu integer #theta(%s); #theta(%s) [rad]" % (label_st(st0), label_st(st0)))

        hname = "emtf_error_scatter_convGlobalPhi_st%i" % (st0)
        histos[hname] = make_tgraph(hname, "; emu integer #phi(%s); emu #phi(%s) [rad]" % (label_st(st0), label_st(st0)))
        hname = "emtf_error_scatter_convGlobalTheta_st%i" % (st0)
        histos[hname] = make_tgraph(hname, "; emu integer #theta(%s); emu #theta(%s) [rad]" % (label_st(st0), label_st(st0)))

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
        elif h.ClassName() == "TGraph":
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

        is_endcap = 1.24 < part_eta < 2.4

        if part_pt > 0 and is_endcap:

            hname = "genParts_pt"
            histos[hname].Fill(part_pt)
            hname = "genParts_invPt"
            histos[hname].Fill(part_charge/part_pt)
            hname = "genParts_eta"
            histos[hname].Fill(part_eta)
            hname = "genParts_phi"
            histos[hname].Fill(part_phi)

            for t in izip(evt.CSCStubs_isector, evt.CSCStubs_istation, evt.CSCStubs_iring, evt.CSCStubs_globalPhi, evt.CSCStubs_globalTheta, evt.CSCStubs_globalEta, evt.CSCStubs_convGlobalPhi, evt.CSCStubs_convGlobalTheta, evt.CSCStubs_convGlobalEta, evt.CSCStubs_convPhi, evt.CSCStubs_convTheta):

                (isector, istation, iring, globalPhi, globalTheta, globalEta, convGlobalPhi, convGlobalTheta, convGlobalEta, convPhi, convTheta) = t

                if istation == 1 and iring == 3:  # skip ME+1/3
                    continue

                st = istation * 10 + iring

                st0 = istation * 10
                if istation == 1:
                    if iring == 1 or iring == 4:
                        st0 = 11
                    if iring == 2:
                        st0 = 12

                #dphi = deltaPhi(globalPhi, convGlobalPhi)
                #dtheta = deltaPhi(globalTheta, convGlobalTheta)
                #deta = deltaEta(globalEta, convGlobalEta)

                dphi = deltaPhi(globalPhi, getConvGlobalPhi(isector, convPhi))
                dtheta = deltaPhi(globalTheta, getConvGlobalTheta(isector, convTheta))

                if st in st_vec:
                    hname = "emtf_error_convGlobalPhi_st%i" % (st)
                    histos[hname].Fill(dphi)
                    hname = "emtf_error_convGlobalTheta_st%i" % (st)
                    histos[hname].Fill(dtheta)

                #if abs(getConvGlobalPhi(isector, convPhi) - convGlobalPhi) > 1e-5:
                #    raise Exception("Inconsistent in converted hit global phi coordinate: %.6f vs %.6f" % (getConvGlobalPhi(isector, convPhi), convGlobalPhi))

                if convPhi != -999 and convTheta != -999:
                    hname = "emtf_error_scatter_globalPhi_st%i" % (st0)
                    fill_tgraph(histos[hname], convPhi, globalPhi)
                    hname = "emtf_error_scatter_globalTheta_st%i" % (st0)
                    fill_tgraph(histos[hname], convTheta, globalTheta)

                    hname = "emtf_error_scatter_convGlobalPhi_st%i" % (st0)
                    fill_tgraph(histos[hname], convPhi, getConvGlobalPhi(isector, convPhi))
                    hname = "emtf_error_scatter_convGlobalTheta_st%i" % (st0)
                    fill_tgraph(histos[hname], convTheta, getConvGlobalTheta(isector, convTheta))
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

        elif h.ClassName() == "TGraph":
            if True:
                freeze_tgraph(h)
                h.SetMarkerStyle(6); h.SetMarkerColor(2)

                #if hname != "emtf_error_scatter_convGlobalPhi_st20":  # FIXME
                #    continue

                h.Draw("ap")
                h.GetXaxis().SetTitle(h.h.GetXaxis().GetTitle())
                h.GetYaxis().SetTitle(h.h.GetYaxis().GetTitle())
                gPad.SetLogy(h.logy)
                CMS_label()
                save(options.outdir, hname, dot_pdf=False)
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

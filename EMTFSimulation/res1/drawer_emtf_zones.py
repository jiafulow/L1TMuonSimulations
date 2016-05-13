#!/usr/bin/env python

from rootdrawing import *
from math import sinh, atan2
import random


# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0

#pt_vec = [3,5,10,20,50]
#st_vec = [11,12,14,21,22,31,32,41,42]  # excluded ME+1/3
std_vec = [2010,2011,2012,2030,2040,3010,3011,3012,3040,4010,4011,4012]


# ______________________________________________________________________________
# Drawer
def drawer_book(options):
    histos = {}

    # TH2F
    nbinsx, xmin, xmax = 100, -50, 50
    nbinsy, ymin, ymax = 6, -0.5, 5.5
    hname = "emtf_dquad_ME11_by_stations"
    histos[hname] = TH2F(hname, "; #Delta quadstrip; station", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME12_by_stations"
    histos[hname] = TH2F(hname, "; #Delta quadstrip; station", nbinsx, xmin, xmax, nbinsy, ymin, ymax)

    nbinsy, ymin, ymax = 100, 0.0, 0.5
    hname = "emtf_dquad_ME11_for_invPt_muplus"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{12}; 1/p_{T} [1/GeV]", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME12_for_invPt_muplus"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{12}; 1/p_{T} [1/GeV]", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME11_for_invPt_muminus"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{12}; 1/p_{T} [1/GeV]", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME12_for_invPt_muminus"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{12}; 1/p_{T} [1/GeV]", nbinsx, xmin, xmax, nbinsy, ymin, ymax)

    nbinsy, ymin, ymax = 100, 0.15, 0.65
    hname = "emtf_dquad_ME11_for_theta"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{12}; #theta [rad]", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME11_for_theta_st30"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{23}; #theta [rad]", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME11_for_theta_st40"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{24}; #theta [rad]", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME12_for_theta"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{12}; #theta [rad]", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME12_for_theta_st30"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{23}; #theta [rad]", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME12_for_theta_st40"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{24}; #theta [rad]", nbinsx, xmin, xmax, nbinsy, ymin, ymax)

    nbinsy, ymin, ymax = 100, 1.0, 2.5
    hname = "emtf_dquad_ME11_for_eta"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{12}; #eta", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME11_for_eta_st30"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{23}; #eta", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME11_for_eta_st40"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{24}; #eta", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME12_for_eta"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{12}; #eta", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME12_for_eta_st30"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{23}; #eta", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME12_for_eta_st40"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{24}; #eta", nbinsx, xmin, xmax, nbinsy, ymin, ymax)

    nbinsy, ymin, ymax = 100, -50, 50
    hname = "emtf_dquad_ME11_for_dquad_st30"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{12}; #Delta quadstrip_{23}", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME12_for_dquad_st30"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{12}; #Delta quadstrip_{23}", nbinsx, xmin, xmax, nbinsy, ymin, ymax)

    nbinsy, ymin, ymax = 100, -50, 50
    hname = "emtf_dquad_ME11_for_dquad_st40"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{12}; #Delta quadstrip_{24}", nbinsx, xmin, xmax, nbinsy, ymin, ymax)
    hname = "emtf_dquad_ME12_for_dquad_st40"
    histos[hname] = TH2F(hname, "; #Delta quadstrip_{12}; #Delta quadstrip_{24}", nbinsx, xmin, xmax, nbinsy, ymin, ymax)


    # Style
    for hname, h in histos.iteritems():
        if h.ClassName() == "TH1F":
            h.style = 0; h.logx = options.logx; h.logy = options.logy
        elif h.ClassName() == "TH2F":
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

        if is_endcap:

            key_quads = []

            for (istation, iring, convZhit) in izip(evt.CSCStubs_istation, evt.CSCStubs_iring, evt.CSCStubs_convZhit):
                st = istation * 10 + iring
                quad = convZhit

                if istation == 2:  # pick station 2
                    if 0+10 <= quad <= 120-10:  # ignore 10 strips near the edge
                        key_quads.append(quad)

            if not key_quads:  # skip if no LCTs in station 2
                continue

            # Pick best key quadstrip
            best_key_quad = random.choice(key_quads)

            dquads_st11 = []
            dquads_st12 = []
            dquads_st30 = []
            dquads_st40 = []

            for (istation, iring, convZhit) in izip(evt.CSCStubs_istation, evt.CSCStubs_iring, evt.CSCStubs_convZhit):
                st = istation * 10 + iring
                quad = convZhit

                if istation == 1 and iring == 3:  # skip ME+1/3
                    continue
                if istation == 2:  # no longer pick station 2
                    continue

                dquad = best_key_quad - quad

                #if abs(dquad) > 50:
                #    print "[ERROR]", ievt, dquad, best_key_quad, quad, st
                if abs(dquad) > 50:  # too far
                    continue

                if istation == 1:  # flip dquad for station 1
                    dquad = -dquad

                if part_charge == 1:  # flip dquad for antimuons
                    dquad = -dquad

                if st == 11 or st == 14:
                    dquads_st11.append(dquad)
                elif st == 12:
                    dquads_st12.append(dquad)
                elif st == 31 or st == 32:
                    dquads_st30.append(dquad)
                elif st == 41 or st == 42:
                    dquads_st40.append(dquad)
                else:
                    raise Exception("Unexpected st: %i" % st)

            if dquads_st11:  # if ME1/1 exists, discard ME1/2
                dquads_st12 = []

            # Pick best delta quadstrips
            best_dquad_st11 = random.choice(dquads_st11) if dquads_st11 else None
            best_dquad_st12 = random.choice(dquads_st12) if dquads_st12 else None
            best_dquad_st30 = random.choice(dquads_st30) if dquads_st30 else None
            best_dquad_st40 = random.choice(dquads_st40) if dquads_st40 else None

            if dquads_st11:
                x = best_dquad_st11
                hname = "emtf_dquad_ME11_by_stations"
                # Station 1
                histos[hname].Fill(x, 1)
                # Station 2
                histos[hname].Fill(0, 2)
                # Other stations
                if dquads_st30:  histos[hname].Fill(best_dquad_st30, 3)
                if dquads_st40:  histos[hname].Fill(best_dquad_st40, 4)

                if part_charge == 1:
                    hname = "emtf_dquad_ME11_for_invPt_muplus"
                else:
                    hname = "emtf_dquad_ME11_for_invPt_muminus"
                histos[hname].Fill(x, 1.0/part_pt)

                hname = "emtf_dquad_ME11_for_theta"
                histos[hname].Fill(x, part_theta)
                hname = "emtf_dquad_ME11_for_theta_st30"
                if dquads_st30:  histos[hname].Fill(best_dquad_st30, part_theta)
                hname = "emtf_dquad_ME11_for_theta_st40"
                if dquads_st40:  histos[hname].Fill(best_dquad_st40, part_theta)

                hname = "emtf_dquad_ME11_for_eta"
                histos[hname].Fill(x, part_eta)
                hname = "emtf_dquad_ME11_for_eta_st30"
                if dquads_st30:  histos[hname].Fill(best_dquad_st30, part_eta)
                hname = "emtf_dquad_ME11_for_eta_st40"
                if dquads_st40:  histos[hname].Fill(best_dquad_st40, part_eta)

                hname = "emtf_dquad_ME11_for_dquad_st30"
                if dquads_st30:  histos[hname].Fill(x, best_dquad_st30)

                hname = "emtf_dquad_ME11_for_dquad_st40"
                if dquads_st40:  histos[hname].Fill(x, best_dquad_st40)

            if dquads_st12:
                x = best_dquad_st12
                hname = "emtf_dquad_ME12_by_stations"
                # Station 1
                histos[hname].Fill(x, 1)
                # Station 2
                histos[hname].Fill(0, 2)
                # Other stations
                if dquads_st30:  histos[hname].Fill(best_dquad_st30, 3)
                if dquads_st40:  histos[hname].Fill(best_dquad_st40, 4)

                if part_charge == 1:
                    hname = "emtf_dquad_ME12_for_invPt_muplus"
                else:
                    hname = "emtf_dquad_ME12_for_invPt_muminus"
                histos[hname].Fill(x, 1.0/part_pt)

                hname = "emtf_dquad_ME12_for_theta"
                histos[hname].Fill(x, part_theta)
                hname = "emtf_dquad_ME12_for_theta_st30"
                if dquads_st30:  histos[hname].Fill(best_dquad_st30, part_theta)
                hname = "emtf_dquad_ME12_for_theta_st40"
                if dquads_st40:  histos[hname].Fill(best_dquad_st40, part_theta)

                hname = "emtf_dquad_ME12_for_eta"
                histos[hname].Fill(x, part_eta)
                hname = "emtf_dquad_ME12_for_eta_st30"
                if dquads_st30:  histos[hname].Fill(best_dquad_st30, part_eta)
                hname = "emtf_dquad_ME12_for_eta_st40"
                if dquads_st40:  histos[hname].Fill(best_dquad_st40, part_eta)

                hname = "emtf_dquad_ME12_for_dquad_st30"
                if dquads_st30:  histos[hname].Fill(x, best_dquad_st30)

                hname = "emtf_dquad_ME12_for_dquad_st40"
                if dquads_st40:  histos[hname].Fill(x, best_dquad_st40)

            #if (ievt % 1000 == 0):  print "evt %i: len(key_quads): %i # len(dquads_st11): %i len(dquads_st12): %i len(dquads_st30): %i len(dquads_st40): %i" % (ievt, len(key_quads), len(dquads_st11), len(dquads_st12), len(dquads_st30), len(dquads_st40))

    return

# ______________________________________________________________________________
def drawer_draw(histos, options):
    def display_quantiles(h, in_quantiles=[0.95,0.98,0.99], scalebox=(1.,1.)):
        # Display one-sided confidence intervals, a.k.a quantiles
        n = len(in_quantiles)
        in_quantiles = array('d', in_quantiles)
        quantiles = array('d', [0. for i in xrange(n)])
        h.GetQuantiles(n, quantiles, in_quantiles)

        gPad.Modified(); gPad.Update()
        ps = h.FindObject("stats"); ps.SetName("mystats")
        newX1NDC = ps.GetX2NDC() - (ps.GetX2NDC() - ps.GetX1NDC()) * scalebox[0]
        newY1NDC = ps.GetY2NDC() - ((ps.GetY2NDC() - ps.GetY1NDC()) / 5 * (5 + n)) * scalebox[1]
        ps.SetX1NDC(newX1NDC); ps.SetY1NDC(newY1NDC)

        for iq, q in enumerate(in_quantiles):
            ps.AddText("%i%% CI = %6.4g" % (int(q*100), quantiles[iq]))
        h.stats = [h.GetMean()] + quantiles.tolist()
        h.SetStats(0); ps.Draw()
        return

    for hname, h in histos.iteritems():

        if h.ClassName() == "TH1F":
            if False:
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
            if True:
                h.SetStats(0); h.Draw("COLZ")

                if hname in ["emtf_dquad_ME11_by_stations", "emtf_dquad_ME12_by_stations"]:
                    xmin, xmax = h.GetXaxis().GetXmin(), h.GetXaxis().GetXmax()
                    for y in [0.5, 1.5, 2.5, 3.5, 4.5]:
                        tline.DrawLine(xmin, y, xmax, y)

                CMS_label()
                save(options.outdir, hname)

            if True:
                # Make 1D projections
                if hname in ["emtf_dquad_ME11_by_stations", "emtf_dquad_ME12_by_stations"]:
                    xlows   = [1, 2, 3, 4]
                    xhighs  = [1, 2, 3, 4]

                    for i, (xlow, xhigh) in enumerate(zip(xlows, xhighs)):
                        h1 = h.ProjectionX(hname + "_px%i" % i, h.GetYaxis().FindFixBin(xlow), h.GetYaxis().FindFixBin(xhigh))
                        if xlow <= 2:
                            h1.GetXaxis().SetTitle("#Delta quadstrip_{%i2}" % (i+1))
                        else:
                            h1.GetXaxis().SetTitle("#Delta quadstrip_{2%i}" % (i+1))
                        h1.SetMaximum(h1.GetMaximum() * 1.4); h1.SetMinimum(0.)
                        h1.SetLineWidth(2); h1.SetMarkerSize(0)
                        h1.SetLineColor(col); h1.SetFillColor(fcol)
                        h1.Draw("hist")
                        CMS_label()
                        save(options.outdir, h1.GetName())

                if hname in ["emtf_dquad_ME11_for_dquad_st30", "emtf_dquad_ME12_for_dquad_st30", "emtf_dquad_ME11_for_dquad_st40", "emtf_dquad_ME12_for_dquad_st40"]:

                    xlows   = [-49, -15, -7, -3, -1, 0, 1, 2,  8]
                    xhighs  = [-16,  -8, -4, -2, -1, 0, 1, 7, 49]

                    for i, (xlow, xhigh) in enumerate(zip(xlows, xhighs)):
                        h1 = h.ProjectionY(hname + "_py%i" % i, h.GetXaxis().FindFixBin(xlow), h.GetXaxis().FindFixBin(xhigh))
                        #h1.GetXaxis().SetTitle(h.GetYaxis().GetTitle() + ", #Delta quadstrip_{12}=[%i,%i]" % (xlow, xhigh))
                        h1.GetXaxis().SetTitle(h.GetYaxis().GetTitle())
                        h1.SetMaximum(h1.GetMaximum() * 1.4); h1.SetMinimum(0.)
                        h1.SetLineWidth(2); h1.SetMarkerSize(0)
                        h1.SetLineColor(col); h1.SetFillColor(fcol)
                        h1.Draw("hist")
                        display_quantiles(h1)
                        CMS_label()
                        save(options.outdir, h1.GetName())

                # Specialized
                if hname in ["emtf_dquad_ME11_for_invPt_muminus", "emtf_dquad_ME12_for_invPt_muminus"]:
                    continue
                if hname in ["emtf_dquad_ME11_for_invPt_muplus", "emtf_dquad_ME12_for_invPt_muplus"]:
                    hname_muminus = hname.replace("_muplus", "_muminus")
                    h_muminus = histos[hname_muminus]

                    xlows   = [-49, -15, -7, -3, -1, 0, 1, 2,  8]
                    xhighs  = [-16,  -8, -4, -2, -1, 0, 1, 7, 49]

                    for i, (xlow, xhigh) in enumerate(zip(xlows, xhighs)):
                        h1 = h.ProjectionY(hname + "_py%i" % i, h.GetXaxis().FindFixBin(xlow), h.GetXaxis().FindFixBin(xhigh))
                        #h1.GetXaxis().SetTitle(h.GetYaxis().GetTitle() + ", #Delta quadstrip_{12}=[%i,%i]" % (xlow, xhigh))
                        h1.GetXaxis().SetTitle(h.GetYaxis().GetTitle())
                        h1.SetMaximum(h1.GetMaximum() * 1.4); h1.SetMinimum(0.)
                        h1.SetLineWidth(2); h1.SetMarkerSize(0)
                        #h1.SetLineColor(col); h1.SetFillColor(fcol)
                        h1.SetLineColor(palette[1])  # muplus

                        # Repeat for h_muminus
                        h1_muminus = h_muminus.ProjectionY(hname_muminus + "_py%i" % i, h_muminus.GetXaxis().FindFixBin(xlow), h_muminus.GetXaxis().FindFixBin(xhigh))
                        #h1_muminus.GetXaxis().SetTitle(h_muminus.GetYaxis().GetTitle() + ", #Delta quadstrip_{12}=[%i,%i]" % (xlow, xhigh))
                        h1_muminus.GetXaxis().SetTitle(h_muminus.GetYaxis().GetTitle())
                        h1_muminus.SetMaximum(h1_muminus.GetMaximum() * 1.4); h1_muminus.SetMinimum(0.)
                        h1_muminus.SetLineWidth(2); h1_muminus.SetMarkerSize(0)
                        #h1_muminus.SetLineColor(col); h1_muminus.SetFillColor(fcol)
                        h1_muminus.SetLineColor(palette[0])  # muminus

                        h1.Draw("hist"); h1_muminus.Draw("hist same")
                        #display_quantiles(h1)
                        CMS_label()
                        save(options.outdir, h1.GetName())
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
    gStyle.SetNdivisions(510, "XY")
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

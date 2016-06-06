#!/usr/bin/env python

from rootdrawing import *
from math import sinh, atan2


# Configurations
col  = TColor.GetColor("#e31a1c")  # nu140
fcol = TColor.GetColor("#fb9a99")  # nu140

mode_vec = [11, 13, 14, 15] # excluded 7
station_vec = [0, 1, 2, 3, 4]

l1t_pt_vec = [10, 16, 20]  # trigger pt


# ______________________________________________________________________________
# Drawer
def drawer_book(options):
    histos = {}

    # TH1F
    hname = "nvertices"
    histos[hname] = TH1F(hname, "; # vertices", 60, 0, 60)

    nbinsx, xmin, xmax = 15, 0, 15
    for station in station_vec:
        hname = "emtf_nstubs_e%i" % station
        histos[hname] = TH1F(hname, "; EMTF # of LCTs per sector; Entries/(3BX)", nbinsx, xmin, xmax)
        hname = "emtf_nsuperstrips_e%i" % station
        histos[hname] = TH1F(hname, "; EMTF # of superstrips per sector; Entries/(3BX)", nbinsx, xmin, xmax)
    hname = "emtf_nroads_winner"
    histos[hname] = TH1F(hname, "; EMTF # of roads per sector; Entries/(3BX)", nbinsx, xmin, xmax)
    hname = "emtf_nroads_besttrack"
    histos[hname] = TH1F(hname, "; EMTF # of 'best' roads per sector; Entries/(3BX)", nbinsx, xmin, xmax)
    hname = "emtf_ntracks"
    histos[hname] = TH1F(hname, "; EMTF # of tracks per sector; Entries/(3BX)", nbinsx, xmin, xmax)

    nbinsx, xmin, xmax = 20, -0.5, 19.5
    hname = "emtf_mode"
    histos[hname] = TH1F(hname, "; EMTF mode; Entries/(3BX)", nbinsx, xmin, xmax)

    nbinsx, xmin, xmax = 100, 0, 150
    hname = "emtf_pt"
    histos[hname] = TH1F(hname, "; EMTF p_{T} [GeV]; Entries/(3BX)", nbinsx, xmin, xmax)

    nbinsx, xmin, xmax = 100, 0, 150
    hname = "emtf_ptcut"
    histos[hname] = TH1F(hname, "; EMTF cutoff p_{T} [GeV]; Entries /w p_{T}>cutoff p_{T}/(3BX)", nbinsx, xmin, xmax)

    nbinsx, xmin, xmax = 64, -3.2, 3.2
    hname = "emtf_globalPhi"
    histos[hname] = TH1F(hname, "; EMTF #phi [rad]; Entries/(3BX)", nbinsx, xmin, xmax)

    nbinsx, xmin, xmax = 60, 1.2, 2.4
    hname = "emtf_globalEta"
    histos[hname] = TH1F(hname, "; EMTF #eta; Entries/(3BX)", nbinsx, xmin, xmax)

    # Style
    for hname, h in histos.iteritems():
        if h.ClassName() == "TH1F":
            h.style = 0; h.logx = options.logx; h.logy = options.logy
            if any([hname.startswith(x) for x in ["emtf_pt", "emtf_ptcut", "emtf_nstubs_", "emtf_nsuperstrips_", "emtf_nroads_", "emtf_ntracks"]]):
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
    tree.SetBranchStatus("gen_nPV", 1)
    tree.SetBranchStatus("CSCTracks_itracks*", 1)
    tree.SetBranchStatus("CSCTracks_convHits_station", 1)
    tree.SetBranchStatus("CSCTracks_convHits_isector", 1)
    tree.SetBranchStatus("CSCTracks_zones_zhitStations", 1)
    tree.SetBranchStatus("CSCTracks_zones_isector", 1)
    tree.SetBranchStatus("CSCTracks_roads_iwinner", 1)
    tree.SetBranchStatus("CSCTracks_roads_ibesttrack", 1)
    tree.SetBranchStatus("CSCTracks_roads_isector", 1)

    def modify_ptcut(h):
        nbins = h.GetNbinsX()
        entries = 0
        for i in xrange(nbins, 0, -1):
            entries += h.GetBinContent(i)
            h.SetBinContent(i, entries)
        h.SetEntries(h.GetEntries() - nbins)
        return

    # Loop over events
    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break
        if (ievt % 1000 == 0):  print "Processing event: %i" % ievt

        itracks_mode = []
        itracks_pt = []
        itracks_globalPhi = []
        itracks_globalEta = []

        for (mode, pt, globalPhi, globalEta) in izip(evt.CSCTracks_itracks_mode, evt.CSCTracks_itracks_pt, evt.CSCTracks_itracks_globalPhi, evt.CSCTracks_itracks_globalEta):

            is_endcap = 1.24 < globalEta < 2.4
            is_ok = pt > 2

            if is_endcap and is_ok:
                itracks_mode.append(mode)
                itracks_pt.append(pt)
                itracks_globalPhi.append(globalPhi)
                itracks_globalEta.append(globalEta)

        for (mode, pt, globalPhi, globalEta) in izip(itracks_mode, itracks_pt, itracks_globalPhi, itracks_globalEta):
            hname = "emtf_mode"
            histos[hname].Fill(mode)

            if mode in mode_vec:
                hname = "emtf_pt"
                histos[hname].Fill(pt)
                hname = "emtf_ptcut"
                histos[hname].Fill(pt)
                hname = "emtf_globalPhi"
                histos[hname].Fill(globalPhi)
                hname = "emtf_globalEta"
                histos[hname].Fill(globalEta)

        # Perform counting
        nstubs = [0] * len(station_vec)
        for (station, isector) in izip(evt.CSCTracks_convHits_station, evt.CSCTracks_convHits_isector):
            if isector == 0:
                assert(1 <= station <= 4)
                nstubs[0] += 1
                nstubs[station] += 1

        nsuperstrips = [0] * len(station_vec)
        for (zhitStations, isector) in izip(evt.CSCTracks_zones_zhitStations, evt.CSCTracks_zones_isector):
            if isector == 0:
                for station in zhitStations:
                    station += 1
                    assert(1 <= station <= 4)
                    nsuperstrips[0] += 1
                    nsuperstrips[station] += 1

        nroads_winner = 0
        nroads_besttrack = 0
        for (iwinner, ibesttrack, isector) in izip(evt.CSCTracks_roads_iwinner, evt.CSCTracks_roads_ibesttrack, evt.CSCTracks_roads_isector):
            if isector == 0:
                if iwinner >= 0:
                    nroads_winner += 1
                if ibesttrack >= 0:
                    nroads_besttrack += 1

        ntracks = 0
        for (mode, isector) in izip(evt.CSCTracks_itracks_mode, evt.CSCTracks_itracks_isector):
            if isector == 0:
                if mode in mode_vec:
                    ntracks += 1

        # Fill
        for station in station_vec:
            hname = "emtf_nstubs_e%i" % station
            histos[hname].Fill(nstubs[station])
            hname = "emtf_nsuperstrips_e%i" % station
            histos[hname].Fill(nsuperstrips[station])
        hname = "emtf_nroads_winner"
        histos[hname].Fill(nroads_winner)
        hname = "emtf_nroads_besttrack"
        histos[hname].Fill(nroads_besttrack)
        hname = "emtf_ntracks"
        histos[hname].Fill(ntracks)

        histos["nvertices"].Fill(evt.gen_nPV)

    # Modify emtf_ptcut
    hname = "emtf_ptcut"
    modify_ptcut(histos[hname])
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

                # Customize
                if hname == "emtf_mode":
                    tlatex.SetNDC(0)
                    for x in mode_vec:
                        tlatex.DrawLatex(x-0.4, h.GetMaximum()/50., str(x))
                    tlatex.SetNDC(1)
                elif hname == "emtf_ptcut":
                    for ix, x in enumerate(l1t_pt_vec):
                        rate = h.GetBinContent(h.FindBin(x))
                        tlatex.DrawLatex(0.67, 0.70-ix*0.04, "Entries /w p_{T}>%3i: %.0f" % (x, rate))

                gPad.SetLogy(h.logy)
                CMS_label()
                save(options.outdir, hname, dot_root=True)
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

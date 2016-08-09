#!/usr/bin/env python

from rootdrawing import *
from math import sinh, atan2


# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0
mcol = TColor.GetColor("#600000")

palette = map(lambda x: TColor.GetColor(x), ("#66DD66", "#3333FF", "#990099", "#FFBB44", "#EE4477", "#56D3DB", "#454545"))

l1t_pt_vec = (3,5,10,20,50,200,1000)
l1t_eta_vec = (0, 1, 2, 3)  # 3 eta bins + inclusive
l1t_eta_floats = ((1.2,1.6), (1.6,2.0), (2.0,2.4), (-99999999.,99999999.))
l1t_mode_vec = (7,11,13,14,15)  # 0b0111, 0b1011, 0b1101, 0b1110, 0b1111
l1t_mode_vec += (27,29,30,31)  # for ME1/2
l1t_mode_vec += (32,)  # inclusive
l1t_quality_vec = (4,8,12,16)


# ______________________________________________________________________________
# Helper
def find_l1t_pt_bin(pt):
    for b_pt in l1t_pt_vec:
        if (0.98 < pt/float(b_pt) < 1.02):
            return b_pt
    return -1

def find_l1t_st_bin(istation, iring, strip):
    if istation == 1 and iring == 1 and strip >= 128:  # fix ME1/A
        iring = 4
    b = 2 * istation + iring  # b=2S+R ok for S=2,3,4
    if istation == 1:
        b -= 1                # b=1+R (2S+R-1=1+R for S=1)
        if b > 4:             # b=1 if R=4
            b = 1
    if 1 <= b <= 10:
        return b
    return -1

def find_l1t_mode_bin(mode, st):
    if (mode & 8) and 3 <= st <= 4:  # for ME1/2
        mode += 16
    if mode in l1t_mode_vec:
        return mode
    return -1

def get_l1t_mode_label(b):
    if b == 7:
        label = "%i" % b
    elif b in (11,13,14,15):
        label = "%i,ME11" % b
    elif b in (27,29,30,31):  # for ME1/2
        label = "%i,ME12" % (b-16)
    elif b in (32,):
        label = "all"
    return label

def find_l1t_eta_bin(eta):
    for b_eta in l1t_eta_vec:
        if (l1t_eta_floats[b_eta][0] <= abs(eta) < l1t_eta_floats[b_eta][1]):
            return b_eta
    return -1

def get_l1t_eta_label(b):
    if b == l1t_eta_vec[-1]:
        label = ""
    else:
        label = "%.1f #leq |#eta| < %.1f" % (l1t_eta_floats[b][0], l1t_eta_floats[b][1])
    return label

def mode_to_quality(mode):
    # modes [15]       --> quality = 16
    # modes [11,13,14] --> quality = 12
    # modes [7,10,12]  --> quality = 8
    # modes [3,5,6,9]  --> quality = 4
    qualities = [
        0, 0, 0, 4,
        0, 4, 4, 8,
        0, 4, 8,12,
        8,12,12,16,
    ]
    return qualities[mode]

def get_l1t_quality_label(b):
    label = ""
    if b == 4:
        label = "[3,5,6,9]"
    elif b == 8:
        label = "[7,10,12]"
    elif b == 12:
        label = "[11,13,14]"
    elif b == 16:
        label = "[15]"
    return label

# ______________________________________________________________________________
# Drawer
def drawer_book(histos, options):
    myptbins = [0.0, 0.5, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 12.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60.0, 70.0, 80.0, 100.0, 125.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 800.0, 1000.0, 2000.0, 7000.0]

    # TH1F
    for ptbin in l1t_pt_vec:
        for modebin in l1t_mode_vec:
            hname = "resolution_pt_in_mode%i_pt%i" % (modebin, ptbin)
            htitle, nbinsx, xmin, xmax = "; #Delta (1/p_{T})*p_{T} {mode %s}; entries" % (get_l1t_mode_label(modebin)), 200, -8, 8
            histos[hname] = TH1F(hname, htitle, nbinsx, xmin, xmax)
            histos[hname].indices = (modebin, ptbin)
        for qualbin in l1t_quality_vec:
            hname = "resolution_pt_in_qual%i_pt%i" % (qualbin, ptbin)
            htitle, nbinsx, xmin, xmax = "; #Delta (1/p_{T})*p_{T} {modes %s}; entries" % (get_l1t_quality_label(qualbin)), 200, -8, 8
            histos[hname] = TH1F(hname, htitle, nbinsx, xmin, xmax)
            histos[hname].indices = (qualbin, ptbin)

    for etabin in l1t_eta_vec:
        for modebin in l1t_mode_vec:
            hname = "resolution_pt_in_mode%i_eta%i" % (modebin, etabin)
            htitle, nbinsx, xmin, xmax = "; #Delta (1/p_{T})*p_{T} {mode %s}; entries" % (get_l1t_mode_label(modebin)), 200, -8, 8
            histos[hname] = TH1F(hname, htitle, nbinsx, xmin, xmax)
            histos[hname].indices = (modebin, etabin)
        for qualbin in l1t_quality_vec:
            hname = "resolution_pt_in_qual%i_eta%i" % (qualbin, etabin)
            htitle, nbinsx, xmin, xmax = "; #Delta (1/p_{T})*p_{T} {modes %s}; entries" % (get_l1t_quality_label(qualbin)), 200, -8, 8
            histos[hname] = TH1F(hname, htitle, nbinsx, xmin, xmax)
            histos[hname].indices = (qualbin, ptbin)

    # TH2F
    for ptbin in l1t_pt_vec:
        for modebin in l1t_mode_vec:
            hname = "resolution_pt_vs_eta_in_mode%i_pt%i" % (modebin, ptbin)
            htitle, nbinsx, xmin, xmax, nbinsy, ymin, ymax = "; #eta; #Delta (1/p_{T})*p_{T} {mode %s}" % (get_l1t_mode_label(modebin)), 200, -8, 8, 26, 1.2, 2.5
            histos[hname] = TH2F(hname, htitle, nbinsy, ymin, ymax, nbinsx, xmin, xmax)
            histos[hname].indices = (modebin, ptbin)
        for qualbin in l1t_quality_vec:
            hname = "resolution_pt_vs_eta_in_qual%i_pt%i" % (qualbin, ptbin)
            htitle, nbinsx, xmin, xmax, nbinsy, ymin, ymax = "; #eta; #Delta (1/p_{T})*p_{T} {modes %s}" % (get_l1t_quality_label(qualbin)), 200, -8, 8, 26, 1.2, 2.5
            histos[hname] = TH2F(hname, htitle, nbinsy, ymin, ymax, nbinsx, xmin, xmax)
            histos[hname].indices = (qualbin, ptbin)

    for etabin in l1t_eta_vec:
        for modebin in l1t_mode_vec:
            hname = "resolution_pt_vs_pt_in_mode%i_eta%i" % (modebin, etabin)
            htitle, nbinsx, xmin, xmax = "; p_{T} [GeV]; #Delta (1/p_{T})*p_{T} {mode %s}" % (get_l1t_mode_label(modebin)), 200, -8, 8
            histos[hname] = TH2F(hname, htitle, len(myptbins)-1, np.array(myptbins), nbinsx, xmin, xmax)
            histos[hname].indices = (modebin, etabin)
        for qualbin in l1t_quality_vec:
            hname = "resolution_pt_vs_pt_in_qual%i_eta%i" % (qualbin, etabin)
            htitle, nbinsx, xmin, xmax = "; p_{T} [GeV]; #Delta (1/p_{T})*p_{T} {modes %s}" % (get_l1t_quality_label(qualbin)), 200, -8, 8
            histos[hname] = TH2F(hname, htitle, len(myptbins)-1, np.array(myptbins), nbinsx, xmin, xmax)
            histos[hname].indices = (qualbin, etabin)

    # Style
    for hname, h in histos.iteritems():
        if h.ClassName() == "TH1F":
            h.style = 0; h.logx = options.logx; h.logy = options.logy
        elif h.ClassName() == "TH2F":
            h.style = 0; h.logx = False; h.logy = False

        if h.ClassName() == "TH2F":
            if hname.startswith("resolution_pt_vs_pt_"):
                h.logx = True
    return

# ______________________________________________________________________________
def drawer_project(tree, histos, options):
    tree.SetBranchStatus("*", 0)
    tree.SetBranchStatus("genParts_*", 1)
    tree.SetBranchStatus("EMTFTracks_*", 1)
    tree.SetBranchStatus("EMTFStubs_istation", 1)
    tree.SetBranchStatus("EMTFStubs_iring", 1)
    tree.SetBranchStatus("EMTFStubs_strip", 1)

    # __________________________________________________________________________
    # Loop over events
    for ievt, evt in enumerate(tree):
        if (ievt == options.nentries):  break
        if (ievt % 1000 == 0):  print "Processing event: %i" % ievt

        part_pt = evt.genParts_pt[0]
        part_phi = evt.genParts_phi[0]
        part_eta = evt.genParts_eta[0]
        #part_cottheta = sinh(evt.genParts_eta[0])
        #part_theta = atan2(evt.genParts_pt[0], evt.genParts_pz[0])
        #part_charge = evt.genParts_charge[0]

        part_globalPhiME2 = evt.genParts_globalPhiME[0][2]
        part_globalEtaME2 = evt.genParts_globalEtaME[0][2]
        #part_globalThetaME2 = evt.genParts_globalThetaME[0][2]

        # Select only endcap
        is_endcap = 1.25 < abs(part_eta) < 2.4

        # Find pt bin
        ptbin = find_l1t_pt_bin(part_pt)

        # Find eta bin
        etabin = find_l1t_eta_bin(part_eta)

        if options.verbose:
            print ".. %i gen pt: %f phi: %f eta: %f ptbin: %i etabin: %i" % (ievt, part_pt, part_phi, part_eta, ptbin, etabin)

        if ptbin >= 0 and is_endcap:
            best_quality, best_dR = 0, 99999999.
            best_itrack, best_mode, best_trig_pt, best_trig_phi, best_trig_eta = 0, 0, 0., 0., 0.

            # __________________________________________________________________
            # Loop over tracks
            for (itrack, mode, bx, sebx, trig_pt, trig_phi, trig_eta) in izip(count(), evt.EMTFTracks_mode, evt.EMTFTracks_firstBX, evt.EMTFTracks_secondBX, evt.EMTFTracks_pt, evt.EMTFTracks_phiGlbRad, evt.EMTFTracks_eta):

                quality = mode_to_quality(mode)
                dR = deltaR(trig_eta, trig_phi, part_globalEtaME2, part_globalPhiME2)
                if options.verbose:
                    print ".... %i l1t pt: %f phi: %f eta: %f mode: %i quality: %i dR: %f" % (itrack, trig_pt, trig_phi, trig_eta, mode, quality, dR)

                if True or (int(bx) == 0):  # don't check BX
                    if (best_quality < quality):
                        best_quality, best_dR = quality, dR
                        best_itrack, best_mode, best_trig_pt, best_trig_phi, best_trig_eta = itrack, mode, trig_pt, trig_phi, trig_eta
                    elif (best_quality == quality) and (best_dR > dR):
                        best_quality, best_dR = quality, dR
                        best_itrack, best_mode, best_trig_pt, best_trig_phi, best_trig_eta = itrack, mode, trig_pt, trig_phi, trig_eta

            if options.verbose:
                print ".. %i best l1t itrack: %i mode: %i pt: %f phi: %f eta: %f" % (ievt, best_itrack, best_mode, best_trig_pt, best_trig_phi, best_trig_eta)

            if best_dR < 0.3:
                best_mode_st = 0
                for istub, stubRef in enumerate(evt.EMTFTracks_stubRefs.at(best_itrack)):
                    istation, iring, strip = evt.EMTFStubs_istation.at(stubRef), evt.EMTFStubs_iring.at(stubRef), evt.EMTFStubs_strip.at(stubRef)
                    st = find_l1t_st_bin(istation, iring, strip)
                    if 3 <= st <= 4:
                        best_mode_st = st
                    if 1 <= st <= 2:
                        best_mode_st = st

                    if options.verbose:
                        print ".... %i best l1t stubRef: %i istation: %i iring: %i strip: %i st: %i" % (istub, stubRef, istation, iring, strip, st)

                modebin = find_l1t_mode_bin(best_mode, best_mode_st)
                qualbin = mode_to_quality(best_mode)
                if options.verbose:
                    print ".. %i modebin: %i qualbin: %i" % (ievt, modebin, qualbin)

                # Calc pT resolution
                #ptres = (best_trig_pt - part_pt) / part_pt
                ptres = (1.0/part_pt - 1.0/best_trig_pt) * part_pt

                # Fill histos
                etabin_tmp = etabin
                modebin_tmp = modebin

                for modebin in (modebin_tmp, l1t_mode_vec[-1]):
                    if modebin in l1t_mode_vec:
                        for etabin in (etabin_tmp, l1t_eta_vec[-1]):
                            hname = "resolution_pt_vs_pt_in_mode%i_eta%i" % (modebin, etabin)
                            histos[hname].Fill(part_pt, ptres)
                            hname = "resolution_pt_in_mode%i_eta%i" % (modebin, etabin)
                            histos[hname].Fill(ptres)
                        hname = "resolution_pt_in_mode%i_pt%i" % (modebin, ptbin)
                        histos[hname].Fill(ptres)
                        hname = "resolution_pt_vs_eta_in_mode%i_pt%i" % (modebin, ptbin)
                        histos[hname].Fill(part_eta, ptres)

                for etabin in (etabin_tmp, l1t_eta_vec[-1]):
                    hname = "resolution_pt_vs_pt_in_qual%i_eta%i" % (qualbin, etabin)
                    histos[hname].Fill(part_pt, ptres)
                    hname = "resolution_pt_in_qual%i_eta%i" % (qualbin, etabin)
                    histos[hname].Fill(ptres)
                hname = "resolution_pt_in_qual%i_pt%i" % (qualbin, ptbin)
                histos[hname].Fill(ptres)
                hname = "resolution_pt_vs_eta_in_qual%i_pt%i" % (qualbin, ptbin)
                histos[hname].Fill(part_eta, ptres)
            continue

        continue
    return

# ______________________________________________________________________________
def drawer_draw(histos, options):
    for hname, h in histos.iteritems():

        if h.ClassName() == "TH1F":
            if True:
                # Style
                if h.style == 0:  # filled
                    h.SetLineWidth(2); h.SetMarkerSize(0)
                    h.SetLineColor(col); h.SetFillColor(fcol)
                elif h.style == 1:  # marker
                    h.SetLineWidth(2); h.SetMarkerStyle(20); h.SetFillStyle(0)
                    h.SetLineColor(col); h.SetMarkerColor(col)
                if h.logy:
                    h.SetMaximum(h.GetMaximum() * 14); h.SetMinimum(0.5)
                else:
                    h.SetMaximum(h.GetMaximum() * 1.4); h.SetMinimum(0.)
                h.Draw("hist")
                gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
                CMS_label()
                save(options.outdir, hname)

        elif h.ClassName() == "TH2F":
            if True:
                h.SetStats(0); h.Draw("COLZ")
                gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
                CMS_label()
                save(options.outdir, hname)
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

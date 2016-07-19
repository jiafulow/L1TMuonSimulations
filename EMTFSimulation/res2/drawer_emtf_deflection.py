#!/usr/bin/env python

from rootdrawing import *
from math import sinh, atan2


# Configurations
col  = TColor.GetColor("#1f78b4")  # mu0
fcol = TColor.GetColor("#a6cee3")  # mu0
mcol = TColor.GetColor("#600000")

palette = map(lambda x: TColor.GetColor(x), ("#66DD66", "#3333FF", "#990099", "#FFBB44", "#EE4477", "#56D3DB", "#454545"))

l1t_pt_vec = (3,5,10,20,50,200,1000)
l1t_st_vec = (1,2,3,4,5,6,7,8,9,10)  # as defined for iChamberType (see DataFormats/MuonDetId/interface/CSCDetId.h)
l1t_std_vec = (1,2,3,4,5,6,7,8,9)  # 9 pairs


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

def find_l1t_std_bin(st1, st2):
    # Squash st to 5 values
    d = {1: 1, 2: 1, 3: 2, 5: 3, 6: 3, 7: 4, 8: 4, 9: 5, 10: 5}
    squash = lambda x : d[x]
    st1 = squash(st1)
    st2 = squash(st2)

    # If station 2 is available, use as the 'key' station,
    # else if station 3 is avail, use it,
    # else if station 4 is avail, use it.
    d = {
        301: 1, 302: 2, 304: 3, 305: 6,
        401: 4, 402: 5, 405: 9,
        501: 7, 502: 8,
    }
    std = st1*100 + st2
    if std in d:
        return d[std]
    return -1

def get_st_label(st):
    d = {1: 'ME+1/1a', 2: 'ME+1/1b', 3: 'ME+1/2', 4: 'ME+1/3', 5: 'ME+2/1', 6: 'ME+2/2', 7: 'ME+3/1', 8: 'ME+3/2', 9: 'ME+4/1', 10: 'ME+4/2'}
    ret = d[st]
    if options.east:
        ret = ret.replace("ME+", "ME-")
    return ret

def get_std_label(std):
    d = {1: ('ME+2','ME+1/1'), 2: ('ME+2','ME+1/2'), 3: ('ME+2','ME+3'), 4: ('ME+3','ME+1/1'), 5: ('ME+3','ME+1/2'), 6: ('ME+2','ME+4'), 7: ('ME+4','ME+1/1'), 8: ('ME+4','ME+1/2'), 9: ('ME+3', 'ME+4')}
    ret = d[std]
    if options.east:
        ret = (ret[0].replace("ME+", "ME-"), ret[1].replace("ME+", "ME-"))
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
    for ptbin in l1t_pt_vec:
        for stbin in l1t_st_vec:
            hname = "deviation_phi_in_st%i_pt%i" % (stbin, ptbin)
            htitle, nbinsx, xmin, xmax = "; #phi(%s) - #phi(muon) [rad]; entries" % (get_st_label(stbin)), 100, -0.2, 1.0
            if options.high_pt:  nbinsx, xmin, xmax = 100, -0.1, 0.2
            histos[hname] = TH1F(hname, htitle, nbinsx, xmin, xmax)
            histos[hname].indices = (stbin, ptbin)
            hname = "deviation_theta_in_st%i_pt%i" % (stbin, ptbin)
            htitle, nbinsx, xmin, xmax = "; #theta(%s) - #theta(muon) [rad]; entries" % (get_st_label(stbin)), 100, -0.2, 0.2
            if options.high_pt:  nbinsx, xmin, xmax = 100, -0.1, 0.1
            histos[hname] = TH1F(hname, htitle, nbinsx, xmin, xmax)
            histos[hname].indices = (stbin, ptbin)
            hname = "deviation_eta_in_st%i_pt%i" % (stbin, ptbin)
            htitle, nbinsx, xmin, xmax = "; #eta(%s) - #eta(muon); entries" % (get_st_label(stbin)), 100, -0.6, 0.6
            if options.high_pt:  nbinsx, xmin, xmax = 100, -0.2, 0.2
            histos[hname] = TH1F(hname, htitle, nbinsx, xmin, xmax)
            histos[hname].indices = (stbin, ptbin)

            hname = "nstubs_in_st%i_pt%i" % (stbin, ptbin)
            htitle, nbinsx, xmin, xmax = "; # LCTs in %s; entries" % (get_st_label(stbin)), 16, -0.5, 15.5
            histos[hname] = TH1F(hname, htitle, nbinsx, xmin, xmax)
            histos[hname].indices = (stbin, ptbin)

        for stdbin in l1t_std_vec:
            hname = "deflection_phi_in_std%i_pt%i" % (stdbin, ptbin)
            htitle, nbinsx, xmin, xmax = "; #phi(%s) - #phi(%s) [rad]; entries" % (get_std_label(stdbin)), 100, -0.2, 0.2
            if options.high_pt:  nbinsx, xmin, xmax = 100, -0.08, 0.08
            histos[hname] = TH1F(hname, htitle, nbinsx, xmin, xmax)
            histos[hname].indices = (stdbin, ptbin)
            hname = "deflection_theta_in_std%i_pt%i" % (stdbin, ptbin)
            htitle, nbinsx, xmin, xmax = "; #theta(%s) - #theta(%s) [rad]; entries" % (get_std_label(stdbin)), 100, -0.1, 0.1
            if options.high_pt:  nbinsx, xmin, xmax = 100, -0.08, 0.08
            histos[hname] = TH1F(hname, htitle, nbinsx, xmin, xmax)
            histos[hname].indices = (stdbin, ptbin)
            hname = "deflection_eta_in_std%i_pt%i" % (stdbin, ptbin)
            htitle, nbinsx, xmin, xmax = "; #eta(%s) - #eta(%s); entries" % (get_std_label(stdbin)), 100, -0.3, 0.3
            if options.high_pt:  nbinsx, xmin, xmax = 100, -0.16, 0.16
            histos[hname] = TH1F(hname, htitle, nbinsx, xmin, xmax)
            histos[hname].indices = (stdbin, ptbin)

    # TH2F and TProfile
    for ptbin in l1t_pt_vec:
        for stdbin in l1t_std_vec:
            hname = "deflection_phi_vs_eta_in_std%i_pt%i" % (stdbin, ptbin)
            htitle, nbinsx, xmin, xmax, nbinsy, ymin, ymax = "; #eta(muon); #phi(%s) - #phi(%s) [rad]" % (get_std_label(stdbin)), 100, -0.2, 0.2, 32, 0.9, 2.5
            histos[hname] = TH2F(hname, htitle, nbinsy, ymin, ymax, nbinsx, xmin, xmax)
            histos[hname].indices = (stdbin, ptbin)
            hname = "deflection_theta_vs_eta_in_std%i_pt%i" % (stdbin, ptbin)
            htitle, nbinsx, xmin, xmax, nbinsy, ymin, ymax = "; #eta(muon); #theta(%s) - #theta(%s) [rad]" % (get_std_label(stdbin)), 100, -0.1, 0.1, 32, 0.9, 2.5
            histos[hname] = TH2F(hname, htitle, nbinsy, ymin, ymax, nbinsx, xmin, xmax)
            histos[hname].indices = (stdbin, ptbin)
            hname = "deflection_eta_vs_eta_in_std%i_pt%i" % (stdbin, ptbin)
            htitle, nbinsx, xmin, xmax, nbinsy, ymin, ymax = "; #eta(muon); #eta(%s) - #eta(%s)" % (get_std_label(stdbin)), 100, -0.3, 0.3, 32, 0.9, 2.5
            histos[hname] = TH2F(hname, htitle, nbinsy, ymin, ymax, nbinsx, xmin, xmax)
            histos[hname].indices = (stdbin, ptbin)

            # error bar indicates std dev
            hname = "deflection_phi_pr_eta_in_std%i_pt%i" % (stdbin, ptbin)
            htitle, nbinsx, xmin, xmax, nbinsy, ymin, ymax = "; #eta(muon); #phi(%s) - #phi(%s) [rad]" % (get_std_label(stdbin)), 100, -0.2, 0.2, 32, 0.9, 2.5
            histos[hname] = TProfile(hname, htitle, nbinsy, ymin, ymax, xmin, xmax, 's')
            histos[hname].indices = (stdbin, ptbin)
            hname = "deflection_theta_pr_eta_in_std%i_pt%i" % (stdbin, ptbin)
            htitle, nbinsx, xmin, xmax, nbinsy, ymin, ymax = "; #eta(muon); #theta(%s) - #theta(%s) [rad]" % (get_std_label(stdbin)), 100, -0.1, 0.1, 32, 0.9, 2.5
            histos[hname] = TProfile(hname, htitle, nbinsy, ymin, ymax, xmin, xmax, 's')
            histos[hname].indices = (stdbin, ptbin)
            hname = "deflection_eta_pr_eta_in_std%i_pt%i" % (stdbin, ptbin)
            htitle, nbinsx, xmin, xmax, nbinsy, ymin, ymax = "; #eta(muon); #eta(%s) - #eta(%s)" % (get_std_label(stdbin)), 100, -0.3, 0.3, 32, 0.9, 2.5
            histos[hname] = TProfile(hname, htitle, nbinsy, ymin, ymax, xmin, xmax, 's')
            histos[hname].indices = (stdbin, ptbin)

    # Style
    for hname, h in histos.iteritems():
        if h.ClassName() == "TH1F":
            h.style = 0; h.logx = options.logx; h.logy = options.logy
        elif h.ClassName() == "TH2F":
            h.style = 0; h.logx = False; h.logy = False
        elif h.ClassName() == "TProfile":
            h.style = 0; h.logx = False; h.logy = False

        if h.ClassName() == "TH1F":
            if hname.startswith("deviation_phi") or hname.startswith("deviation_theta") or hname.startswith("deviation_eta") or \
                hname.startswith("deflection_phi") or hname.startswith("deflection_theta") or hname.startswith("deflection_eta") or \
                hname.startswith("nstubs_"):
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

        # Select only endcap
        is_endcap = 1.25 < abs(part_eta) < 2.4

        # Find pt bin
        ptbin = find_l1t_pt_bin(part_pt)

        if options.verbose:
            print ".. %i gen pt: %f phi: %f eta: %f ptbin: %i" % (ievt, part_pt, part_phi, part_eta, ptbin)

        if ptbin >= 0 and is_endcap:

            # __________________________________________________________________
            # Loop over stubs
            counters = {}

            for imuon1, vmuon1 in enumerate(izip(evt.CSCStubs_istation, evt.CSCStubs_iring, evt.CSCStubs_strip, evt.CSCStubs_globalPhi, evt.CSCStubs_globalTheta, evt.CSCStubs_globalEta)):
                (istation1, iring1, strip1, globalPhi1, globalTheta1, globalEta1) = vmuon1

                if istation1 == 1 and iring1 == 3:  # skip ME+1/3
                    continue

                stbin = find_l1t_st_bin(istation1, iring1, strip1)
                if stbin == -1:
                    raise Exception("Failed to get stbin. Why? %i %i %i" % (istation1, iring1, strip1))

                dphi = deltaPhi(globalPhi1, part_phi)
                dtheta = deltaPhi(globalTheta1, part_theta)
                deta = deltaEta(globalEta1, part_eta)

                if part_charge == 1:  # flip dphi for antimuons
                    dphi = -dphi

                # Fill histos
                hname = "deviation_phi_in_st%i_pt%i" % (stbin, ptbin)
                histos[hname].Fill(dphi)
                hname = "deviation_theta_in_st%i_pt%i" % (stbin, ptbin)
                histos[hname].Fill(dtheta)
                hname = "deviation_eta_in_st%i_pt%i" % (stbin, ptbin)
                histos[hname].Fill(deta)
                hname = "nstubs_in_st%i_pt%i" % (stbin, ptbin)
                counters[hname] = counters.get(hname, 0) + 1
                continue

            # Fill count histos
            for stbin in l1t_st_vec:
                hname = "nstubs_in_st%i_pt%i" % (stbin, ptbin)
                if hname in counters:
                    histos[hname].Fill(counters[hname])

            # __________________________________________________________________
            # Loop over pairs of stubs
            for imuon1, vmuon1 in enumerate(izip(evt.CSCStubs_istation, evt.CSCStubs_iring, evt.CSCStubs_strip, evt.CSCStubs_globalPhi, evt.CSCStubs_globalTheta, evt.CSCStubs_globalEta)):
                for imuon2, vmuon2 in enumerate(izip(evt.CSCStubs_istation, evt.CSCStubs_iring, evt.CSCStubs_strip, evt.CSCStubs_globalPhi, evt.CSCStubs_globalTheta, evt.CSCStubs_globalEta)):
                    (istation1, iring1, strip1, globalPhi1, globalTheta1, globalEta1) = vmuon1
                    (istation2, iring2, strip2, globalPhi2, globalTheta2, globalEta2) = vmuon2

                    if imuon1 >= imuon2:  # no double counting
                        continue
                    if istation1 == istation2:  # only if different station
                        continue
                    if istation1 == 1 and iring1 == 3:  # skip ME+1/3
                        continue
                    if istation2 == 1 and iring2 == 3:  # skip ME+1/3
                        continue

                    stbin1 = find_l1t_st_bin(istation1, iring1, strip1)
                    stbin2 = find_l1t_st_bin(istation2, iring2, strip2)
                    stdbin = find_l1t_std_bin(stbin1, stbin2)
                    if stdbin == -1:
                        (istation2, iring2, strip2, globalPhi2, globalTheta2, globalEta2) = vmuon1
                        (istation1, iring1, strip1, globalPhi1, globalTheta1, globalEta1) = vmuon2
                        stbin1 = find_l1t_st_bin(istation1, iring1, strip1)
                        stbin2 = find_l1t_st_bin(istation2, iring2, strip2)
                        stdbin = find_l1t_std_bin(stbin1, stbin2)
                    if stdbin == -1:
                        raise Exception("Failed to get stdbin. Why? %i %i %i %i %i %i" % (istation1, iring1, strip1, istation2, iring2, strip2))

                    dphi = deltaPhi(globalPhi1, globalPhi2)
                    dtheta = deltaPhi(globalTheta1, globalTheta2)
                    deta = deltaEta(globalEta1, globalEta2)

                    if part_charge == 1:  # flip dphi for antimuons
                        dphi = -dphi

                    # Fill histos
                    hname = "deflection_phi_in_std%i_pt%i" % (stdbin, ptbin)
                    histos[hname].Fill(dphi)
                    hname = "deflection_theta_in_std%i_pt%i" % (stdbin, ptbin)
                    histos[hname].Fill(dtheta)
                    hname = "deflection_eta_in_std%i_pt%i" % (stdbin, ptbin)
                    histos[hname].Fill(deta)

                    hname = "deflection_phi_vs_eta_in_std%i_pt%i" % (stdbin, ptbin)
                    histos[hname].Fill(part_eta, dphi)
                    hname = "deflection_theta_vs_eta_in_std%i_pt%i" % (stdbin, ptbin)
                    histos[hname].Fill(part_eta, dtheta)
                    hname = "deflection_eta_vs_eta_in_std%i_pt%i" % (stdbin, ptbin)
                    histos[hname].Fill(part_eta, deta)

                    hname = "deflection_phi_pr_eta_in_std%i_pt%i" % (stdbin, ptbin)
                    histos[hname].Fill(part_eta, dphi)
                    hname = "deflection_theta_pr_eta_in_std%i_pt%i" % (stdbin, ptbin)
                    histos[hname].Fill(part_eta, dtheta)
                    hname = "deflection_eta_pr_eta_in_std%i_pt%i" % (stdbin, ptbin)
                    histos[hname].Fill(part_eta, deta)
                    continue
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
            pass

        elif h.ClassName() == "TProfile":
            if True:
                # Draw profile plots on top of TH2
                h2 = histos[hname.replace("_pr_", "_vs_")]
                if h.style == 0:
                    h.SetMarkerStyle(20); h.SetMarkerColor(mcol); h.SetLineColor(mcol)
                h2.SetStats(0); h2.Draw("COLZ")
                h.SetStats(0); h.Draw("same")
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

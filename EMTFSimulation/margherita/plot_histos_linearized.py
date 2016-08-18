#!/usr/bin/env python

from ROOT import TFile, TH1F, TH2F, TProfile, TF1, TText, gPad, gROOT, gStyle
import numpy as np
import os

imgdir = "figures_linearized/"
histos = {}
figures = []

def style_tgraph(g):
    g.SetMarkerStyle(20); g.SetMarkerSize(0.5); g.SetMarkerColor(2)
    return

def style_th1f(h):
    h.SetLineWidth(2)
    return

def style_th2f(h):
    return

def moveStats(h, x1=0.64, y1=0.70, x2=0.88, y2=0.93, color=None):
    statsObj = h.FindObject("stats")
    statsObj.SetX1NDC(x1); statsObj.SetY1NDC(y1); statsObj.SetX2NDC(x2); statsObj.SetY2NDC(y2)
    if color:
        statsObj.SetTextColor(color)

def make_figures(infile='histos_linearized.root'):
    if not os.path.exists(imgdir):
        os.makedirs(imgdir)

    tfile = TFile.Open(infile)
    tfile.cd()
    for h in tfile.GetListOfKeys():
        h = h.ReadObj()
        try:
            h.SetDirectory(0)
        except:
            pass
        histos[h.GetName()] = h
    tfile.Close()

    for hname, h in histos.iteritems():
        if h.ClassName() == "TH1F":
            style_th1f(h)
            h.Draw("hist")
            imgname = hname
            gPad.Print(imgdir+imgname+".png")

        elif h.ClassName() == "TH2F":
            style_th2f(h)
            h.Draw("COLZ")
            imgname = hname
            gPad.Print(imgdir+imgname+".png")

        elif h.ClassName() == "TGraph":
            g = h
            style_tgraph(g)
            hname = "h_"+g.GetName()[2:]
            histos[hname].g = g

    gPad.DrawFrame(-1.,-1.,1.,1.)
    text_empty = TText(0., 0., "Empty")
    text_empty.SetTextAlign(22); text_empty.Draw()
    gPad.Print(imgdir+"empty.png")


    #print figures
    figures[:] = [
        'h_phi0_vs_p0',
        'h_phi0_vs_p1',
        'h_phi0_vs_p2',
        'h_phi0_vs_p4',
        'h_phi1_vs_p0',
        'h_phi1_vs_p1',
        'h_phi1_vs_p2',
        'h_phi1_vs_p4',
        'h_phi2_vs_p0',
        'h_phi2_vs_p1',
        'h_phi2_vs_p2',
        'h_phi2_vs_p4',
        'h_phi3_vs_p0',
        'h_phi3_vs_p1',
        'h_phi3_vs_p2',
        'h_phi3_vs_p4',

        'h_phi4_vs_p0',
        'h_phi4_vs_p1',
        'h_phi4_vs_p2',
        'h_phi4_vs_p4',
        'h_phi5_vs_p0',
        'h_phi5_vs_p1',
        'h_phi5_vs_p2',
        'h_phi5_vs_p4',

        'h_phi_of_z_corr0',
        'h_phi_of_z_corr1',
        'h_phi_of_zt_corr0',
        'h_phi_of_zt_corr1',
        'h_phi0_corr0',
        'empty',
        'h_phi0_f_corr0',
        'h_phi0_r_corr0',
        'h_phi1_corr0',
        'empty',
        'h_phi1_f_corr0',
        'h_phi1_r_corr0',
        'h_phi2_corr0',
        'empty',
        'h_phi2_f_corr0',
        'h_phi2_r_corr0',
        'h_phi3_corr0',
        'empty',
        'h_phi3_f_corr0',
        'h_phi3_r_corr0',

        'h_rho0_vs_p0',
        'h_rho0_vs_p1',
        'h_rho0_vs_p2',
        'h_rho0_vs_p4',
        'h_rho1_vs_p0',
        'h_rho1_vs_p1',
        'h_rho1_vs_p2',
        'h_rho1_vs_p4',
        'h_rho2_vs_p0',
        'h_rho2_vs_p1',
        'h_rho2_vs_p2',
        'h_rho2_vs_p4',
        'h_rho3_vs_p0',
        'h_rho3_vs_p1',
        'h_rho3_vs_p2',
        'h_rho3_vs_p4',

        'h_rho4_vs_p0',
        'h_rho4_vs_p1',
        'h_rho4_vs_p2',
        'h_rho4_vs_p4',
        'h_rho5_vs_p0',
        'h_rho5_vs_p1',
        'h_rho5_vs_p2',
        'h_rho5_vs_p4',

        'h_rho_of_z_corr0',
        'h_rho_of_z_corr1',
        'h_rho_of_zt_corr0',
        'h_rho_of_zt_corr1',
        'h_rho0_corr0',
        'empty',
        'h_rho0_f_corr0',
        'h_rho0_r_corr0',
        'h_rho1_corr0',
        'empty',
        'h_rho1_f_corr0',
        'h_rho1_r_corr0',
        'h_rho2_corr0',
        'empty',
        'h_rho2_f_corr0',
        'h_rho2_r_corr0',
        'h_rho3_corr0',
        'empty',
        'h_rho3_f_corr0',
        'h_rho3_r_corr0',
    ]
    return

def make_more_figures():
    def doit():
        style_th2f(h)
        th2 = False
        if th2:
            h.Draw("COLZ")
        else:
            h1 = h.Clone(h.GetName() + "_frame"); h1.Reset(); h.frame = h1
            h.frame.SetStats(0); h.frame.Draw()
            h.g.Fit("pol1", "Q")
            h.g.Draw("p")
            gPad.Modified(); gPad.Update()
            moveStats(h.g, 0.64, 0.76, 0.98, 0.995)
        gPad.Print(imgdir+"%s.png" % h.GetName())
        return

    h = histos["h_phi_of_z_corr0"]
    doit()
    h = histos["h_phi_of_z_corr1"]
    doit()
    h = histos["h_phi_of_zt_corr0"]
    doit()
    h = histos["h_phi_of_zt_corr1"]
    doit()
    h = histos["h_rho_of_z_corr0"]
    doit()
    h = histos["h_rho_of_z_corr1"]
    doit()
    h = histos["h_rho_of_zt_corr0"]
    doit()
    h = histos["h_rho_of_zt_corr1"]
    doit()

    for j in xrange(4):
        h = histos["h_phi%i_vs_p0" % j]
        doit()
        h = histos["h_phi%i_vs_p1" % j]
        doit()
        h = histos["h_phi%i_vs_p2" % j]
        doit()
        h = histos["h_phi%i_vs_p3" % j]
        doit()
        h = histos["h_phi%i_vs_p4" % j]
        doit()
        h = histos["h_rho%i_vs_p0" % j]
        doit()
        h = histos["h_rho%i_vs_p1" % j]
        doit()
        h = histos["h_rho%i_vs_p2" % j]
        doit()
        h = histos["h_rho%i_vs_p3" % j]
        doit()
        h = histos["h_rho%i_vs_p4" % j]
        doit()

    def doit():
        style_th2f(h)
        h.g.Draw("ap")
        ymin, ymax = h.g.GetHistogram().GetMinimum(), h.g.GetHistogram().GetMaximum()
        h1 = h.Clone(h.GetName() + "_frame"); h1.Reset(); h.frame = h1
        h.frame.SetStats(0); h.frame.Draw()
        h.frame.GetYaxis().SetRangeUser(ymin, ymax)
        h.g.Fit("pol1", "Q")
        h.g.Draw("p")
        gPad.Modified(); gPad.Update()
        moveStats(h.g, 0.64, 0.76, 0.98, 0.995)
        gPad.Print(imgdir+"%s.png" % h.GetName())
        return

    for j in xrange(4,6):
        h = histos["h_phi%i_vs_p0" % j]
        doit()
        h = histos["h_phi%i_vs_p1" % j]
        doit()
        h = histos["h_phi%i_vs_p2" % j]
        doit()
        h = histos["h_phi%i_vs_p3" % j]
        doit()
        h = histos["h_phi%i_vs_p4" % j]
        doit()
        h = histos["h_rho%i_vs_p0" % j]
        doit()
        h = histos["h_rho%i_vs_p1" % j]
        doit()
        h = histos["h_rho%i_vs_p2" % j]
        doit()
        h = histos["h_rho%i_vs_p3" % j]
        doit()
        h = histos["h_rho%i_vs_p4" % j]
        doit()

    def doit():
        style_th1f(ha)
        style_th1f(hb)
        ha.SetMaximum(max(ha.GetMaximum(), hb.GetMaximum()) * 1.4)
        ha.SetStats(1); ha.Draw()
        hb.SetLineColor(4); hb.Draw("sames")
        gPad.Modified(); gPad.Update()
        moveStats(ha, 0.73, 0.80, 0.98, 0.995)
        moveStats(hb, 0.73, 0.60, 0.98, 0.795, color=4)
        gPad.Print(imgdir+"%s.png" % ha.GetName())
        return

    for j in xrange(4):
        ha = histos["h_phi%i_corr0" % j]
        hb = histos["h_phi%i_corr1" % j]
        print "std: %f %f" % (ha.GetRMS(), hb.GetRMS())
        doit()
        ha = histos["h_phi%i_f_corr0" % j]
        hb = histos["h_phi%i_f_corr1" % j]
        doit()
        ha = histos["h_phi%i_r_corr0" % j]
        hb = histos["h_phi%i_r_corr1" % j]
        doit()
        ha = histos["h_rho%i_corr0" % j]
        hb = histos["h_rho%i_corr1" % j]
        print "std: %f %f" % (ha.GetRMS(), hb.GetRMS())
        doit()
        ha = histos["h_rho%i_f_corr0" % j]
        hb = histos["h_rho%i_f_corr1" % j]
        doit()
        ha = histos["h_rho%i_r_corr0" % j]
        hb = histos["h_rho%i_r_corr1" % j]
        doit()
    return

def serve_figures(htmlfile='index.html'):
    html = '''<html><head><link href="bootstrap.min.css" rel="stylesheet"></head><body><div class="container"><div class="row">%s</div></div></body></html>'''

    writeme = []
    for fig in figures:
        s = '''<div class="col-md-3"><img src="%s.png" class="img-responsive"></div>''' % fig
        writeme.append(s)
    writeme = '\n'.join(writeme)
    writeme = html % writeme

    if not os.path.isfile(imgdir+"bootstrap.min.css"):
        import subprocess
        subprocess.check_call(['wget', 'https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css'])
        subprocess.check_call(['mv', 'bootstrap.min.css', imgdir])

    with open(imgdir+htmlfile, 'w') as f:
        f.write(writeme)
    print "Check out the webpage at file://%s/%s" % (os.getcwd(), imgdir+htmlfile)
    return

def main():
    gROOT.LoadMacro("tdrstyle.C")
    gROOT.ProcessLine("setTDRStyle();")
    gROOT.ProcessLine("gErrorIgnoreLevel = kWarning;")
    gROOT.SetBatch(True)
    gStyle.SetOptStat(111110)
    gStyle.SetNdivisions(505, "XY")
    gStyle.SetPalette(57)  # kBird
    gStyle.SetNumberContours(100)

    make_figures()
    make_more_figures()
    serve_figures()

# ______________________________________________________________________________
if __name__ == "__main__":

    main()


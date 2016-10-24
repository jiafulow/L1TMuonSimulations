#!/usr/bin/env python

from rootdrawing import *
d = None
histos = None
options = None
figures = []

col  = TColor.GetColor("#e31a1c")  # nu140
fcol = TColor.GetColor("#fb9a99")  # nu140


# ______________________________________________________________________________
def display_fit(h, xxmin, xxmax, fun="pol2"):
    status = 99
    if h.Integral() > 0 and (xxmax > xxmin and h.Integral(h.FindFixBin(xxmin),h.FindFixBin(xxmax))):
        if fun == "pol2":
            f1 = TF1("f1", fun)
            f1.SetParLimits(0, -50., h.GetMaximum())
            fun = "f1"
        status = int(h.Fit(fun, "QB", "", xxmin, xxmax))
    if status == 0:
        h.fit = h.GetFunction(fun)
    else:
        h.fit = TF1("fa1", fun + "(0)")
    h.fit.SetLineWidth(2); h.fit.SetLineColor(darken_color(h.GetLineColor(),20))

# ______________________________________________________________________________
def do_trueNPV(hname="trueNPV"):
    h = histos[hname]
    h.Scale(1.0/h.Integral())
    h.SetTitle("; gen # PU; arb. unit")
    h.SetLineWidth(2); h.SetLineColor(4); h.SetFillStyle(0)

    h.SetStats(0); h.Draw("hist")
    gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
    CMS_label()
    imgname = hname
    save(options.outdir, imgname); figures.append(imgname); figures.append(imgname)
    return

# ______________________________________________________________________________
def do_ptcut(hname="rate_of_pt_in_eta3_bx1_mode2_pu0"):
    h = histos[hname]
    h.SetLineWidth(2); h.SetMarkerSize(0)
    h.SetLineColor(col); h.SetFillColor(fcol)

    h.Draw("hist")
    for j, l1ptbin in enumerate(d.l1t_l1pt_vec):
        l1pt = d.l1t_l1pt_thresholds[l1ptbin]
        rate = h.Integral(h.FindBin(l1pt), h.GetNbinsX())  #
        tlatex.DrawLatex(0.67, 0.70-j*0.04, "Entries w/ p_{T}>%3i: %.0f" % (l1pt, rate))

    gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
    CMS_label()
    imgname = hname
    save(options.outdir, imgname); figures.append(imgname)

    hname = hname.replace("rate_of_pt_", "rate_of_ptcut_")
    h = histos[hname]
    h.SetLineWidth(2); h.SetMarkerSize(0)
    h.SetLineColor(col); h.SetFillColor(fcol)

    h.Draw("hist")
    for j, l1ptbin in enumerate(d.l1t_l1pt_vec):
        l1pt = d.l1t_l1pt_thresholds[l1ptbin]
        rate = h.GetBinContent(h.FindBin(l1pt))  #
        tlatex.DrawLatex(0.67, 0.70-j*0.04, "Entries w/ p_{T}>%3i: %.0f" % (l1pt, rate))

    gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
    CMS_label()
    imgname = hname
    save(options.outdir, imgname); figures.append(imgname)
    return

# ______________________________________________________________________________
def do_etaphi(hname="rate_of_eta_in_bx1_mode2_l1pt0", hname1="rate_of_eta_in_bx1_mode2_l1pt1", rebin=0):
    def doit():
        h = histos[hname]
        if rebin:
            h.Rebin(rebin)
            h.Scale(1.0/rebin)
        h.SetLineWidth(2); h.SetMarkerSize(0)
        h.SetLineColor(col); h.SetFillColor(fcol)
        h.Draw("hist")

        gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
        CMS_label()
        imgname = hname
        save(options.outdir, imgname); figures.append(imgname)
    doit()

    hname = hname.replace("rate_of_eta_", "rate_of_phi_")
    doit()

    hname = hname1
    rebin = 2
    doit()

    hname = hname.replace("rate_of_eta_", "rate_of_phi_")
    doit()
    return

# ______________________________________________________________________________
def do_bx(hname="rate_of_bx_in_mode2_l1pt0", hname1="rate_of_bx_in_mode2_l1pt1", hname2="rate_of_bx_in_mode2_l1pt2", rebin=0):
    def doit():
        h = histos[hname]
        if rebin:
            h.Rebin(rebin)
            h.Scale(1.0/rebin)
        h.SetLineWidth(2); h.SetMarkerSize(0)
        h.SetLineColor(col); h.SetFillColor(fcol)
        h.Draw("hist")

        gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
        CMS_label()
        imgname = hname
        save(options.outdir, imgname); figures.append(imgname)
    doit()

    hname = hname1
    doit()

    hname = hname2
    doit()
    return

# ______________________________________________________________________________
def do_stack(special_hname, special_values, legend, label_0=None, label_1=None, label_2=None, label_3=None, label_4=None, label_5=None, exclusive=False):
    ymax = 0.
    prev = None
    for i, v in enumerate(reversed(special_values)):
        hname = special_hname % v
        hhname = hname + "_stack"
        h = histos[hname]
        hh = h.Clone(hhname)
        hh.__dict__ = h.__dict__
        histos[hhname] = hh
        if exclusive:
            if prev:
                hh.Add(prev)
            prev = hh
            ymax += h.GetMaximum()
        else:
            ymax = max(ymax, h.GetMaximum())

    hname = (special_hname + "_stack") % (special_values[-1])
    h = histos[hname]

    h1 = h.Clone(hname+"_frame"); h1.Reset()
    h1.SetMaximum(ymax); h1.SetMinimum(0.5)
    h1.SetStats(0); h1.Draw()
    gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)

    moveLegend(tlegend,0.66,0.80,0.90,0.92); tlegend.Clear()
    movePaveText(tpavetext, 0.66, 0.68, 0.90, 0.80); tpavetext.Clear()
    for i, v in enumerate(special_values):
        j = len(special_values) - i - 1
        hname = (special_hname + "_stack") % v
        h = histos[hname]
        h.SetFillColor(lighten_color(ctapalette[j],40)); h.SetLineColor(ctapalette[j])
        h.Draw("same hist")
        tlegend.AddEntry(h, legend(h), "f")
    if label_0 and label_0(h):  tpavetext.AddText(label_0(h))
    if label_1 and label_1(h):  tpavetext.AddText(label_1(h))
    if label_2 and label_2(h):  tpavetext.AddText(label_2(h))
    if label_3 and label_3(h):  tpavetext.AddText(label_3(h))
    if label_4 and label_4(h):  tpavetext.AddText(label_4(h))
    if label_5 and label_5(h):  tpavetext.AddText(label_5(h))
    tlegend.Draw()
    tpavetext.Draw()

    CMS_label()
    imgname = special_hname % (99)
    save(options.outdir, imgname); figures.append(imgname)
    return

# ______________________________________________________________________________
def do_overlay(special_hname, special_values, legend, label_0=None, label_1=None, label_2=None, label_3=None, label_4=None, label_5=None):
    hname = special_hname % (special_values[-1])
    h = histos[hname]

    ymax = h.GetMaximum()
    h1 = h.Clone(hname+"_frame"); h1.Reset()
    h1.SetMaximum(ymax); h1.SetMinimum(0.5)
    h1.SetStats(0); h1.Draw()
    gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)

    moveLegend(tlegend,0.66,0.80,0.90,0.92); tlegend.Clear()
    movePaveText(tpavetext, 0.66, 0.68, 0.90, 0.80); tpavetext.Clear()
    for i, v in enumerate(special_values):
        hname = special_hname % v
        h = histos[hname]
        h.SetFillStyle(0); h.SetLineColor(ctapalette[i])
        h.Draw("same hist")
        tlegend.AddEntry(h, legend(h), "l")
    if label_0 and label_0(h):  tpavetext.AddText(label_0(h))
    if label_1 and label_1(h):  tpavetext.AddText(label_1(h))
    if label_2 and label_2(h):  tpavetext.AddText(label_2(h))
    if label_3 and label_3(h):  tpavetext.AddText(label_3(h))
    if label_4 and label_4(h):  tpavetext.AddText(label_4(h))
    if label_5 and label_5(h):  tpavetext.AddText(label_5(h))
    tlegend.Draw()
    tpavetext.Draw()

    CMS_label()
    imgname = special_hname % (99)
    save(options.outdir, imgname); figures.append(imgname)
    return

# ______________________________________________________________________________
def do_fit(hname, rebin=0, xxmin=0, xxmax=0, label_0=None, col=col):
    h = histos[hname]
    if rebin:
        h.Rebin(rebin)
        h.Scale(1.0/rebin)
    h.SetLineWidth(2); h.SetMarkerStyle(20); h.SetFillStyle(0)
    h.SetLineColor(col); h.SetMarkerColor(col)
    h.SetStats(0); h.Draw("e")
    display_fit(h, xxmin, xxmax)
    tlatex.DrawLatex(0.20, 0.90, "#color[%i]{p_{0} = %.4f}" % (h.fit.GetLineColor(), h.fit.GetParameter(0)))
    tlatex.DrawLatex(0.20, 0.86, "#color[%i]{p_{1} = %.4f}" % (h.fit.GetLineColor(), h.fit.GetParameter(1)))
    tlatex.DrawLatex(0.20, 0.82, "#color[%i]{p_{2} = %.4f}" % (h.fit.GetLineColor(), h.fit.GetParameter(2)))
    if label_0 and label_0(h):  tlatex.DrawLatex(0.20, 0.76, label_0(h))

    gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
    CMS_label()
    imgname = hname
    save(options.outdir, imgname); figures.append(imgname)
    return


# ______________________________________________________________________________
def serve_figures(htmlfile='index.html'):
    html = '''<html><head><link href="bootstrap.min.css" rel="stylesheet"></head><body><div class="container"><div class="row">%s</div></div></body></html>'''

    writeme = []
    for fig in figures:
        s = '''<div class="col-md-6"><img src="%s.png" class="img-responsive"></div>''' % fig
        writeme.append(s)
    writeme = '\n'.join(writeme)
    writeme = html % writeme

    if not os.path.isfile(options.outdir+"bootstrap.min.css"):
        import subprocess
        subprocess.check_call(['wget', 'https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css'])
        subprocess.check_call(['mv', 'bootstrap.min.css', options.outdir])

    with open(options.outdir+htmlfile, 'w') as f:
        f.write(writeme)
    print "Check out the webpage at file://%s/%s" % (os.getcwd(), options.outdir+htmlfile)
    return


# ______________________________________________________________________________
# Main function
def main():
    global d
    global histos
    global options

    # Init
    drawer = MyDrawer()
    gROOT.SetBatch(True)

    # Import
    import importlib
    prog = sys.argv[0]
    prog = prog.replace(".py", "").replace("_pronto", "")
    d = importlib.import_module(prog)

    imgdir = sys.argv[1]
    if not imgdir.endswith('/'):
        imgdir += '/'
    histos, options = unpack_histos(imgdir)

    # Process
    do_trueNPV()
    do_ptcut()
    do_etaphi()
    do_bx()

    do_stack(special_hname="rate_of_ptcut_in_eta3_bx1_mode%i_pu0", special_values=(0,1,2),
        legend= (lambda h: d.get_l1t_mode_label(h.indices[2])),
        label_0=(lambda h: ""),
        exclusive=True,
        )

    do_stack(special_hname="rate_of_ptcut_in_eta%i_bx1_mode2_pu0", special_values=(2,1,0),
        legend= (lambda h: d.get_l1t_eta_label(h.indices[0])),
        label_0=(lambda h: d.get_l1t_mode_label(h.indices[2])),
        exclusive=True,
        )

    do_overlay(special_hname="rate_of_ptcut_in_eta3_bx%i_mode2_pu0", special_values=(1,0),
        legend= (lambda h: d.get_l1t_bx_label(h.indices[1]) if h.indices[1] != 1 else "BX = 0"),
        label_0=(lambda h: d.get_l1t_mode_label(h.indices[2])),
        )

    do_fit(hname="rate_of_pu_in_eta3_bx1_mode2_l1pt0", rebin=0, xxmin=2, xxmax=50,
        label_0=(lambda h: d.get_l1t_l1pt_label(h.indices[3])),
        col=ctapalette[2],
        )
    do_fit(hname="rate_of_pu_in_eta3_bx1_mode2_l1pt1", rebin=2, xxmin=16, xxmax=50,
        label_0=(lambda h: d.get_l1t_l1pt_label(h.indices[3])),
        col=ctapalette[1],
        )
    do_fit(hname="rate_of_pu_in_eta3_bx1_mode2_l1pt2", rebin=4, xxmin=32, xxmax=48,
        label_0=(lambda h: d.get_l1t_l1pt_label(h.indices[3])),
        col=ctapalette[0],
        )
    do_fit(hname="rate_of_pu_in_eta3_bx0_mode2_l1pt0", rebin=0, xxmin=2, xxmax=50,
        label_0=(lambda h: d.get_l1t_l1pt_label(h.indices[3])),
        col=ctapalette[2],
        )
    do_fit(hname="rate_of_pu_in_eta3_bx0_mode2_l1pt1", rebin=2, xxmin=4, xxmax=50,
        label_0=(lambda h: d.get_l1t_l1pt_label(h.indices[3])),
        col=ctapalette[1],
        )
    do_fit(hname="rate_of_pu_in_eta3_bx0_mode2_l1pt2", rebin=4, xxmin=4, xxmax=48,
        label_0=(lambda h: d.get_l1t_l1pt_label(h.indices[3])),
        col=ctapalette[0],
        )

    # Serve
    serve_figures()
    return


# ______________________________________________________________________________
if __name__ == "__main__":
    import sys
    if len(sys.argv) != 2:
        raise Exception("Please provide 'imgdir'")

    # Call the main function
    main()


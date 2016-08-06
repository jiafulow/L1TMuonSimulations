#!/usr/bin/env python

from rootdrawing import *
d = None
histos = None
options = None
figures = []

ptpalette = map(lambda x: TColor.GetColor(x), ("#66DD66", "#3333FF", "#990099", "#FFBB44", "#EE4477", "#56D3DB", "#454545"))
tlegend2 = tlegend.Clone()


# ______________________________________________________________________________
def display_fit(h, xxmin, xxmax, fun="gaus"):
    status = 99
    if h.Integral() > 0 and (xxmax > xxmin and h.Integral(h.FindFixBin(xxmin),h.FindFixBin(xxmax))):
        status = int(h.Fit(fun, "Q", "", xxmin, xxmax))
    if status == 0:
        h.fit = h.GetFunction(fun)
    else:
        h.fit = TF1("fa1", fun + "(0)")
    h.fit.SetLineWidth(2); h.fit.SetLineColor(darken_color(h.GetLineColor(),20))

def display_yfit(h, xxmin, xxmax, fun="gaus"):
    nbinsx, xmin, xmax = h.GetNbinsX(), h.GetXaxis().GetXmin(), h.GetXaxis().GetXmax()
    htitle, htitle2 = h.GetXaxis().GetTitle(), h.GetYaxis().GetTitle()
    h.yfit1 = h.ProjectionX(h.GetName()+"_yfit1"); h.yfit1.Reset()
    h.yfit2 = h.ProjectionX(h.GetName()+"_yfit2"); h.yfit2.Reset()
    h.yfit1.SetTitle(";"+htitle+";"+re.sub(r"(.*)({.*})", r"#mu(\1)\2", htitle2))
    h.yfit2.SetTitle(";"+htitle+";"+re.sub(r"(.*)({.*})", r"#sigma(\1)\2", htitle2))

    for b in xrange(1,nbinsx+1):
        py = h.ProjectionY("_py", b, b)
        if py.Integral() > 50:
            display_fit(py, xxmin, xxmax)
            p1, p2 = py.fit.GetParameter(1), py.fit.GetParameter(2)
            e1, e2 = py.fit.GetParError(1), py.fit.GetParError(2)
            is_fit_ok = (abs(p1 - py.GetMean())/py.GetRMS() < 1) and (1e-2 < p2/py.GetRMS() < 1e2)
            if not is_fit_ok:
                p1, p2 = py.GetMean(), py.GetRMS()
                e1, e2 = py.GetMeanError(), py.GetRMSError()
            h.yfit1.SetBinContent(b, p1)
            h.yfit1.SetBinError(b, p2)
            h.yfit2.SetBinContent(b, p2)
            h.yfit2.SetBinError(b, e2)

# ______________________________________________________________________________
def do_resolution_pt_vs_pt_overlays(special_hname = "resolution_pt_vs_pt_in_mode%i_eta%i"):
    l1t_eta_vec = d.l1t_eta_vec[:-1]  # no inclusive bin

    def doit():
        for i, etabin in enumerate(l1t_eta_vec):
            hname = special_hname % (mode, etabin)
            h = histos[hname]
            ymin, ymax = h.GetYaxis().GetXmin(), h.GetYaxis().GetXmax()
            yymin, yymax = h.GetMean(2)-4.0*h.GetRMS(2), h.GetMean(2)+4.0*h.GetRMS(2)
            display_yfit(h, yymin, yymax)

        for yfit in ["yfit1", "yfit2"]:
            hname = special_hname % (mode, l1t_eta_vec[-1])
            h = histos[hname]; h = getattr(h, yfit)
            htitle2 = h.GetYaxis().GetTitle()
            h1 = h.Clone(hname+"_frame"); h1.Reset()
            h1.GetYaxis().SetTitle(re.sub(r"(.*)({.*})", r"\1", htitle2))
            if yfit == "yfit1":
                h1.SetMinimum(ymin); h1.SetMaximum(ymax)
            else:
                h1.SetMinimum(0.); h1.SetMaximum(3.)
            h1.SetStats(0); h1.Draw()
            tlatex.DrawLatex(0.68, 0.76, re.sub(r"(.*){(.*)}", r"\2", htitle2))
            gPad.SetLogx(True)
            h1.GetXaxis().SetRangeUser(2, 2000)

            moveLegend(tlegend,0.66,0.80,0.90,0.92); tlegend.Clear()
            for i, etabin in enumerate(l1t_eta_vec):
                hname = special_hname % (mode, etabin)
                h = histos[hname]; h = getattr(h, yfit)
                h.SetLineWidth(2); h.SetMarkerStyle(20); h.SetFillStyle(0)
                h.SetLineColor(ctapalette[i]); h.SetMarkerColor(ctapalette[i])
                h.SetStats(0); h.Draw("same e")
                tlegend.AddEntry(h, d.get_l1t_eta_label(etabin), "p")
            tlegend.Draw()
            tlegend2.Draw()

            CMS_label()
            imgname = (special_hname % (mode, 99)) + ("_%s" % (yfit))
            save(options.outdir, imgname); figures.append(imgname)
            gPad.SetLogx(False)

    for mode in d.l1t_mode_vec:
        doit()

    special_hname = special_hname.replace("mode", "qual")
    for mode in d.l1t_quality_vec:
        doit()
    return

# ______________________________________________________________________________
def do_resolution_pt_vs_eta_overlays(special_hname = "resolution_pt_vs_eta_in_mode%i_pt%i"):
    l1t_pt_vec = [5,10,50,200,1000]

    def doit():
        for i, ptbin in enumerate(l1t_pt_vec):
            hname = special_hname % (mode, ptbin)
            h = histos[hname]
            ymin, ymax = h.GetYaxis().GetXmin(), h.GetYaxis().GetXmax()
            yymin, yymax = h.GetMean(2)-4.0*h.GetRMS(2), h.GetMean(2)+4.0*h.GetRMS(2)
            display_yfit(h, yymin, yymax)

        for yfit in ["yfit1", "yfit2"]:
            hname = special_hname % (mode, l1t_pt_vec[-1])
            h = histos[hname]; h = getattr(h, yfit)
            htitle2 = h.GetYaxis().GetTitle()
            h1 = h.Clone(hname+"_frame"); h1.Reset()
            h1.GetYaxis().SetTitle(re.sub(r"(.*)({.*})", r"\1", htitle2))
            if yfit == "yfit1":
                h1.SetMinimum(ymin); h1.SetMaximum(ymax)
            else:
                h1.SetMinimum(0.); h1.SetMaximum(3.)
            h1.SetStats(0); h1.Draw()
            tlatex.DrawLatex(0.68, 0.68, re.sub(r"(.*){(.*)}", r"\2", htitle2))

            moveLegend(tlegend,0.66,0.72,0.90,0.92); tlegend.Clear()
            for i, ptbin in enumerate(l1t_pt_vec):
                hname = special_hname % (mode, ptbin)
                h = histos[hname]; h = getattr(h, yfit)
                h.SetLineWidth(2); h.SetMarkerStyle(20); h.SetFillStyle(0)
                h.SetLineColor(ptpalette[i]); h.SetMarkerColor(ptpalette[i])
                h.SetStats(0); h.Draw("same e")
                tlegend.AddEntry(h, "p_{T} = %i GeV" % ptbin, "p")
            tlegend.Draw()
            tlegend2.Draw()

            CMS_label()
            imgname = (special_hname % (mode, 99)) + ("_%s" % (yfit))
            save(options.outdir, imgname); figures.append(imgname)

    for mode in d.l1t_mode_vec:
        doit()

    special_hname = special_hname.replace("mode", "qual")
    for mode in d.l1t_quality_vec:
        doit()
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
    do_resolution_pt_vs_pt_overlays()
    do_resolution_pt_vs_eta_overlays()

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


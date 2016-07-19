#!/usr/bin/env python

from rootdrawing import *
d = None
histos = None
options = None

figures = []


# ______________________________________________________________________________
def do_overlay(special_hname, special_values, legend, label_0=None, label_1=None, label_2=None, label_3=None, label_4=None, label_5=None):
    hname = special_hname % (special_values[-1])
    h = histos[hname]

    ymax = 1.1
    h1 = h.GetCopyTotalHisto().Clone(hname+"_frame"); h1.Reset()
    h1.SetMaximum(ymax); h1.SetMinimum(0)
    h1.SetStats(0); h1.Draw()
    gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)
    if "efficiency_of_pt" in special_hname:
        gPad.SetLogx(True)
        h1.GetXaxis().SetRangeUser(2, 1000)

    xmin, xmax = h1.GetXaxis().GetXmin(), h1.GetXaxis().GetXmax()
    tline2 = TLine(); tline2.SetLineColor(1)
    tline2.DrawLine(xmin, 1.0, xmax, 1.0)

    moveLegend(tlegend,0.66,0.20,0.90,0.32); tlegend.Clear()
    movePaveText(tpavetext, 0.66, 0.32, 0.90, 0.44); tpavetext.Clear()
    for i, v in enumerate(special_values):
        hname = special_hname % v
        h = histos[hname]
        h.gr = h.CreateGraph()
        h.gr.SetMarkerStyle(20); h.gr.SetMarkerSize(0.8); h.gr.SetMarkerColor(ctapalette[i]); h.gr.SetLineColor(ctapalette[i])
        h.gr.Draw("p")
        tlegend.AddEntry(h.gr, legend(h), "p")
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
    do_overlay(special_hname="efficiency_of_pt_in_eta3_bx1_mode2_l1pt%i", special_values=(2,1,0),
        legend=( lambda h: d.get_l1t_l1pt_label(h.indices[3])),
        label_0=(lambda h: "gen p_{T} > 20" if "efficiency_of_pt" not in h.GetName() else ""),
        label_1=(lambda h: d.get_l1t_eta_label(h.indices[0])),
        label_2=(lambda h: d.get_l1t_bx_label(h.indices[1])),
        label_3=(lambda h: d.get_l1t_mode_label(h.indices[2])),
        )

    do_overlay(special_hname="efficiency_of_eta_in_genpt2_bx1_mode2_l1pt%i", special_values=(2,1,0),
        legend= (lambda h: d.get_l1t_l1pt_label(h.indices[3])),
        label_0=(lambda h: "gen p_{T} > 20" if "efficiency_of_pt" not in h.GetName() else ""),
        label_1=(lambda h: d.get_l1t_genpt_label(h.indices[0])),
        label_2=(lambda h: d.get_l1t_bx_label(h.indices[1])),
        label_3=(lambda h: d.get_l1t_mode_label(h.indices[2])),
        )

    do_overlay(special_hname="efficiency_of_pt_in_eta3_bx1_mode%i_l1pt2", special_values=(2,1,0),
        legend= (lambda h: d.get_l1t_mode_label(h.indices[2])),
        label_0=(lambda h: "gen p_{T} > 20" if "efficiency_of_pt" not in h.GetName() else ""),
        label_1=(lambda h: d.get_l1t_eta_label(h.indices[0])),
        label_2=(lambda h: d.get_l1t_bx_label(h.indices[1])),
        label_3=(lambda h: d.get_l1t_l1pt_label(h.indices[3])),
        )

    do_overlay(special_hname="efficiency_of_eta_in_genpt2_bx1_mode%i_l1pt2", special_values=(2,1,0),
        legend= (lambda h: d.get_l1t_mode_label(h.indices[2])),
        label_0=(lambda h: "gen p_{T} > 20" if "efficiency_of_pt" not in h.GetName() else ""),
        label_1=(lambda h: d.get_l1t_genpt_label(h.indices[0])),
        label_2=(lambda h: d.get_l1t_bx_label(h.indices[1])),
        label_3=(lambda h: d.get_l1t_l1pt_label(h.indices[3])),
        )

    do_overlay(special_hname="efficiency_of_pt_in_eta3_bx%i_mode2_l1pt2", special_values=(1,0),
        legend= (lambda h: d.get_l1t_bx_label(h.indices[1]) if h.indices[1] != 1 else "BX = 0"),
        label_0=(lambda h: "gen p_{T} > 20" if "efficiency_of_pt" not in h.GetName() else ""),
        label_1=(lambda h: d.get_l1t_eta_label(h.indices[0])),
        label_2=(lambda h: d.get_l1t_mode_label(h.indices[2])),
        label_3=(lambda h: d.get_l1t_l1pt_label(h.indices[3])),
        )

    do_overlay(special_hname="efficiency_of_eta_in_genpt2_bx%i_mode2_l1pt2", special_values=(1,0),
        legend= (lambda h: d.get_l1t_bx_label(h.indices[1]) if h.indices[1] != 1 else "BX = 0"),
        label_0=(lambda h: "gen p_{T} > 20" if "efficiency_of_pt" not in h.GetName() else ""),
        label_1=(lambda h: d.get_l1t_genpt_label(h.indices[0])),
        label_2=(lambda h: d.get_l1t_mode_label(h.indices[2])),
        label_3=(lambda h: d.get_l1t_l1pt_label(h.indices[3])),
        )

    do_overlay(special_hname="efficiency_of_pt_in_eta%i_bx1_mode2_l1pt2", special_values=(0,1,2),
        legend= (lambda h: d.get_l1t_eta_label(h.indices[0])),
        label_0=(lambda h: "gen p_{T} > 20" if "efficiency_of_pt" not in h.GetName() else ""),
        label_1=(lambda h: d.get_l1t_bx_label(h.indices[1])),
        label_2=(lambda h: d.get_l1t_mode_label(h.indices[2])),
        label_3=(lambda h: d.get_l1t_l1pt_label(h.indices[3])),
        )

    do_overlay(special_hname="efficiency_of_eta_in_genpt%i_bx1_mode2_l1pt2", special_values=(0,1),
        legend= (lambda h: d.get_l1t_genpt_label(h.indices[0])),
        label_0=(lambda h: ""),
        label_1=(lambda h: d.get_l1t_bx_label(h.indices[1])),
        label_2=(lambda h: d.get_l1t_mode_label(h.indices[2])),
        label_3=(lambda h: d.get_l1t_l1pt_label(h.indices[3])),
        )

    do_overlay(special_hname="efficiency_of_pt_in_eta3_bx1_mode2_pu%i", special_values=(0,1,2,3),
        legend=( lambda h: d.get_l1t_pu_label(h.indices[3])),
        label_0=(lambda h: "gen p_{T} > 20" if "efficiency_of_pt" not in h.GetName() else ""),
        label_1=(lambda h: d.get_l1t_eta_label(h.indices[0])),
        label_2=(lambda h: d.get_l1t_bx_label(h.indices[1])),
        label_3=(lambda h: d.get_l1t_mode_label(h.indices[2])),
        )

    do_overlay(special_hname="efficiency_of_eta_in_genpt2_bx1_mode2_pu%i", special_values=(0,1,2,3),
        legend= (lambda h: d.get_l1t_pu_label(h.indices[3])),
        label_0=(lambda h: "gen p_{T} > 20" if "efficiency_of_pt" not in h.GetName() else ""),
        label_1=(lambda h: d.get_l1t_genpt_label(h.indices[0])),
        label_2=(lambda h: d.get_l1t_bx_label(h.indices[1])),
        label_3=(lambda h: d.get_l1t_mode_label(h.indices[2])),
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


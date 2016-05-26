#!/usr/bin/env python

from rootdrawing import *


pt_vec = [2., 3., 5., 10., 20., 50., 100.]
eta_vec = [1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4]
phi_vec = [3.14159265359/4]

stations_rmin = [946.753/2-58.6855, 1105.98/2-58.3355, 1319.28/2-57.9266, 1605.12/2-98.886]
stations_rmax = [946.753/2, 1105.98/2, 1319.28/2, 1605.12/2]

stations_zmin = [602.332 -22.358, 698.693 -23.0366, 828.392 -21.8752, 935.437 -22.1453, 1025.17 -21.7811]
stations_zmax = [602.332 +22.358, 698.693 +23.0367, 828.392 +21.8753, 935.437 +22.1453, 1025.17 +21.7810]

#station_zmin = [-602.298 -22.202, -699.064 -23.0214, -828.573 -21.8973, -935.498 -22.0365, -1024.88 -21.7631]
#station_zmax = [-602.298 +22.202, -699.064 +23.0214, -828.573 +21.8973, -935.498 +22.0366, -1024.88 +21.7631]


# ______________________________________________________________________________
# Drawer
def drawer_book(options):
    histos = {}

    hname = "hframe_phiz"
    histos[hname] = TH2F(hname, "; z [cm]; #phi [rad]", 100, 0, 1100, 100, 0.7, 2.0)

    hname = "hframe_phir"
    histos[hname] = TH2F(hname, "; r [cm]; #phi [rad]", 100, 0, 800, 100, 0.7, 2.0)

    # Style
    for hname, h in histos.iteritems():
        h.style = 0; h.logx = options.logx; h.logy = options.logy
    donotdelete.append(histos)
    return histos

# ______________________________________________________________________________
def drawer_project(tree, histos, options):

    for i in xrange(len(phi_vec)):
        for j in xrange(len(eta_vec)):
            for k in xrange(len(pt_vec)):

                selection = "%.2f <= pt && pt <= %.2f && %.2f <= eta && eta <= %.2f && %.2f <= phi && phi <= %.2f && hits_rho>0.01" % (pt_vec[k]*0.99, pt_vec[k]*1.01, eta_vec[j]*0.99, eta_vec[j]*1.01, phi_vec[i]*0.99, phi_vec[i]*1.01)
                option = "goff"

                varexp = "hits_phi:hits_z"
                n = tree.Draw(varexp, selection, option)
                grname = "propagate_phiz_pt%i_eta%i_phi%i" % (k, j, i)
                gr = TGraph(n, tree.GetV2(), tree.GetV1())
                gr.SetName(grname)
                histos[grname] = gr

                varexp = "hits_phi:hits_rho"
                n = tree.Draw(varexp, selection, option)
                grname = "propagate_phir_pt%i_eta%i_phi%i" % (k, j, i)
                gr = TGraph(n, tree.GetV2(), tree.GetV1())
                gr.SetName(grname)
                histos[grname] = gr
    return

# ______________________________________________________________________________
def drawer_draw(histos, options):

    tpolylines = []
    for zmin, zmax in izip(stations_zmin, stations_zmax):
        x1, x2, y1, y2 = zmin, zmax, 0.7, 2.0
        tpolyline = TPolyLine(5, array('d', [x1, x2, x2, x1, x1]), array('d', [y1, y1, y2, y2, y1]))
        tpolyline.SetFillColor(38)
        tpolyline.SetFillStyle(3004)
        tpolylines.append(tpolyline)

    # Specialized: fixed eta
    for hvar in ["phiz"]:
        i = 0
        for j in xrange(len(eta_vec)):
            hframe = histos["hframe_%s" % (hvar)]
            hframe.SetStats(0); hframe.Draw()
            for tpolyline in tpolylines:
                tpolyline.Draw("f")

            moveLegend(tlegend,0.1,0.64,0.5,0.92); tlegend.Clear()
            for k in xrange(len(pt_vec)):
                grname = "propagate_%s_pt%i_eta%i_phi%i" % (hvar, k, j, i)
                gr = histos[grname]
                gr.SetMarkerStyle(6)
                gr.SetMarkerColor(palette[k])
                gr.Draw("p")
                tlegend.AddEntry(gr, "#color[%i]{p_{T} = %5.0f GeV, #eta = %.1f}" % (palette[k], pt_vec[k], eta_vec[j]), "")
            tlegend.Draw()

            CMS_label()
            save(options.outdir, "propagate_%s_eta%i" % (hvar, j))

    # Specialized: fixed pT
    for hvar in ["phiz"]:
        i = 0
        for k in xrange(len(pt_vec)):
            hframe = histos["hframe_%s" % (hvar)]
            hframe.SetStats(0); hframe.Draw()
            for tpolyline in tpolylines:
                tpolyline.Draw("f")

            moveLegend(tlegend,0.1,0.64,0.5,0.92); tlegend.Clear()
            for j in xrange(len(eta_vec)):
                grname = "propagate_%s_pt%i_eta%i_phi%i" % (hvar, k, j, i)
                gr = histos[grname]
                gr.SetMarkerStyle(6)
                gr.SetMarkerColor(palette[k])
                gr.Draw("p")
                tlegend.AddEntry(gr, "#color[%i]{p_{T} = %5.0f GeV, #eta = %.1f}" % (palette[k], pt_vec[k], eta_vec[j]), "")
            tlegend.Draw()

            CMS_label()
            save(options.outdir, "propagate_%s_pt%i" % (hvar, k))

    donotdelete.append(tpolylines)
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
    #tchain = TChain("ntupler/tree", "")
    tchain = TChain("tree", "")
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

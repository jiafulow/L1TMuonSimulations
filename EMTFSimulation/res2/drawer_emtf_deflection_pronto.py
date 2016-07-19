#!/usr/bin/env python

from rootdrawing import *
d = None
histos = None
options = None
figures = []
tables = {}

palette = map(lambda x: TColor.GetColor(x), ("#66DD66", "#3333FF", "#990099", "#FFBB44", "#EE4477", "#56D3DB", "#454545"))
tlegend2 = tlegend.Clone()


# ______________________________________________________________________________
def display_fit(h, xxmin, xxmax, fun="gaus"):
    if h.Integral() > 0 and (xxmax > xxmin and h.Integral(h.FindFixBin(xxmin),h.FindFixBin(xxmax))):
        h.Fit(fun, "q", "", xxmin, xxmax)
        h.fit = h.GetFunction(fun)
        h.fit.SetLineWidth(2); h.fit.SetLineColor(darken_color(h.GetLineColor(),20))
    else:
        h.fit = TF1("fa1", fun + "(0)")

# ______________________________________________________________________________
def do_deviation_overlays(special_hname = "deviation_%s_in_st%i_pt%i", variables=["phi", "theta", "eta"]):
    for var in variables:
        for st in d.l1t_st_vec:
            if st == 4:  # skip ME+1/3
                continue
            if options.high_pt:
                hname = special_hname % (var, st, 1000)
            else:
                hname = special_hname % (var, st, 50)
            h = histos[hname]
            h1 = h.Clone(hname+"_frame"); h1.Reset()
            h1.SetMaximum(h1.GetMaximum()*14); h1.SetMinimum(0.5)
            h1.SetStats(0); h1.Draw()
            gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)

            moveLegend(tlegend,0.06,0.68,0.52,0.94); tlegend.Clear()
            moveLegend(tlegend2,0.46,0.68,0.92,0.94); tlegend2.Clear()
            for i, pt in enumerate(d.l1t_pt_vec):
                hname = special_hname % (var, st, pt)
                h = histos[hname]
                if options.high_pt:
                    if pt < 50:  continue
                    xmin, xmax = h.GetXaxis().GetXmin(), h.GetXaxis().GetXmax()
                    xxmin, xxmax = h.GetMean()-min((xmax-xmin)/8.0,4.0*h.GetRMS()), h.GetMean()+min((xmax-xmin)/8.0,4.0*h.GetRMS())
                else:
                    if pt > 50:  continue
                    xxmin, xxmax = h.GetMean()-4.0*h.GetRMS(), h.GetMean()+4.0*h.GetRMS()
                h.SetLineWidth(2); h.SetMarkerStyle(20); h.SetFillStyle(0)
                h.SetLineColor(palette[i]); h.SetMarkerColor(palette[i])
                h.SetStats(0); h.Draw("same e")
                display_fit(h, xxmin, xxmax)
                tlegend.AddEntry(h, "#color[%i]{#mu(p_{T} = %i GeV) = %.2fe-03}" % (h.fit.GetLineColor(), pt, h.fit.GetParameter(1)*1e3), "")
                tlegend2.AddEntry(h, "#color[%i]{#sigma(p_{T} = %i GeV) = %.2fe-03}" % (h.fit.GetLineColor(), pt, h.fit.GetParameter(2)*1e3), "")
                tables[hname] = h.fit.GetParameter(2)*1e3
            tlegend.Draw()
            tlegend2.Draw()

            CMS_label()
            imgname = special_hname % (var, st, 99)
            save(options.outdir, imgname); figures.append(imgname)
    return

# ______________________________________________________________________________
def do_nstubs_overlays(special_hname = "%s_in_st%i_pt%i", variables=["nstubs"]):
    for var in variables:
        for st in d.l1t_st_vec:
            if st == 4:  # skip ME+1/3
                continue
            if options.high_pt:
                hname = special_hname % (var, st, 1000)
            else:
                hname = special_hname % (var, st, 50)
            h = histos[hname]
            h1 = h.Clone(hname+"_frame"); h1.Reset()
            h1.SetMaximum(h1.GetMaximum()*14); h1.SetMinimum(0.5)
            h1.SetStats(0); h1.Draw()
            gPad.SetLogx(h.logx); gPad.SetLogy(h.logy or True)

            #moveLegend(tlegend,0.06,0.68,0.52,0.94); tlegend.Clear()
            #moveLegend(tlegend2,0.46,0.68,0.92,0.94); tlegend2.Clear()
            for i, pt in enumerate(d.l1t_pt_vec):
                hname = special_hname % (var, st, pt)
                h = histos[hname]
                if options.high_pt:
                    if pt < 50:  continue
                else:
                    if pt > 50:  continue
                h.SetLineWidth(2); h.SetMarkerStyle(20); h.SetFillStyle(0)
                h.SetLineColor(palette[i]); h.SetMarkerColor(palette[i])
                h.SetMinimum(0.5)
                h.SetStats(0); h.Draw("same hist")
            #tlegend.Draw()
            #tlegend2.Draw()

            CMS_label()
            imgname = special_hname % (var, st, 99)
            save(options.outdir, imgname); figures.append(imgname)
    return

# ______________________________________________________________________________
def do_deflection_overlays(special_hname = "deflection_%s_in_std%i_pt%i", variables=["phi", "theta", "eta"]):
    for var in variables:
        for std in d.l1t_std_vec:
            if options.high_pt:
                hname = special_hname % (var, std, 1000)
            else:
                hname = special_hname % (var, std, 50)
            h = histos[hname]
            h1 = h.Clone(hname+"_frame"); h1.Reset()
            h1.SetMaximum(h1.GetMaximum()*14); h1.SetMinimum(0.5)
            h1.SetStats(0); h1.Draw()
            gPad.SetLogx(h.logx); gPad.SetLogy(h.logy)

            moveLegend(tlegend,0.06,0.68,0.52,0.94); tlegend.Clear()
            moveLegend(tlegend2,0.46,0.68,0.92,0.94); tlegend2.Clear()
            for i, pt in enumerate(d.l1t_pt_vec):
                hname = special_hname % (var, std, pt)
                h = histos[hname]
                if options.high_pt:
                    if pt < 50:  continue
                    xmin, xmax = h.GetXaxis().GetXmin(), h.GetXaxis().GetXmax()
                    xxmin, xxmax = h.GetMean()-min((xmax-xmin)/8.0,4.0*h.GetRMS()), h.GetMean()+min((xmax-xmin)/8.0,4.0*h.GetRMS())
                else:
                    if pt > 50:  continue
                    xxmin, xxmax = h.GetMean()-4.0*h.GetRMS(), h.GetMean()+4.0*h.GetRMS()
                h.SetLineWidth(2); h.SetMarkerStyle(20); h.SetFillStyle(0)
                h.SetLineColor(palette[i]); h.SetMarkerColor(palette[i])
                h.SetStats(0); h.Draw("same e")
                display_fit(h, xxmin, xxmax)
                tlegend.AddEntry(h, "#color[%i]{#mu(p_{T} = %i GeV) = %.2fe-03}" % (h.fit.GetLineColor(), pt, h.fit.GetParameter(1)*1e3), "")
                tlegend2.AddEntry(h, "#color[%i]{#sigma(p_{T} = %i GeV) = %.2fe-03}" % (h.fit.GetLineColor(), pt, h.fit.GetParameter(2)*1e3), "")
                tables[hname] = h.fit.GetParameter(2)*1e3
            tlegend.Draw()
            tlegend2.Draw()

            CMS_label()
            imgname = special_hname % (var, std, 99)
            save(options.outdir, imgname); figures.append(imgname)
    return

# ______________________________________________________________________________
def serve_figures(htmlfile='index.html'):
    html = '''<html><head><link href="bootstrap.min.css" rel="stylesheet"></head><body><div class="container"><div class="row">%s</div></div></body></html>'''

    writeme = []
    for fig in figures:
        s = '''<div class="col-md-4"><img src="%s.png" class="img-responsive"></div>''' % fig
        writeme.append(s)
    writeme = '\n'.join(writeme)

    writeme2 = []  # hardcoded tables
    deflection_table = '''<table class="table table-striped"><thead><tr><th>%s</th><th>%s</th><th>%s</th><th>%s</th><th>%s</th><th>%s</th><th>%s</th><th>%s</th></tr></thead><tbody>%s</tbody></table>'''
    deflection_td = '''<tr><th scope="row">%s</th><td>%.2f</td><td>%.2f</td><td>%.2f</td><td>%.2f</td><td>%.2f</td><td>%.2f</td><td>%.2f</td></tr>'''
    deflection = [
         ('deflection<br/>angle &sigma;', '3 GeV', '5 GeV', '10 GeV', '20 GeV', '50 GeV', '200 GeV', '1 TeV') ,
         ('&Delta;&phi;(1/1,2)', 28.70057378635474, 16.361480702092567, 7.969275172797519, 4.125704057366091  , 1.878301603532725, 1.3205705216766, 1.4741468334125118) ,
         ('&Delta;&phi;(1/2,2)', 15.93307938434108, 7.16290112688564, 3.049482337948278, 2.0243328187078906   , 1.0546040835772938, 0.7464435620438112, 0.898699117383353) ,
         ('&Delta;&phi;(2,3)', 14.845150909580791, 5.770863401259427, 2.98232549683461, 2.0866757944935164    , 1.3117058063994678, 1.287431428843035, 1.6008755415943996) ,
         ('&Delta;&phi;(1/1,3)', 32.620694365002144, 18.705695775017805, 9.298434828555719, 4.797531610317441 , 2.1291238878971375, 1.3197818212120525, 1.4762906078957803) ,
         ('&Delta;&phi;(1/2,3)', 26.870533553026302, 11.650589971102136, 4.957309235214656, 2.6263084277673676, 1.1042036015568462, 0.7426533525304362, 0.893261649773898) ,
         ('&Delta;&phi;(2,4)', 27.012192793161038, 10.471083429596854, 4.2903645809543764, 2.604194030676988  , 1.3812255582275803, 1.2778405781422022, 1.5357247741356967) ,
         ('&Delta;&phi;(1/1,4)', 38.4451368918996, 20.831791762810255, 10.310130184853579, 5.272492172486817  , 2.2912766346477595, 1.341404269766481, 1.4482486647940933) ,
         ('&Delta;&phi;(1/2,4)', 37.57395241746872, 15.258095423160649, 6.281199826494449, 3.100339397410178  , 1.2547374444653256, 0.7654158086393191, 0.9057637989202223) ,
         ('&Delta;&phi;(3,4)', 13.39106547571133, 5.153252431567552, 2.4983439914403927, 2.0236897407424985   , 1.1762094714827038, 1.2240492450713683, 1.4792478923374794) ,
    ]
    s = (deflection_table % (deflection[0] + ('\n'.join([(deflection_td % d) for d in deflection[1:]]),) ))
    writeme2.append(s)

    deflection = [
         ('deflection<br/>angle &sigma;', '3 GeV', '5 GeV', '10 GeV', '20 GeV', '50 GeV', '200 GeV', '1 TeV') ,
         ('&Delta;&theta;(1/1,2)', 7.806085071211806, 4.2637263052265295, 2.716045015891165, 2.334264918894411  , 2.30096701784377, 3.0470449179234915, 8.121679886894162) ,
         ('&Delta;&theta;(1/2,2)', 7.2971060535285135, 3.3470133804745577, 2.3094231495013053, 2.132273301664383, 2.06923240178122, 2.212313567159643, 2.741385895387034) ,
         ('&Delta;&theta;(2,3)', 3.8915567008146996, 2.3114919909668448, 1.6798672698697623, 1.5952477232068698 , 1.4835538406213662, 1.8993767824907666, 6.345519459903542) ,
         ('&Delta;&theta;(1/1,3)', 9.672452437289728, 5.530619602200526, 3.4354550273422912, 2.8173328456870044 , 2.7129657980930624, 3.945139281034762, 7.815492939591785) ,
         ('&Delta;&theta;(1/2,3)', 10.747562411008309, 4.88719556667945, 2.7415574670590543, 2.198685097941244  , 2.0498220341806723, 2.1613687497258263, 2.7186701121701398) ,
         ('&Delta;&theta;(2,4)', 6.528959694289758, 3.5811223328500064, 2.2072100285559046, 1.8833709220698258  , 1.866926783439047, 2.5317404816996665, 6.111556756067449) ,
         ('&Delta;&theta;(1/1,4)', 11.438104491986696, 6.518299518321793, 4.019130504015324, 3.252530789828267  , 3.1302952536249635, 4.285057628177638, 7.796967217426792) ,
         ('&Delta;&theta;(1/2,4)', 14.086838535819725, 6.456652219574493, 3.272845712362833, 2.3930668348455337 , 2.080123035075458, 2.1663330693157197, 2.771726087040243) ,
         ('&Delta;&theta;(3,4)', 3.1558142064707395, 1.9305366592919273, 1.5929311305160896, 1.5774541695295685 , 1.451324927756465, 1.7539408658141464, 5.210050070879754) ,
    ]
    s = (deflection_table % (deflection[0] + ('\n'.join([(deflection_td % d) for d in deflection[1:]]),) ))
    writeme2.append(s)

    deviation_table = deflection_table
    deviation_td = deflection_td
    deviation = [
         ('deviation<br/>angle &sigma;', '3 GeV', '5 GeV', '10 GeV', '20 GeV', '50 GeV', '200 GeV', '1 TeV') ,
         ('&Delta;&phi;(1/1a)', 18.2675265818344, 10.992747079094137, 5.597725256392353, 3.1603115122670427 , 1.759338720441058, 1.4741502978044287, 1.5668880949559876) ,
         ('&Delta;&phi;(1/1b)', 19.17845524793655, 11.356472535528832, 5.745710190286591, 3.0873126213371123, 1.4923100536156004, 1.060483458642753, 1.0821505690782864) ,
         ('&Delta;&phi;(1/2)', 29.420460646777315, 16.62157705548697, 8.186822125981543, 4.147610004064279  , 1.736175860930336, 0.8540963637686427, 0.8509418786583511) ,
         ('&Delta;&phi;(2/1)', 33.64557822422797, 19.235873859252216, 9.48212225873724, 4.80656787027975    , 2.157617902616487, 1.2967092185880364, 1.2988617376109686) ,
         ('&Delta;&phi;(2/2)', 37.54218662052018, 20.527558783759734, 9.910045549668276, 5.009232044779859  , 2.01487560246295, 0.8774472691691416, 0.8672495298329862) ,
         ('&Delta;&phi;(3/1)', 39.9755335126921, 22.413409379364765, 10.932114410958194, 5.545762218360833  , 2.421438300573709, 1.323667732087448, 1.313018645331049) ,
         ('&Delta;&phi;(3/2)', 42.7435075114248, 23.109463618999214, 10.897763438733513, 5.495329122041658  , 2.2197241867117086, 0.9161793069436273, 0.8932090311028292) ,
         ('&Delta;&phi;(4/1)', 45.13486083849123, 25.03969319922153, 12.143141049718494, 6.080424285704711  , 2.6230978225265438, 1.3409189661223537, 1.3070825228851748) ,
         ('&Delta;&phi;(4/2)', 48.22316567786325, 25.63879964476003, 11.960749179307726, 5.983621360103173  , 2.3906683617480704, 0.9281649176472236, 0.8944234758856104) ,
    ]
    s = (deviation_table % (deviation[0] + ('\n'.join([(deviation_td % d) for d in deviation[1:]]),) ))
    writeme2.append(s)

    deviation = [
         ('deviation<br/>angle &sigma;', '3 GeV', '5 GeV', '10 GeV', '20 GeV', '50 GeV', '200 GeV', '1 TeV') ,
         ('&Delta;&theta;(1/1a)', 5.384975611990646, 4.039578568269869, 3.2414705824988523, 3.057255574903799 , 2.9953165153964463, 3.41275988257737, 6.285463735195674) ,
         ('&Delta;&theta;(1/1b)', 6.824358150625172, 4.804809900719903, 3.7120714664078087, 3.4166719939364154, 3.3416803926765635, 3.64901892634449, 4.530857409204292) ,
         ('&Delta;&theta;(1/2)', 13.491588304323834, 8.224504824778617, 5.006288821232986, 3.8991633478065038 , 3.5378539397711815, 3.713131508977602, 4.362833226489877) ,
         ('&Delta;&theta;(2/1)', 9.17997225586555, 5.517832906951735, 3.201983753486181, 2.3439682888833637   , 2.052426800648229, 2.171306163650826, 2.72513816050523) ,
         ('&Delta;&theta;(2/2)', 17.75616702320496, 10.139090548174236, 5.631395612707707, 3.999816922121263  , 3.4117254726786173, 3.4919402839328764, 4.135374553141277) ,
         ('&Delta;&theta;(3/1)', 10.181614781627395, 5.893160699544359, 3.230808140369706, 2.185119883819388  , 1.7911252009407446, 1.8633057886061333, 2.368075181649926) ,
         ('&Delta;&theta;(3/2)', 19.046640845274617, 10.62795579735836, 5.5755586010492015, 3.7134251445396487, 2.9834238956931345, 3.0110894394356267, 3.3799745292838526) ,
         ('&Delta;&theta;(4/1)', 10.924402912163197, 6.20619076286386, 3.268377876339254, 2.111243340199368   , 1.6291158266569028, 1.6789865929067864, 2.177318478856732) ,
         ('&Delta;&theta;(4/2)', 20.455114264371776, 11.227917845043008, 5.668066015798004, 3.6037029404965444, 2.7538031628674533, 2.7039206169088335, 3.120302224025974) ,
    ]
    s = (deviation_table % (deviation[0] + ('\n'.join([(deviation_td % d) for d in deviation[1:]]),) ))
    writeme2.append(s)
    writeme2 = '\n'.join(writeme2)

    writeme = html % (writeme + writeme2)

    if not os.path.isfile(options.outdir+"bootstrap.min.css"):
        import subprocess
        subprocess.check_call(['wget', 'https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css'])
        subprocess.check_call(['mv', 'bootstrap.min.css', options.outdir])

    with open(options.outdir+htmlfile, 'w') as f:
        f.write(writeme)
    print "Check out the webpage at file://%s/%s" % (os.getcwd(), options.outdir+htmlfile)
    return

# ______________________________________________________________________________
def serve_tables():
    def pretty_print(l, name=''):
        if name:
            print name, '=',
        print '['
        for ll in l:
            print '    ',
            print ll,
            print ','
        print ']'

    # deflection phi
    deflection_th_row = ["deflection<br/>angle &sigma;", "&Delta;&phi;(1/1,2)", "&Delta;&phi;(1/2,2)", "&Delta;&phi;(2,3)", "&Delta;&phi;(1/1,3)", "&Delta;&phi;(1/2,3)", "&Delta;&phi;(2,4)", "&Delta;&phi;(1/1,4)", "&Delta;&phi;(1/2,4)", "&Delta;&phi;(3,4)",]
    deflection_th = ["3 GeV", "5 GeV", "10 GeV", "20 GeV", "50 GeV", "200 GeV", "1 TeV",]
    var = "phi"
    special_hname = "deflection_%s_in_std%i_pt%i"
    def doit():
        deflection = []
        t = deflection_th_row[0:1] + deflection_th
        deflection.append(tuple(t))
        for i, std in enumerate(d.l1t_std_vec):
            t = deflection_th_row[i+1:i+2] + [tables.get(special_hname % (var, std, pt), 0) for pt in d.l1t_pt_vec]
            deflection.append(tuple(t))
        pretty_print(deflection, name="deflection")
    doit()

    # deflection theta
    deflection_th_row = [s.replace('&phi;', '&theta;') for s in deflection_th_row]
    var = "theta"
    doit()

    # deviation phi
    deviation_th_row = ["deviation<br/>angle &sigma;", "&Delta;&phi;(1/1a)", "&Delta;&phi;(1/1b)", "&Delta;&phi;(1/2)", "&Delta;&phi;(1/3)", "&Delta;&phi;(2/1)", "&Delta;&phi;(2/2)", "&Delta;&phi;(3/1)", "&Delta;&phi;(3/2)", "&Delta;&phi;(4/1)", "&Delta;&phi;(4/2)",]
    deviation_th = ["3 GeV", "5 GeV", "10 GeV", "20 GeV", "50 GeV", "200 GeV", "1 TeV",]
    var = "phi"
    special_hname = "deviation_%s_in_st%i_pt%i"
    def doit():
        deviation = []
        t = deviation_th_row[0:1] + deviation_th
        deviation.append(tuple(t))
        for i, st in enumerate(d.l1t_st_vec):
            if st == 4:  # skip ME+1/3
                continue
            t = deviation_th_row[i+1:i+2] + [tables.get(special_hname % (var, st, pt), 0) for pt in d.l1t_pt_vec]
            deviation.append(tuple(t))
        pretty_print(deviation, name="deviation")
    doit()

    # deviation theta
    deviation_th_row = [s.replace('&phi;', '&theta;') for s in deviation_th_row]
    var = "theta"
    doit()


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
    do_deviation_overlays()

    do_nstubs_overlays()

    do_deflection_overlays()

    # Serve
    serve_figures()
    serve_tables()
    return


# ______________________________________________________________________________
if __name__ == "__main__":
    import sys
    if len(sys.argv) != 2:
        raise Exception("Please provide 'imgdir'")

    # Call the main function
    main()


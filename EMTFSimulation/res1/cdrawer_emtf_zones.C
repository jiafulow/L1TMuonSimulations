{

gROOT->LoadMacro("tdrstyle.C");
setTDRStyle();

// _____________________________________________________________________________
// Positive endcap

if (1) {
    TFile *_file0 = TFile::Open("ntuple_p.root");
    ntupler->cd();
    TString outdir = "figures_emtf_zones/";

    if (0) {
        tree->Draw("CSCStubs_convZhit:CSCStubs_strip", "CSCStubs_convPhi!=-999 && CSCStubs_istation==2 && CSCStubs_iring==1", "");
        htemp->SetTitle("; strip number(ME+2/1); quadstrip number(ME+2/1)");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"quadstrip_vs_strip_ME+21.png");

        tree->Draw("CSCStubs_convZhit:CSCStubs_strip", "CSCStubs_convPhi!=-999 && CSCStubs_istation==2 && CSCStubs_iring==2", "");
        htemp->SetTitle("; strip number(ME+2/2); quadstrip number(ME+2/2)");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"quadstrip_vs_strip_ME+22.png");

        tree->Draw("CSCStubs_convZhit:CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==2 && CSCStubs_iring==1", "");
        htemp->SetTitle("; #phi(ME+2/1) [rad]; quadstrip number(ME+2/1)");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"quadstrip_vs_phi_ME+21.png");

        tree->Draw("CSCStubs_convZhit:CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==2 && CSCStubs_iring==2", "");
        htemp->SetTitle("; #phi(ME+2/2) [rad]; quadstrip number(ME+2/2)");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"quadstrip_vs_phi_ME+22.png");
    }

    if (1) {
        tree->Draw("CSCStubs_convZoneCont:CSCStubs_keywire", "CSCStubs_convPhi!=-999", "");
        htemp->SetTitle("; wire number(ME+m/n); zone number");
        htemp->GetYaxis()->SetRangeUser(-1, 5);
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"zone_vs_keywire_ME+00.png");

        tree->Draw("CSCStubs_convZoneCont:CSCStubs_globalTheta", "CSCStubs_convPhi!=-999", "");
        htemp->SetTitle("; #theta(ME+m/n) [rad]; zone number");
        htemp->GetYaxis()->SetRangeUser(-1, 5);
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"zone_vs_theta_ME+00.png");
    }
}

}

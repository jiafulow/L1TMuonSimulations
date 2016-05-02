{

// _____________________________________________________________________________
// Positive endcap

if (1) {
    TFile *_file0 = TFile::Open("ntuple_p.root");
    ntupler->cd();
    TString outdir = "figures_emtf_conversion/";

    // xyz_view
    if (0) {
        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_globalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1", "");
        htemp->SetTitle("; z(ME+1) [cm]; x(ME+1) [cm]; y(ME+1) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_sim_ME+1.png");

        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_convGlobalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1", "");
        htemp->SetTitle("; emu z(ME+1) [cm]; x(ME+1) [cm]; y(ME+1) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_emu_ME+1.png");

        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_globalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==2", "");
        htemp->SetTitle("; z(ME+2) [cm]; x(ME+2) [cm]; y(ME+2) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_sim_ME+2.png");

        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_convGlobalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==2", "");
        htemp->SetTitle("; emu z(ME+2) [cm]; x(ME+2) [cm]; y(ME+2) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_emu_ME+2.png");

        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_globalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==3", "");
        htemp->SetTitle("; z(ME+3) [cm]; x(ME+3) [cm]; y(ME+3) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_sim_ME+3.png");

        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_convGlobalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==3", "");
        htemp->SetTitle("; emu z(ME+3) [cm]; x(ME+3) [cm]; y(ME+3) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_emu_ME+3.png");

        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_globalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==4", "");
        htemp->SetTitle("; z(ME+4) [cm]; x(ME+4) [cm]; y(ME+4) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_sim_ME+4.png");

        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_convGlobalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==4", "");
        htemp->SetTitle("; emu z(ME+4) [cm]; x(ME+4) [cm]; y(ME+4) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_emu_ME+4.png");
    }

    // theta residual vs phi
    if (0) {
        tree->Draw("CSCStubs_globalTheta-CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1 && CSCStubs_iring==4", "");
        htemp->SetTitle("; #phi(ME+1/1a) [rad]; #theta(ME+1/1a) - emu #theta(ME+1/1a) [rad]");
        gPad->Modified();
        gPad->Print(outdir+"theta_residual_vs_phi_ME+11a.png");

        tree->Draw("CSCStubs_globalTheta-CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1 && CSCStubs_iring==1", "");
        htemp->SetTitle("; #phi(ME+1/1b) [rad]; #theta(ME+1/1b) - emu #theta(ME+1/1b) [rad]");
        gPad->Modified();
        gPad->Print(outdir+"theta_residual_vs_phi_ME+11b.png");

        tree->Draw("CSCStubs_globalTheta-CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1 && CSCStubs_iring==2", "");
        htemp->SetTitle("; #phi(ME+1/2) [rad]; #theta(ME+1/2) - emu #theta(ME+1/2) [rad]");
        gPad->Modified();
        gPad->Print(outdir+"theta_residual_vs_phi_ME+12.png");

        tree->Draw("CSCStubs_globalTheta-CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1 && CSCStubs_iring==3", "");
        htemp->SetTitle("; #phi(ME+1/3) [rad]; #theta(ME+1/3) - emu #theta(ME+1/3) [rad]");
        gPad->Modified();
        gPad->Print(outdir+"theta_residual_vs_phi_ME+13.png");

        tree->Draw("CSCStubs_globalTheta-CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==2 && CSCStubs_iring==1", "");
        htemp->SetTitle("; #phi(ME+2/1) [rad]; #theta(ME+2/1) - emu #theta(ME+2/1) [rad]");
        gPad->Modified();
        gPad->Print(outdir+"theta_residual_vs_phi_ME+21.png");

        tree->Draw("CSCStubs_globalTheta-CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==2 && CSCStubs_iring==2", "");
        htemp->SetTitle("; #phi(ME+2/2) [rad]; #theta(ME+2/2) - emu #theta(ME+2/2) [rad]");
        gPad->Modified();
        gPad->Print(outdir+"theta_residual_vs_phi_ME+22.png");

        tree->Draw("CSCStubs_globalTheta-CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==3 && CSCStubs_iring==1", "");
        htemp->SetTitle("; #phi(ME+3/1) [rad]; #theta(ME+3/1) - emu #theta(ME+3/1) [rad]");
        gPad->Modified();
        gPad->Print(outdir+"theta_residual_vs_phi_ME+31.png");

        tree->Draw("CSCStubs_globalTheta-CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==3 && CSCStubs_iring==2", "");
        htemp->SetTitle("; #phi(ME+3/2) [rad]; #theta(ME+3/2) - emu #theta(ME+3/2) [rad]");
        gPad->Modified();
        gPad->Print(outdir+"theta_residual_vs_phi_ME+32.png");

        tree->Draw("CSCStubs_globalTheta-CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==4 && CSCStubs_iring==1", "");
        htemp->SetTitle("; #phi(ME+4/1) [rad]; #theta(ME+4/1) - emu #theta(ME+4/1) [rad]");
        gPad->Modified();
        gPad->Print(outdir+"theta_residual_vs_phi_ME+41.png");

        tree->Draw("CSCStubs_globalTheta-CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==4 && CSCStubs_iring==2", "");
        htemp->SetTitle("; #phi(ME+4/2) [rad]; #theta(ME+4/2) - emu #theta(ME+4/2) [rad]");
        gPad->Modified();
        gPad->Print(outdir+"theta_residual_vs_phi_ME+42.png");
    }

    // theta vs phi
    if (0) {
        int keywire = 10;
        TString str = Form("%d", keywire);

        tree->Draw("CSCStubs_globalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1 && CSCStubs_iring==4 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; #theta(ME+1/1a, wire "+str+") [rad]; #phi(ME+1/1a, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_sim_ME+11a_wire"+str+".png");

        tree->Draw("CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1 && CSCStubs_iring==4 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; emu #theta(ME+1/1a, wire "+str+") [rad]; #phi(ME+1/1a, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_emu_ME+11a_wire"+str+".png");

        keywire = 20;
        str = Form("%d", keywire);

        tree->Draw("CSCStubs_globalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1 && CSCStubs_iring==1 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; #theta(ME+1/1b, wire "+str+") [rad]; #phi(ME+1/1b, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_sim_ME+11b_wire"+str+".png");

        tree->Draw("CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1 && CSCStubs_iring==1 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; emu #theta(ME+1/1b, wire "+str+") [rad]; #phi(ME+1/1b, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_emu_ME+11b_wire"+str+".png");

        tree->Draw("CSCStubs_globalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1 && CSCStubs_iring==2 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; #theta(ME+1/2, wire "+str+") [rad]; #phi(ME+1/2, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_sim_ME+12_wire"+str+".png");

        tree->Draw("CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1 && CSCStubs_iring==2 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; emu #theta(ME+1/2, wire "+str+") [rad]; #phi(ME+1/2, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_emu_ME+12_wire"+str+".png");

        tree->Draw("CSCStubs_globalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1 && CSCStubs_iring==3 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; #theta(ME+1/3, wire "+str+") [rad]; #phi(ME+1/3, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        TGraph * graph = (TGraph *) Graph->Clone("graph");
        TH2F * h2 = new TH2F("h2", "; #theta(ME+1/3, wire "+str+") [rad]; #phi(ME+1/3, wire "+str+") [rad]", 40, -3.6, 3.6, 40, 0.71, 0.73);
        h2->Draw(); graph->Draw("p");
        gPad->Print(outdir+"theta_vs_phi_sim_ME+13_wire"+str+".png");

        tree->Draw("CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1 && CSCStubs_iring==3 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; emu #theta(ME+1/3, wire "+str+") [rad]; #phi(ME+1/3, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        TGraph * graph_1 = (TGraph *) Graph->Clone("graph");
        TH2F * h2_1 = new TH2F("h2_1", "; emu #theta(ME+1/3, wire "+str+") [rad]; #phi(ME+1/3, wire "+str+") [rad]", 40, -3.6, 3.6, 40, 0.71, 0.73);
        h2_1->Draw(); graph_1->Draw("p");
        gPad->Print(outdir+"theta_vs_phi_emu_ME+13_wire"+str+".png");

        tree->Draw("CSCStubs_globalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==2 && CSCStubs_iring==1 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; #theta(ME+2/1, wire "+str+") [rad]; #phi(ME+2/1, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_sim_ME+21_wire"+str+".png");

        tree->Draw("CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==2 && CSCStubs_iring==1 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; emu #theta(ME+2/1, wire "+str+") [rad]; #phi(ME+2/1, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_emu_ME+21_wire"+str+".png");

        tree->Draw("CSCStubs_globalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==2 && CSCStubs_iring==2 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; #theta(ME+2/2, wire "+str+") [rad]; #phi(ME+2/2, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_sim_ME+22_wire"+str+".png");

        tree->Draw("CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==2 && CSCStubs_iring==2 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; emu #theta(ME+2/2, wire "+str+") [rad]; #phi(ME+2/2, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_emu_ME+22_wire"+str+".png");

        tree->Draw("CSCStubs_globalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==3 && CSCStubs_iring==1 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; #theta(ME+3/1, wire "+str+") [rad]; #phi(ME+3/1, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_sim_ME+31_wire"+str+".png");

        tree->Draw("CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==3 && CSCStubs_iring==1 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; emu #theta(ME+3/1, wire "+str+") [rad]; #phi(ME+3/1, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_emu_ME+31_wire"+str+".png");

        tree->Draw("CSCStubs_globalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==3 && CSCStubs_iring==2 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; #theta(ME+3/2, wire "+str+") [rad]; #phi(ME+3/2, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_sim_ME+32_wire"+str+".png");

        tree->Draw("CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==3 && CSCStubs_iring==2 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; emu #theta(ME+3/2, wire "+str+") [rad]; #phi(ME+3/2, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_emu_ME+32_wire"+str+".png");

        tree->Draw("CSCStubs_globalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==4 && CSCStubs_iring==1 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; #theta(ME+4/1, wire "+str+") [rad]; #phi(ME+4/1, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_sim_ME+41_wire"+str+".png");

        tree->Draw("CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==4 && CSCStubs_iring==1 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; emu #theta(ME+4/1, wire "+str+") [rad]; #phi(ME+4/1, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_emu_ME+41_wire"+str+".png");

        tree->Draw("CSCStubs_globalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==4 && CSCStubs_iring==2 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; #theta(ME+4/2, wire "+str+") [rad]; #phi(ME+4/2, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_sim_ME+42_wire"+str+".png");

        tree->Draw("CSCStubs_convGlobalTheta : CSCStubs_globalPhi", "CSCStubs_convPhi!=-999 && CSCStubs_istation==4 && CSCStubs_iring==2 && CSCStubs_keywire=="+str, "");
        htemp->SetTitle("; emu #theta(ME+4/2, wire "+str+") [rad]; #phi(ME+4/2, wire "+str+") [rad]");
        Graph->SetMarkerStyle(6);
        gPad->Modified();
        gPad->Print(outdir+"theta_vs_phi_emu_ME+42_wire"+str+".png");
    }
}

// _____________________________________________________________________________
// Negative endcap

if (1) {
    TFile *_file0 = TFile::Open("ntuple_n.root");
    ntupler->cd();
    TString outdir = "figures_emtf_conversion/";

    // xyz_view
    if (1) {
        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_globalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1", "");
        htemp->SetTitle("; z(ME-1) [cm]; x(ME-1) [cm]; y(ME-1) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_sim_ME-1.png");

        //tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_convGlobalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1", "");
        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(-CSCStubs_convGlobalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==1", "");
        htemp->SetTitle("; emu z(ME-1) [cm]; x(ME-1) [cm]; y(ME-1) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_emu_ME-1.png");

        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_globalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==2", "");
        htemp->SetTitle("; z(ME-2) [cm]; x(ME-2) [cm]; y(ME-2) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_sim_ME-2.png");

        //tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_convGlobalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==2", "");
        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(-CSCStubs_convGlobalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==2", "");
        htemp->SetTitle("; emu z(ME-2) [cm]; x(ME-2) [cm]; y(ME-2) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_emu_ME-2.png");

        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_globalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==3", "");
        htemp->SetTitle("; z(ME-3) [cm]; x(ME-3) [cm]; y(ME-3) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_sim_ME-3.png");

        //tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_convGlobalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==3", "");
        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(-CSCStubs_convGlobalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==3", "");
        htemp->SetTitle("; emu z(ME-3) [cm]; x(ME-3) [cm]; y(ME-3) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_emu_ME-3.png");

        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_globalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==4", "");
        htemp->SetTitle("; z(ME-4) [cm]; x(ME-4) [cm]; y(ME-4) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_sim_ME-4.png");

        //tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(CSCStubs_convGlobalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==4", "");
        tree->Draw("CSCStubs_globalY:CSCStubs_globalX:CSCStubs_globalRho/tan(-CSCStubs_convGlobalTheta)", "CSCStubs_convPhi!=-999 && CSCStubs_istation==4", "");
        htemp->SetTitle("; emu z(ME-4) [cm]; x(ME-4) [cm]; y(ME-4) [cm]");
        gPad->Modified();
        gPad->Print(outdir+"xyz_view_emu_ME-4.png");
    }
}

}

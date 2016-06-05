{

gROOT->LoadMacro("tdrstyle.C");
setTDRStyle();


if (1) {

    TString outdir = "figures_emtf_finepatterns/";

    if (1) {
        TString fname, imgname;
        TFile * _file0;
        TH2F* h2;
        TProfile* pf;

        fname = "bank_ss0p5.root";
        _file0 = TFile::Open(fname);
        h2 = new TH2F("h2", "; p_{T} [GeV]; (#Delta p_{T})/p_{T}", 100, 0, 30., 500, 0, 10);
        patternBank->Project("h2", "phi_sigma/phi_mean:phi_mean", "popularity>=5");
        h2->SetMarkerStyle(6);
        h2->Draw(); h2->GetYaxis()->SetRangeUser(0, 0.65);
        pf = h2->ProfileX("h2_pfx");
        pf->SetMarkerStyle(20); pf->SetMarkerSize(0.7); pf->SetMarkerColor(6); pf->SetLineWidth(2); pf->SetLineColor(6);
        pf->Draw("same");
        imgname = fname.ReplaceAll(".root", "_pt.png");
        gPad->Print(outdir+imgname);

        fname = "bank_ss1.root";
        _file0 = TFile::Open(fname);
        h2 = new TH2F("h2", "; p_{T} [GeV]; (#Delta p_{T})/p_{T}", 100, 0, 30., 500, 0, 10);
        patternBank->Project("h2", "phi_sigma/phi_mean:phi_mean", "popularity>=5");
        h2->SetMarkerStyle(6);
        h2->Draw(); h2->GetYaxis()->SetRangeUser(0, 0.65);
        pf = h2->ProfileX("h2_pfx");
        pf->SetMarkerStyle(20); pf->SetMarkerSize(0.7); pf->SetMarkerColor(6); pf->SetLineWidth(2); pf->SetLineColor(6);
        pf->Draw("same");
        imgname = fname.ReplaceAll(".root", "_pt.png");
        gPad->Print(outdir+imgname);

        fname = "bank_ss2.root";
        _file0 = TFile::Open(fname);
        h2 = new TH2F("h2", "; p_{T} [GeV]; (#Delta p_{T})/p_{T}", 100, 0, 30., 500, 0, 10);
        patternBank->Project("h2", "phi_sigma/phi_mean:phi_mean", "popularity>=5");
        h2->SetMarkerStyle(6);
        h2->Draw(); h2->GetYaxis()->SetRangeUser(0, 0.65);
        pf = h2->ProfileX("h2_pfx");
        pf->SetMarkerStyle(20); pf->SetMarkerSize(0.7); pf->SetMarkerColor(6); pf->SetLineWidth(2); pf->SetLineColor(6);
        pf->Draw("same");
        imgname = fname.ReplaceAll(".root", "_pt.png");
        gPad->Print(outdir+imgname);

        fname = "bank_ss4.root";
        _file0 = TFile::Open(fname);
        h2 = new TH2F("h2", "; p_{T} [GeV]; (#Delta p_{T})/p_{T}", 100, 0, 30., 500, 0, 10);
        patternBank->Project("h2", "phi_sigma/phi_mean:phi_mean", "popularity>=5");
        h2->SetMarkerStyle(6);
        h2->Draw(); h2->GetYaxis()->SetRangeUser(0, 0.65);
        pf = h2->ProfileX("h2_pfx");
        pf->SetMarkerStyle(20); pf->SetMarkerSize(0.7); pf->SetMarkerColor(6); pf->SetLineWidth(2); pf->SetLineColor(6);
        pf->Draw("same");
        imgname = fname.ReplaceAll(".root", "_pt.png");
        gPad->Print(outdir+imgname);

        fname = "bank_ss8.root";
        _file0 = TFile::Open(fname);
        h2 = new TH2F("h2", "; p_{T} [GeV]; (#Delta p_{T})/p_{T}", 100, 0, 30., 500, 0, 10);
        patternBank->Project("h2", "phi_sigma/phi_mean:phi_mean", "popularity>=5");
        h2->SetMarkerStyle(6);
        h2->Draw(); h2->GetYaxis()->SetRangeUser(0, 0.65);
        pf = h2->ProfileX("h2_pfx");
        pf->SetMarkerStyle(20); pf->SetMarkerSize(0.7); pf->SetMarkerColor(6); pf->SetLineWidth(2); pf->SetLineColor(6);
        pf->Draw("same");
        imgname = fname.ReplaceAll(".root", "_pt.png");
        gPad->Print(outdir+imgname);
    }

    if (1) {
        TString fname, imgname;
        TFile * _file0;
        TH2F* h2;
        TProfile* pf;

        fname = "bank_ss0p5.root";
        _file0 = TFile::Open(fname);
        h2 = new TH2F("h2", "; q/p_{T} [1/GeV]; <q>", 100, -0.3, 0.3, 500, -2, 2);
        patternBank->Project("h2", "z0_mean:invPt_mean", "popularity>=5");
        h2->SetMarkerStyle(6);
        h2->Draw(); h2->GetYaxis()->SetRangeUser(-1.1, 1.1);
        pf = h2->ProfileX("h2_pfx");
        pf->SetMarkerStyle(20); pf->SetMarkerSize(0.7); pf->SetMarkerColor(6); pf->SetLineWidth(2); pf->SetLineColor(6);
        pf->Draw("same");
        imgname = fname.ReplaceAll(".root", "_charge.png");
        gPad->Print(outdir+imgname);

        fname = "bank_ss1.root";
        _file0 = TFile::Open(fname);
        h2 = new TH2F("h2", "; q/p_{T} [1/GeV]; <q>", 100, -0.3, 0.3, 500, -2, 2);
        patternBank->Project("h2", "z0_mean:invPt_mean", "popularity>=5");
        h2->SetMarkerStyle(6);
        h2->Draw(); h2->GetYaxis()->SetRangeUser(-1.1, 1.1);
        pf = h2->ProfileX("h2_pfx");
        pf->SetMarkerStyle(20); pf->SetMarkerSize(0.7); pf->SetMarkerColor(6); pf->SetLineWidth(2); pf->SetLineColor(6);
        pf->Draw("same");
        imgname = fname.ReplaceAll(".root", "_charge.png");
        gPad->Print(outdir+imgname);

        fname = "bank_ss2.root";
        _file0 = TFile::Open(fname);
        h2 = new TH2F("h2", "; q/p_{T} [1/GeV]; <q>", 100, -0.3, 0.3, 500, -2, 2);
        patternBank->Project("h2", "z0_mean:invPt_mean", "popularity>=5");
        h2->SetMarkerStyle(6);
        h2->Draw(); h2->GetYaxis()->SetRangeUser(-1.1, 1.1);
        pf = h2->ProfileX("h2_pfx");
        pf->SetMarkerStyle(20); pf->SetMarkerSize(0.7); pf->SetMarkerColor(6); pf->SetLineWidth(2); pf->SetLineColor(6);
        pf->Draw("same");
        imgname = fname.ReplaceAll(".root", "_charge.png");
        gPad->Print(outdir+imgname);

        fname = "bank_ss4.root";
        _file0 = TFile::Open(fname);
        h2 = new TH2F("h2", "; q/p_{T} [1/GeV]; <q>", 100, -0.3, 0.3, 500, -2, 2);
        patternBank->Project("h2", "z0_mean:invPt_mean", "popularity>=5");
        h2->SetMarkerStyle(6);
        h2->Draw(); h2->GetYaxis()->SetRangeUser(-1.1, 1.1);
        pf = h2->ProfileX("h2_pfx");
        pf->SetMarkerStyle(20); pf->SetMarkerSize(0.7); pf->SetMarkerColor(6); pf->SetLineWidth(2); pf->SetLineColor(6);
        pf->Draw("same");
        imgname = fname.ReplaceAll(".root", "_charge.png");
        gPad->Print(outdir+imgname);

        fname = "bank_ss8.root";
        _file0 = TFile::Open(fname);
        h2 = new TH2F("h2", "; q/p_{T} [1/GeV]; <q>", 100, -0.3, 0.3, 500, -2, 2);
        patternBank->Project("h2", "z0_mean:invPt_mean", "popularity>=5");
        h2->SetMarkerStyle(6);
        h2->Draw(); h2->GetYaxis()->SetRangeUser(-1.1, 1.1);
        pf = h2->ProfileX("h2_pfx");
        pf->SetMarkerStyle(20); pf->SetMarkerSize(0.7); pf->SetMarkerColor(6); pf->SetLineWidth(2); pf->SetLineColor(6);
        pf->Draw("same");
        imgname = fname.ReplaceAll(".root", "_charge.png");
        gPad->Print(outdir+imgname);
    }


}


}


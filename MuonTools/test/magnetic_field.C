{

gROOT->LoadMacro("tdrstyle.C");
setTDRStyle();
gStyle->SetPadRightMargin(0.1);
//gStyle->SetPalette(kViridis);
gStyle->SetNumberContours(100);

TCanvas * c1 = new TCanvas("c1", "c1", 1200, 800);
c1->Draw();
TFile * tfile = TFile::Open("magnetic_field.root");


// _____________________________________________________________________________
// Color Brewer 9-class RdBu
Double_t stops[7] = { 0./6, 1./6, 2./6, 3./6, 4./6, 5./6, 6./6};
Double_t red  [7] = { 103/255., 209/255., 255/255., 253/255., 244/255., 214/255., 178/255.};
Double_t green[7] = { 169/255., 229/255., 255/255., 219/255., 165/255.,  96/255.,  24/255.};
Double_t blue [7] = { 207/255., 240/255., 255/255., 199/255., 130/255.,  77/255.,  43/255.};
Int_t Idx = TColor::CreateGradientColorTable(7, stops, red, green, blue, 255, 1);

TH2F * h2_bz = (TH2F *) tfile->Get("bz");
h2_bz->GetZaxis()->SetRangeUser(-2, 4);
h2_bz->GetZaxis()->SetTitleOffset(-0.3);
h2_bz->GetZaxis()->SetTitle("B_{z} [T]    ");
h2_bz->Draw("COLZ");
gPad->Print("magnetic_field_Bz.png");


// _____________________________________________________________________________
// Color Brewer 9-class RdBu
Double_t stops2[4] = { 0./3, 1./3, 2./3, 3./3};
Double_t red2  [4] = { 255/255., 253/255., 244/255., 214/255.};
Double_t green2[4] = { 255/255., 219/255., 165/255.,  96/255.};
Double_t blue2 [4] = { 255/255., 199/255., 130/255.,  77/255.};
Int_t Idx2 = TColor::CreateGradientColorTable(4, stops2, red2, green2, blue2, 255, 1);

TH2F * h2_br = (TH2F *) tfile->Get("br");
h2_br->GetZaxis()->SetRangeUser(0, 3);
h2_br->GetZaxis()->SetTitleOffset(-0.3);
h2_br->GetZaxis()->SetTitle("B_{r} [T]    ");
h2_br->Draw("COLZ");
gPad->Print("magnetic_field_Br.png");

}

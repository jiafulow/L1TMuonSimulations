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
// TPolyLines
Double_t x1_, x2_, y1_, y2_;
Double_t xx[5], yy[5];

x1_ = 0.; x2_ = 1318.3/2; y1_ = 946.753/2; y2_ = y1_-58.6855;
xx[0] = x1_; xx[1] = x2_; xx[2] = x2_; xx[3] = x1_; xx[4] = x1_; yy[0] = y1_; yy[1] = y1_; yy[2] = y2_; yy[3] = y2_; yy[4] = y1_;
TPolyLine * mb1 = new TPolyLine(5, xx, yy);
x1_ = 0.; x2_ = 1318.05/2; y1_ = 1105.98/2; y2_ = y1_-58.3355;
xx[0] = x1_; xx[1] = x2_; xx[2] = x2_; xx[3] = x1_; xx[4] = x1_; yy[0] = y1_; yy[1] = y1_; yy[2] = y2_; yy[3] = y2_; yy[4] = y1_;
TPolyLine * mb2 = new TPolyLine(5, xx, yy);
x1_ = 0.; x2_ = 1318.1/2; y1_ = 1319.28/2; y2_ = y1_-57.9266;
xx[0] = x1_; xx[1] = x2_; xx[2] = x2_; xx[3] = x1_; xx[4] = x1_; yy[0] = y1_; yy[1] = y1_; yy[2] = y2_; yy[3] = y2_; yy[4] = y1_;
TPolyLine * mb3 = new TPolyLine(5, xx, yy);
x1_ = 0.; x2_ = 1318.17/2; y1_ = 1605.12/2; y2_ = y1_-98.886;
xx[0] = x1_; xx[1] = x2_; xx[2] = x2_; xx[3] = x1_; xx[4] = x1_; yy[0] = y1_; yy[1] = y1_; yy[2] = y2_; yy[3] = y2_; yy[4] = y1_;
TPolyLine * mb4 = new TPolyLine(5, xx, yy);

x1_ = 602.332-22.358; x2_ = 602.332+22.358; y1_ = 100.346; y2_ = 264.388;
xx[0] = x1_; xx[1] = x2_; xx[2] = x2_; xx[3] = x1_; xx[4] = x1_; yy[0] = y1_; yy[1] = y1_; yy[2] = y2_; yy[3] = y2_; yy[4] = y1_;
TPolyLine * me11 = new TPolyLine(5, xx, yy);
x1_ = 698.693-23.0366; x2_ = 698.693+23.0367; y1_ = 274.909; y2_ = 687.485;
xx[0] = x1_; xx[1] = x2_; xx[2] = x2_; xx[3] = x1_; xx[4] = x1_; yy[0] = y1_; yy[1] = y1_; yy[2] = y2_; yy[3] = y2_; yy[4] = y1_;
TPolyLine * me12 = new TPolyLine(5, xx, yy);
x1_ = 828.392-21.8752; x2_ = 828.392+21.8752; y1_ = 140.349; y2_ = 699.773;
xx[0] = x1_; xx[1] = x2_; xx[2] = x2_; xx[3] = x1_; xx[4] = x1_; yy[0] = y1_; yy[1] = y1_; yy[2] = y2_; yy[3] = y2_; yy[4] = y1_;
TPolyLine * me2 = new TPolyLine(5, xx, yy);
x1_ = 935.437-22.1453; x2_ = 935.437+22.1453; y1_ = 160.314; y2_ = 699.813;
xx[0] = x1_; xx[1] = x2_; xx[2] = x2_; xx[3] = x1_; xx[4] = x1_; yy[0] = y1_; yy[1] = y1_; yy[2] = y2_; yy[3] = y2_; yy[4] = y1_;
TPolyLine * me3 = new TPolyLine(5, xx, yy);
x1_ = 1025.17-21.7811; x2_ = 1025.17+21.7810; y1_ = 179.232; y2_ = 699.72;
xx[0] = x1_; xx[1] = x2_; xx[2] = x2_; xx[3] = x1_; xx[4] = x1_; yy[0] = y1_; yy[1] = y1_; yy[2] = y2_; yy[3] = y2_; yy[4] = y1_;
TPolyLine * me4 = new TPolyLine(5, xx, yy);


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
mb1->Draw(); mb2->Draw(); mb3->Draw(); mb4->Draw(); me11->Draw(); me12->Draw(); me2->Draw(); me3->Draw(); me4->Draw();
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
mb1->Draw(); mb2->Draw(); mb3->Draw(); mb4->Draw(); me11->Draw(); me12->Draw(); me2->Draw(); me3->Draw(); me4->Draw();
gPad->Print("magnetic_field_Br.png");


tfile->Close();
}

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
// TProfile
int nbinsx = 60;
TProfile * pf11 = new TProfile("pf11", "; r [cm]; B_{z} [T]", nbinsx, 100, 700, -10, 10, "s");
TProfile * pf12 = new TProfile("pf12", "; r [cm]; B_{z} [T]", nbinsx, 100, 700, -10, 10, "s");
TProfile * pf2  = new TProfile("pf2" , "; r [cm]; B_{z} [T]", nbinsx, 100, 700, -10, 10, "s");
TProfile * pf3  = new TProfile("pf3" , "; r [cm]; B_{z} [T]", nbinsx, 100, 700, -10, 10, "s");
TProfile * pf4  = new TProfile("pf4" , "; r [cm]; B_{z} [T]", nbinsx, 100, 700, -10, 10, "s");

// _____________________________________________________________________________
// Project
tree->Project("pf11", "Bz:r", "602.332-22.3580 < z && z < 602.332+22.3580", "goff");
tree->Project("pf12", "Bz:r", "698.693-23.0366 < z && z < 698.693+23.0367", "goff");
tree->Project("pf2" , "Bz:r", "828.392-21.8752 < z && z < 828.392+21.8753", "goff");
tree->Project("pf3" , "Bz:r", "935.437-22.1453 < z && z < 935.437+22.1453", "goff");
tree->Project("pf4" , "Bz:r", "1025.17-21.7811 < z && z < 1025.17+21.7810", "goff");

pf11->Draw("e");
gPad->Print("magnetic_field_vs_r_pf11.png");
pf12->Draw("e");
gPad->Print("magnetic_field_vs_r_pf12.png");
pf2->Draw("e");
gPad->Print("magnetic_field_vs_r_pf2.png");
pf3->Draw("e");
gPad->Print("magnetic_field_vs_r_pf3.png");
pf4->Draw("e");
gPad->Print("magnetic_field_vs_r_pf4.png");

// _____________________________________________________________________________
// Print

std::vector<TProfile *> vec;
vec.push_back(pf11);
vec.push_back(pf12);
vec.push_back(pf2);
vec.push_back(pf3);
vec.push_back(pf4);

for (unsigned int j=0; j<vec.size(); j++) {
    TProfile * pf = vec.at(j);
    std::cout << "# " << pf->GetName() << std::endl;
    std::cout << "[\n    ";
    for (int i=0; i<nbinsx; i++) {
        std::cout << Form("%+.3f", pf->GetBinContent(i+1));
        if (i == nbinsx-1)  break;
        std::cout << ", ";
        if (i%10 == 9)  std::cout << "\n    ";
    }
    std::cout << "\n]";
    std::cout << std::endl;
}


tfile->Close();
}

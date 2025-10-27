#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TStyle.h"

using namespace std;

void plot_@VARNAME@() {
    TString varName = "@VARNAME@";

    double var_min = 1e10;
    double var_max = -1e10;
    
    TFile *f_data = nullptr;
    TFile *f_mc   = nullptr;

    f_data = TFile::Open("../validation/sw_root/X/DATA_X_cut2_sw.root");
    f_mc   = TFile::Open("../selection/root_files/X/MC_X_cut1.root");

    TTree *tree_data = (TTree*)f_data->Get("tree");
    TTree *tree_mc   = (TTree*)f_mc->Get("tree");

    Float_t var_value;
    tree_data->SetBranchAddress(varName, &var_value);
    double min_data = tree_data->GetMinimum(varName);
    double max_data = tree_data->GetMaximum(varName);

    tree_mc->SetBranchAddress(varName, &var_value);
    double min_mc   = tree_mc->GetMinimum(varName);
    double max_mc   = tree_mc->GetMaximum(varName);

    var_min = std::min({min_data, min_mc});
    var_max = std::max({max_data, max_mc});

    int nBins = 100;
    TH1F *h_data = new TH1F("h_data", varName, nBins, var_min, var_max);
    TH1F *h_mc   = new TH1F("h_mc",   varName, nBins, var_min, var_max);

    tree_data->Draw(varName + ">>h_data", "sWeight", "goff");
    tree_mc->Draw(varName + ">>h_mc", "", "goff");

    if (h_data->Integral() > 0) h_data->Scale(1.0 / h_data->Integral());
    if (h_mc->Integral() > 0)   h_mc->Scale(1.0 / h_mc->Integral());

    int brightAzure = TColor::GetColor(51, 153, 255);
    h_data->SetLineColor(brightAzure);
    h_data->SetLineWidth(2);
    h_data->SetFillColor(brightAzure);
    h_data->SetFillStyle(3345);

    h_mc->SetLineColor(kOrange + 7);
    h_mc->SetLineWidth(2);
    h_mc->SetFillColorAlpha(kOrange + 7, 0.4);
    h_mc->SetFillStyle(1001);

    float max_val = std::max({h_data->GetMaximum(), h_mc->GetMaximum()});
    h_data->SetMaximum(max_val * 1.2);

    TCanvas *c1 = new TCanvas("c1", "Comparison", 800, 600);
    gStyle->SetOptStat(0);
    h_data->SetTitle("Comparison of " + varName + "; " + varName + "; Normalized Entries");
    h_data->Draw("hist");
    h_mc->Draw("hist same");

    TLegend *leg = new TLegend(0.65, 0.70, 0.88, 0.88);
    leg->AddEntry(h_data, "X(3872) splot", "lf");
    leg->AddEntry(h_mc,   "X(3872) MC",   "lf");
    leg->Draw();

    gSystem->Exec("mkdir -p pdf/X/");
    c1->SaveAs("pdf/X/"+varName + "_dist.pdf");
}

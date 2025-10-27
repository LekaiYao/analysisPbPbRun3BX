#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TColor.h"
#include "TSystem.h"

using namespace std;

void plot_@VARNAME@() {
    TString varName = "@VARNAME@";
    TString massVar = "Bmass";

    //Bs
    //double sig_low  = 5.30;
    //double sig_high = 5.42;
    //double sb1_low  = 5.00;
    //double sb1_high = 5.20;
    //double sb2_low  = 5.50;
    //double sb2_high = 5.70;

    //X
    double sig_low  = 3.85;
    double sig_high = 3.90;
    double sb1_low  = 3.75;
    double sb1_high = 3.80;
    double sb2_low  = 3.95;
    double sb2_high = 4.00;

    //TFile *f_data = TFile::Open("../selection/root_files/Bs/DATA_Bs_cut1.root");
    //TFile *f_mc   = TFile::Open("../selection/root_files/Bs/MC_Bs_cut1.root");
    //TFile *f_data = TFile::Open("../selection/root_files/Bu/DATA_Bu_cut1.root");
    //TFile *f_mc   = TFile::Open("../selection/root_files/Bu/MC_Bu_cut1.root");
    TFile *f_data = TFile::Open("../selection/root_files/X/DATA_X_cut2.root");
    TFile *f_mc   = TFile::Open("../selection/root_files/X/MC_X_cut1.root");
    TTree *tree_data = (TTree*)f_data->Get("tree");
    TTree *tree_mc   = (TTree*)f_mc->Get("tree");

    double var_min = std::min(tree_data->GetMinimum(varName), tree_mc->GetMinimum(varName));
    double var_max = std::max(tree_data->GetMaximum(varName), tree_mc->GetMaximum(varName));
    int nBins = 100;

    TH1F *h_sig = new TH1F("h_sig", "Signal region", nBins, var_min, var_max);
    TH1F *h_sb1 = new TH1F("h_sb1", "Sideband1", nBins, var_min, var_max);
    TH1F *h_sb2 = new TH1F("h_sb2", "Sideband2", nBins, var_min, var_max);
    TH1F *h_bkg = new TH1F("h_bkg", "Combined sidebands", nBins, var_min, var_max);
    TH1F *h_sub = new TH1F("h_sub", "Sideband-subtracted", nBins, var_min, var_max);
    TH1F *h_mc  = new TH1F("h_mc",  "MC", nBins, var_min, var_max);

    tree_data->Draw(varName + ">>h_sig", Form("%s>%.3f && %s<%.3f", massVar.Data(), sig_low, massVar.Data(), sig_high), "goff");
    tree_data->Draw(varName + ">>h_sb1", Form("%s>%.3f && %s<%.3f", massVar.Data(), sb1_low, massVar.Data(), sb1_high), "goff");
    tree_data->Draw(varName + ">>h_sb2", Form("%s>%.3f && %s<%.3f", massVar.Data(), sb2_low, massVar.Data(), sb2_high), "goff");
    tree_mc->Draw(varName + ">>h_mc", "", "goff");

    double sig_width = sig_high - sig_low;
    double sb_width = (sb1_high - sb1_low) + (sb2_high - sb2_low);
    double scale = sig_width / sb_width;

    h_bkg->Add(h_sb1);
    h_bkg->Add(h_sb2);
    h_bkg->Scale(scale);

    h_sub->Add(h_sig);
    h_sub->Add(h_bkg, -1);

    if (h_sub->Integral() > 0) h_sub->Scale(1.0 / h_sub->Integral());
    if (h_mc->Integral() > 0)  h_mc->Scale(1.0 / h_mc->Integral());

    int azure = TColor::GetColor(51, 153, 255);
    h_sub->SetLineColor(azure);
    h_sub->SetLineWidth(2);
    h_sub->SetFillColor(azure);
    h_sub->SetFillStyle(3345);

    h_mc->SetLineColor(kOrange + 7);
    h_mc->SetLineWidth(2);
    h_mc->SetFillColorAlpha(kOrange + 7, 0.4);
    h_mc->SetFillStyle(1001);

    float max_val = std::max(h_sub->GetMaximum(), h_mc->GetMaximum());
    h_sub->SetMaximum(max_val * 1.2);

    gStyle->SetOptStat(0);
    TCanvas *c1 = new TCanvas("c1", "Sideband Subtraction", 800, 600);
    h_sub->SetTitle("Sideband-subtracted " + varName + "; " + varName + "; Normalized entries");
    h_sub->Draw("hist");
    h_mc->Draw("hist same");

    TLegend *leg = new TLegend(0.65, 0.70, 0.88, 0.88);
    leg->AddEntry(h_sub, "X(3872) SBS", "lf");
    leg->AddEntry(h_mc,  "X(3872) MC", "lf");
    leg->Draw();

    gSystem->Exec("mkdir -p SBS_output/X/");
    c1->SaveAs("SBS_output/X/" + varName + "_dist.pdf");
}

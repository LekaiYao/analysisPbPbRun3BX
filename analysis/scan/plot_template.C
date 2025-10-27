#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TStyle.h"

using namespace std;

void plot_@VARNAME@(int mode = 0) {
    // --------- User-defined variable name ---------
    TString varName = "@VARNAME@";  // Variable to inspect

    // --------- Automatically determined variable range ---------
    double var_min = 1e10;
    double var_max = -1e10;
    
    TFile *f_data = nullptr;
    TFile *f_mc   = nullptr;
    TFile *f_mc2  = nullptr;
    TTree *tree_mc2 = nullptr;
    TH1F *h_mc2 = nullptr;

    // Open ROOT files
    // 0(X),1(Bu),2(Bd),3(Bs)
    if (mode == 0) {
        f_data = TFile::Open("../selection/root_files/X/sideband_X.root");
        f_mc   = TFile::Open("../selection/root_files/X/MC_PSI2S.root");
        f_mc2  = TFile::Open("../selection/root_files/X/MC_X.root");
    }
    else if(mode==1){
        f_data = TFile::Open("../selection/root_files/Bu/sideband_Bu.root");
        f_mc   = TFile::Open("../selection/root_files/Bu/MC_Bu.root");
    }
    else if(mode==2){
        f_data = TFile::Open("../selection/root_files/Bd/sideband_Bd.root");
        f_mc   = TFile::Open("../selection/root_files/Bd/MC_Bd.root");
    }
    else if(mode==3){
        f_data = TFile::Open("../selection/root_files/Bs/sideband_Bs.root");
        f_mc   = TFile::Open("../selection/root_files/Bs/MC_Bs.root");
    }

    TTree *tree_data = (TTree*)f_data->Get("tree");
    TTree *tree_mc   = (TTree*)f_mc->Get("tree");
    if(mode==0){
        tree_mc2  = (TTree*)f_mc2->Get("tree");
    }
    

    Float_t var_value;
    tree_data->SetBranchAddress(varName, &var_value);
    double min_data = tree_data->GetMinimum(varName);
    double max_data = tree_data->GetMaximum(varName);

    tree_mc->SetBranchAddress(varName, &var_value);
    double min_mc   = tree_mc->GetMinimum(varName);
    double max_mc   = tree_mc->GetMaximum(varName);

    double min_mc2,max_mc2;
    if(mode==0){
        tree_mc2->SetBranchAddress(varName, &var_value);
        min_mc2   = tree_mc2->GetMinimum(varName);
        max_mc2   = tree_mc2->GetMaximum(varName);
    }

    if(mode==0){
        var_min = std::min({min_data, min_mc, min_mc2});
        var_max = std::max({max_data, max_mc, max_mc2});
    }
    else{
        var_min = std::min({min_data, min_mc});
        var_max = std::max({max_data, max_mc});
    }
    

    int nBins = 100;
    TH1F *h_data = new TH1F("h_data", varName, nBins, var_min, var_max);
    TH1F *h_mc   = new TH1F("h_mc",   varName, nBins, var_min, var_max);
    if(mode==0){
        h_mc2  = new TH1F("h_mc2",  varName, nBins, var_min, var_max);
    }
   

    // Fill histograms
    tree_data->Draw(varName + ">>h_data", "", "goff");
    tree_mc->Draw(varName + ">>h_mc", "", "goff");
    if(mode==0){
        tree_mc2->Draw(varName + ">>h_mc2", "", "goff");
    }
    

    // Normalize histograms
    if (h_data->Integral() > 0) h_data->Scale(1.0 / h_data->Integral());
    if (h_mc->Integral() > 0)   h_mc->Scale(1.0 / h_mc->Integral());
    if(mode==0){
        if (h_mc2->Integral() > 0)  h_mc2->Scale(1.0 / h_mc2->Integral());
    }
    

    // Set styles
    int brightAzure = TColor::GetColor(51, 153, 255);
    h_data->SetLineColor(brightAzure);
    h_data->SetLineWidth(2);
    h_data->SetFillColor(brightAzure);
    h_data->SetFillStyle(3345);  // diagonal hatch

    h_mc->SetLineColor(kOrange + 7);
    h_mc->SetLineWidth(2);
    h_mc->SetFillColorAlpha(kOrange + 7, 0.4); // semi-transparent orange
    h_mc->SetFillStyle(1001);

    int lightBrightYellow = TColor::GetColor(255, 255, 0);  
    if(mode==0){
        h_mc2->SetLineColor(lightBrightYellow);
        h_mc2->SetLineWidth(2);
        h_mc2->SetFillColorAlpha(lightBrightYellow, 0.2);// semi-transparent yellow
        h_mc2->SetFillStyle(1001);
    }
    
    float max_val;
    if(mode==0){
        max_val = std::max({h_data->GetMaximum(), h_mc->GetMaximum(), h_mc2->GetMaximum()});
    }
    else{
        max_val = std::max({h_data->GetMaximum(), h_mc->GetMaximum()});
    }
    
    h_data->SetMaximum(max_val * 1.2);

    // Draw to canvas
    TCanvas *c1 = new TCanvas("c1", "Comparison", 800, 600);
    gStyle->SetOptStat(0);
    h_data->SetTitle("Comparison of " + varName + "; " + varName + "; Normalized Entries");
    h_data->Draw("hist");
    h_mc->Draw("hist same");
    if(mode==0){
        h_mc2->Draw("hist same");
    }

    // Add legend
    TLegend *leg = new TLegend(0.65, 0.70, 0.88, 0.88);
    leg->AddEntry(h_data, "sideband", "lf");
    if(mode==0){
        leg->AddEntry(h_mc,   "#Psi(2S)",   "lf");
    }
    else if(mode==1){
        leg->AddEntry(h_mc,   "B^{+}",   "lf");
    }
    else if(mode==2){
        leg->AddEntry(h_mc,   "B^{0}",   "lf");
    }
    else if(mode==3){
        leg->AddEntry(h_mc,   "B^{0}_{s}",   "lf");
    }
    if(mode==0){
        leg->AddEntry(h_mc2,  "X(3872)",  "lf");
    }
    leg->Draw();

    
    if(mode==0){
        gSystem->Exec("mkdir -p output_scan/XPSI");
        c1->SaveAs("output_scan/XPSI/"+varName + "_dist.pdf");
    } else if(mode==1){
        gSystem->Exec("mkdir -p output_scan/Bu");
        c1->SaveAs("output_scan/Bu/"+varName + "_dist.pdf");
    } else if(mode==2){
        gSystem->Exec("mkdir -p output_scan/Bd");
        c1->SaveAs("output_scan/Bd/"+varName + "_dist.pdf");
    } else if(mode==3){
        gSystem->Exec("mkdir -p output_scan/Bs");
        c1->SaveAs("output_scan/Bs/"+varName + "_dist.pdf");
    }
    
}

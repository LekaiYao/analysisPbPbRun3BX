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

void plot_@VARNAME@(int channel = 0) {
    TString varName = "@VARNAME@";
    TString massVar = "Bmass";

    double sig_low, sig_high, sb1_low, sb1_high, sb2_low, sb2_high, sb_low, sb_high;
    if(channel==0){
        //X
        sig_low  = 3.85;
        sig_high = 3.90;
        sb1_low  = 3.75;
        sb1_high = 3.80;
        sb2_low  = 3.95;
        sb2_high = 4.00;
    }
    else if(channel==1){
        //Bu
        sig_low  = 5.18;
        sig_high = 5.36;
        sb_low  = 5.4;
        sb_high = 5.6;
    }
    else if(channel==3){
        //Bs
        sig_low  = 5.30;
        sig_high = 5.42;
        sb1_low  = 5.00;
        sb1_high = 5.20;
        sb2_low  = 5.50;
        sb2_high = 5.70;
    }


    TFile *f_data = nullptr;
    TFile *f_splot = nullptr;
    TFile *f_mc   = nullptr;
    if(channel==0){
        f_splot = TFile::Open("./sw_root/X/DATA_X_cut2_sw.root");
        f_data = TFile::Open("../selection/root_files/X/DATA_X_cut2.root");
        f_mc   = TFile::Open("../selection/root_files/X/MC_X_cut1.root");
    }
    else if(channel==1){
        f_splot = TFile::Open("./sw_root/Bu/DATA_Bu_cut1_sw.root");
        f_data = TFile::Open("../selection/root_files/Bu/DATA_Bu_cut1.root");
        f_mc   = TFile::Open("../selection/root_files/Bu/MC_Bu_cut1.root");
    }
    else if(channel==3){
        f_splot = TFile::Open("./sw_root/Bs/DATA_Bs_cut1_sw.root");
        f_data = TFile::Open("../selection/root_files/Bs/DATA_Bs_cut1.root");
        f_mc   = TFile::Open("../selection/root_files/Bs/MC_Bs_cut1.root");
    }
    TTree *tree_splot = (TTree*)f_splot->Get("tree");
    TTree *tree_data = (TTree*)f_data->Get("tree");
    TTree *tree_mc   = (TTree*)f_mc->Get("tree");

    double var_min = std::min(std::min(tree_data->GetMinimum(varName), tree_mc->GetMinimum(varName)), tree_splot->GetMinimum(varName));
    double var_max = std::max(std::max(tree_data->GetMaximum(varName), tree_mc->GetMaximum(varName)), tree_splot->GetMaximum(varName));
    int nBins = 100;

    TH1F *h_splot = new TH1F("h_splot", "SPlot", nBins, var_min, var_max);
    TH1F *h_sig = new TH1F("h_sig", "Signal region", nBins, var_min, var_max);
    TH1F *h_sb1 = new TH1F("h_sb1", "Sideband1", nBins, var_min, var_max);
    TH1F *h_sb2 = new TH1F("h_sb2", "Sideband2", nBins, var_min, var_max);
    TH1F *h_bkg = new TH1F("h_bkg", "Combined sidebands", nBins, var_min, var_max);
    TH1F *h_sbs = new TH1F("h_sbs", "Sideband-subtracted", nBins, var_min, var_max);
    TH1F *h_mc  = new TH1F("h_mc",  "MC", nBins, var_min, var_max);

    tree_splot->Draw(varName + ">>h_splot", "sWeight", "goff");

    if(channel==1){
        tree_data->Draw(varName + ">>h_sig", Form("%s>%.3f && %s<%.3f", massVar.Data(), sig_low, massVar.Data(), sig_high), "goff");
        tree_data->Draw(varName + ">>h_bkg", Form("%s>%.3f && %s<%.3f", massVar.Data(), sb_low, massVar.Data(), sb_high), "goff");
        tree_mc->Draw(varName + ">>h_mc", "", "goff");

        double sig_width = sig_high - sig_low;
        double sb_width = sb_high - sb_low;
        double scale = sig_width / sb_width;

        h_bkg->Scale(scale);

        h_sbs->Add(h_sig);
        h_sbs->Add(h_bkg, -1);
    }
    else if((channel==0)||(channel==3)){
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

        h_sbs->Add(h_sig);
        h_sbs->Add(h_bkg, -1);
    }

    if (h_splot->Integral() > 0) h_splot->Scale(1.0 / h_splot->Integral());
    if (h_sbs->Integral() > 0) h_sbs->Scale(1.0 / h_sbs->Integral());
    if (h_mc->Integral() > 0)  h_mc->Scale(1.0 / h_mc->Integral());

    int azure = TColor::GetColor(51, 153, 255);
    h_sbs->SetLineColor(azure);
    h_sbs->SetLineWidth(2);
    h_sbs->SetFillColor(azure);
    h_sbs->SetFillStyle(3345);

    h_mc->SetLineColor(kOrange + 7);
    h_mc->SetLineWidth(2);
    h_mc->SetFillColorAlpha(kOrange + 7, 0.4);
    h_mc->SetFillStyle(1001);
    
    int lightBrightYellow = TColor::GetColor(255, 255, 0);
    h_splot->SetLineColor(lightBrightYellow);
    h_splot->SetLineWidth(2);
    h_splot->SetFillColorAlpha(lightBrightYellow, 0.2);// semi-transparent yellow
    h_splot->SetFillStyle(1001);

    float max_val = std::max(std::max(h_sbs->GetMaximum(), h_mc->GetMaximum()), h_splot->GetMaximum());
    h_sbs->SetMaximum(max_val * 1.2);

    gStyle->SetOptStat(0);
    TCanvas *c1 = new TCanvas("c1", "Sideband Subtraction", 800, 600);
    h_sbs->SetTitle("Sideband-subtracted " + varName + "; " + varName + "; Normalized entries");
    h_sbs->Draw("hist");
    h_mc->Draw("hist same");
    h_splot->Draw("hist same");

    TLegend *leg = new TLegend(0.65, 0.70, 0.88, 0.88);
    if(channel==0){
        //X
        leg->AddEntry(h_sbs, "X(3872) SBS", "lf");
        leg->AddEntry(h_mc,  "X(3872) MC", "lf");
        leg->AddEntry(h_splot, "X(3872) SPlot", "lf");
    }
    else if(channel==1){
        //Bu
        leg->AddEntry(h_sbs, "Bu SBS", "lf");
        leg->AddEntry(h_mc,  "Bu MC", "lf");
        leg->AddEntry(h_splot, "Bu SPlot", "lf");
    }
    else if(channel==3){
        //Bs
        leg->AddEntry(h_sbs, "Bs SBS", "lf");
        leg->AddEntry(h_mc,  "Bs MC", "lf");
        leg->AddEntry(h_splot, "Bs SPlot", "lf");
    }
    leg->Draw();

    if(channel==0){
        //X
        gSystem->Exec("mkdir -p all_output/X/");
        c1->SaveAs("all_output/X/" + varName + "_dist.pdf");
    }
    else if(channel==1){
        //Bu
        gSystem->Exec("mkdir -p all_output/Bu/");
        c1->SaveAs("all_output/Bu/" + varName + "_dist.pdf");
    }
    else if(channel==3){
        //Bs
        gSystem->Exec("mkdir -p all_output/Bs/");
        c1->SaveAs("all_output/Bs/" + varName + "_dist.pdf");
    }
}

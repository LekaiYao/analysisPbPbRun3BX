#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TBox.h>
#include <TLegend.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <TStyle.h>
#include <TGraph.h>
#include <TLine.h>
#include <TMarker.h>

// mode : 0(X+PSI),1(Bu),2(Bd),3(Bs),4(X)
void optimization_@VAR@(int mode = 0) {
    // scaling factors (background and signal)
    // wait to change
    Double_t fb = 0.329285, fs = 0.45116248;
    //input files
    TFile *f_bkg = nullptr;
    TFile *f_sig   = nullptr;
    TFile *f_sig2  = nullptr;
    TTree *tree_sig2 = nullptr;
    if (mode == 0) {
        f_bkg = TFile::Open("../selection/root_files/X/sideband_X.root");
        f_sig   = TFile::Open("../selection/root_files/X/MC_X.root");
        f_sig2  = TFile::Open("../selection/root_files/X/MC_PSI2S.root");
    }
    else if(mode==1){
        f_bkg = TFile::Open("../selection/root_files/Bu/sideband_Bu.root");
        f_sig   = TFile::Open("../selection/root_files/Bu/MC_Bu.root");
    }
    else if(mode==2){
        f_bkg = TFile::Open("../selection/root_files/Bd/sideband_Bd.root");
        f_sig   = TFile::Open("../selection/root_files/Bd/MC_Bd.root");
    }
    else if(mode==3){
        f_bkg = TFile::Open("../selection/root_files/Bs/sideband_Bs.root");
        f_sig   = TFile::Open("../selection/root_files/Bs/MC_Bs.root");
    }
    else if (mode==4) {
        f_bkg = TFile::Open("../selection/root_files/X/sideband_X.root");
        f_sig   = TFile::Open("../selection/root_files/X/MC_X.root");
    }

    TTree *tree_bkg = (TTree*)f_bkg->Get("tree");
    TTree *tree_sig = (TTree*)f_sig->Get("tree");
    if (mode==0){
        tree_sig2 = (TTree*)f_sig2->Get("tree");
    }

    int yellow = TColor::GetColor(204, 204,   0); 
	int orange = TColor::GetColor(204, 102,   0); 

    TCanvas *c1 = new TCanvas("c1", "Entry Scan", 900, 700);
    c1->SetGrid();

    float min = @MIN@;
    float max = @MAX@;
    float step = @STEP@;
    int n_points = int((max - min) / step) + 1;
    int nbin = int((max - min) / step) + 1;

    TString var_name = "@VAR@";

    // l for "x < cut" (less-than), g for "x > cut" (greater-than)
    Double_t n_bkg_l = 0, n_sig_l = 0, FOM_l = 0;
    Double_t n_bkg_g = 0, n_sig_g = 0, FOM_g = 0;

    Double_t n_sig2_l = 0, n_sig2_g = 0, FOM2_l = 0, FOM2_g = 0;

    TString flag = "";

    Double_t FOM_max_l = 0, cut_best_l = min;
    Double_t FOM_max_g = 0, cut_best_g = min;

    Double_t FOM2_max_l = 0, FOM2_max_g = 0;

    TGraph *graph_l = new TGraph(); // var < cut
    TGraph *graph_g = new TGraph(); // var > cut

    TGraph *graph_l2 = new TGraph(); // var < cut
    TGraph *graph_g2 = new TGraph(); // var > cut

    // Histograms for cumulative integrals
    TH1F *hist_bkg     = new TH1F("hist_bkg",     "hist_bkg",     nbin, min, max);
    TH1F *hist_sig       = new TH1F("hist_sig",       "hist_sig",       nbin, min, max);
    TH1F *hist_sig2   = new TH1F("hist_sig2",   "hist_sig2",   nbin, min, max); 

    // Fill histograms from trees
    // wait to change
    TString cutStr = "Bchi2cl > 0.003 && BQvalueuj < 0.2";
    tree_bkg->Draw(Form("%s >> hist_bkg", var_name.Data()), cutStr);
    tree_sig  ->Draw(Form("%s >> hist_sig",   var_name.Data()), cutStr);
    if(mode==0){
        tree_sig2->Draw(Form("%s >> hist_sig2", var_name.Data()), cutStr); 
    }


    // Loop over cut positions and compute FOM for both directions
    int ipt = 0;
    for (int i = 1; i < n_points; ++i) {
        float cut_val = min + i * step;

        // ------- LEFT: x < cut -------
        n_bkg_l   = hist_bkg->Integral(0, i);
        n_sig_l     = hist_sig->Integral(0, i);

        FOM_l      = (n_bkg_l==0.0 && n_sig_l==0.0) ? 0.0
                     : (n_sig_l * fs) / std::sqrt(n_sig_l * fs + n_bkg_l * fb);
        if (FOM_l > FOM_max_l) { FOM_max_l = FOM_l; cut_best_l = cut_val; }
        graph_l->SetPoint(ipt, cut_val, FOM_l);

        if(mode==0){
            n_sig2_l  = hist_sig2->Integral(0, i);         
            FOM2_l   = (n_bkg_l==0.0 && n_sig2_l==0.0) ? 0.0
                        : (n_sig2_l * fs) / std::sqrt(n_sig2_l * fs + n_bkg_l * fb); 
            if (FOM2_l > FOM2_max_l) FOM2_max_l = FOM2_l;   
            graph_l2->SetPoint(ipt, cut_val, FOM2_l);          
        }
        

        // ------- RIGHT: x > cut -------
        n_bkg_g   = hist_bkg->Integral(i, nbin + 1);
        n_sig_g     = hist_sig->Integral(i, nbin + 1);


        FOM_g      = (n_bkg_g==0.0 && n_sig_g==0.0) ? 0.0
                     : (n_sig_g * fs) / std::sqrt(n_sig_g * fs + n_bkg_g * fb);

        if (FOM_g > FOM_max_g) { FOM_max_g = FOM_g; cut_best_g = cut_val; }


        graph_g->SetPoint(ipt, cut_val, FOM_g);

        if(mode==0){
            n_sig2_g  = hist_sig2->Integral(i, nbin + 1);        
            FOM2_g   = (n_bkg_g==0.0 && n_sig2_g==0.0) ? 0.0    
                        : (n_sig2_g * fs) / std::sqrt(n_sig2_g * fs + n_bkg_g * fb);
            if (FOM2_g > FOM2_max_g) FOM2_max_g = FOM2_g;   
            graph_g2->SetPoint(ipt, cut_val, FOM2_g);          
        }

        ++ipt;
    }

    // Common axis/line/marker styling helpers (extended with color)
    auto style_graph = [&](TGraph* g, Color_t lc=kBlue+1, Style_t ms=20){
        g->SetLineColor(lc);
        g->SetMarkerColor(lc);
        g->SetLineWidth(2);
        g->SetMarkerStyle(ms);
        g->SetMarkerSize(0.9);
        g->SetTitle("");
        g->GetXaxis()->SetTitle(Form("%s", var_name.Data()));
        g->GetYaxis()->SetTitle("FOM");
        g->GetXaxis()->SetTitleSize(0.04);
        g->GetXaxis()->SetTitleOffset(1.05);
        g->GetYaxis()->SetTitleSize(0.04);
        g->GetYaxis()->SetTitleOffset(1.05);
    };

    // Y-range
    double yMax_combined;
    if(mode==0){
        yMax_combined = 1.2 * std::max(std::max(FOM_max_g, FOM_max_l),std::max(FOM2_max_g, FOM2_max_l));
    }
    else{
        yMax_combined = 1.2 * std::max(FOM_max_g, FOM_max_l);
    }


    // Decide which direction is better and draw
    if (FOM_max_g > FOM_max_l) {
        std::cout << "The best cut for " << var_name << " is greater than " << cut_best_g << std::endl;
        std::cout << "FOM is " << FOM_max_g << std::endl;
        flag = "g";

        style_graph(graph_g, yellow, 20); 
        graph_g->GetYaxis()->SetRangeUser(0, yMax_combined);
        graph_g->Draw("AL");

        if(mode==0){
            style_graph(graph_g2, orange, 24);
            graph_g2->Draw("L SAME");
        }


        // vertical line and best marker
        TLine *bestLine = new TLine(cut_best_g, 0, cut_best_g, FOM_max_g * 1.02);
        bestLine->SetLineColor(kOrange+7);
        bestLine->SetLineStyle(2);
        bestLine->SetLineWidth(2);
        bestLine->Draw("SAME");

        TMarker *bestMark = new TMarker(cut_best_g, FOM_max_g, 29);
        bestMark->SetMarkerColor(kMagenta+1);
        bestMark->SetMarkerSize(1.2);
        bestMark->Draw("SAME");

        // legend
        auto leg = new TLegend(0.58, 0.75, 0.90, 0.90); // NDC
        leg->SetTextSize(0.032);
        leg->SetTextFont(62);
        if(mode==0){
            leg->AddEntry(graph_g,     "X(3872)",  "l");
            leg->AddEntry(graph_g2, "(#psi(2S))", "l");
        }
        else if(mode==1){
            leg->AddEntry(graph_g,     "B^+",  "l");
        }
        else if(mode==2){
            leg->AddEntry(graph_g,     "B^0",  "l");
        }
        else if(mode==3){
            leg->AddEntry(graph_g,     "B^0_s",  "l");
        }
        else if(mode==4){
            leg->AddEntry(graph_g,     "X(3872)",  "l");
        }

        leg->AddEntry((TObject*)0, Form("Best cut : x > %.3f", cut_best_g), "h");
        leg->AddEntry((TObject*)0, Form("Max FOM = %.3f", FOM_max_g), "h");

        leg->Draw();

    } else {
        std::cout << "The best cut for " << var_name << " is less than " << cut_best_l << std::endl;
        std::cout << "FOM is " << FOM_max_l << std::endl;
        flag = "l";

        style_graph(graph_l, yellow, 20);
        graph_l->GetYaxis()->SetRangeUser(0, yMax_combined);
        graph_l->Draw("AL");

        if(mode==0){
            style_graph(graph_l2, orange, 24); 
            graph_l2->Draw("L SAME");
        }


        // vertical line and best marker
        TLine *bestLine = new TLine(cut_best_l, 0, cut_best_l, FOM_max_l * 1.02);
        bestLine->SetLineColor(kOrange+7);
        bestLine->SetLineStyle(2);
        bestLine->SetLineWidth(2);
        bestLine->Draw("SAME");

        TMarker *bestMark = new TMarker(cut_best_l, FOM_max_l, 29);
        bestMark->SetMarkerColor(kMagenta+1);
        bestMark->SetMarkerSize(1.2);
        bestMark->Draw("SAME");

        // legend
        auto leg = new TLegend(0.58, 0.75, 0.89, 0.90); // NDC
        leg->SetTextSize(0.032);
        leg->SetTextFont(62);
        if(mode==0){
            leg->AddEntry(graph_l,     "X(3872)",  "l");
            leg->AddEntry(graph_l2, "(#psi(2S))", "l");
        }
        else if(mode==1){
            leg->AddEntry(graph_l,     "B^+",  "l");
        }
        else if(mode==2){
            leg->AddEntry(graph_l,     "B^0",  "l");
        }
        else if(mode==3){
            leg->AddEntry(graph_l,     "B^0_s",  "l");
        }
        else if(mode==4){
            leg->AddEntry(graph_l,     "X(3872)",  "l");
        }
        leg->AddEntry((TObject*)0, Form("Best cut : x < %.3f", cut_best_l), "");
        leg->AddEntry((TObject*)0, Form("Max FOM = %.3f", FOM_max_l), "");
        leg->Draw();
    }

    // Save
    gPad->RedrawAxis();
    if(mode==0){
        gSystem->Exec("mkdir -p FOM/XPSI");
        c1->SaveAs(Form("FOM/XPSI/%s_%d_v1.pdf", var_name.Data(), nbin));
    } else if(mode==1){
        gSystem->Exec("mkdir -p FOM/Bu");
        c1->SaveAs(Form("FOM/Bu/%s_%d_v1.pdf", var_name.Data(), nbin));
    } else if(mode==2){
        gSystem->Exec("mkdir -p FOM/Bd");
        c1->SaveAs(Form("FOM/Bd/%s_%d_v1.pdf", var_name.Data(), nbin));
    } else if(mode==3){
        gSystem->Exec("mkdir -p FOM/Bs");
        c1->SaveAs(Form("FOM/Bs/%s_%d_v1.pdf", var_name.Data(), nbin));
    } else if(mode==4){
        gSystem->Exec("mkdir -p FOM/X");
        c1->SaveAs(Form("FOM/X/%s_%d_v1.pdf", var_name.Data(), nbin));
    }

    // Summary log with mode-specific output
    TString logDir;
    if      (mode == 0) logDir = "FOM/XPSI";
    else if (mode == 1) logDir = "FOM/Bu";
    else if (mode == 2) logDir = "FOM/Bd";
    else if (mode == 3) logDir = "FOM/Bs";
    else if (mode == 4) logDir = "FOM/X";

    gSystem->Exec(Form("mkdir -p %s", logDir.Data()));
    TString logFile = Form("%s/FOM_summary_v1.txt", logDir.Data());

    std::ofstream fout(logFile.Data(), std::ios::app);
    if (fout.is_open()) {
        if (flag == "g"){
            fout << "The best cut for " << var_name.Data()
                << " is greater than " << cut_best_g
                << " FOM_max=" << FOM_max_g << std::endl;
        } else {
            fout << "The best cut for " << var_name.Data()
                << " is less than " << cut_best_l
                << " FOM_max=" << FOM_max_l << std::endl;
        }
        fout << "The range of " << var_name.Data()
            << " is [ " << min << " , " << max << " ]" << std::endl;
        fout.close();
    }

    f_bkg->Close();
    f_sig->Close();
    if (mode==0) f_sig2->Close(); 
}

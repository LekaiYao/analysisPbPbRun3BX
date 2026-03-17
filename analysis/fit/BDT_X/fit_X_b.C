#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH2F.h"
#include "RooAbsPdf.h"
#include "RooExtendPdf.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "RooChebychev.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooAddPdf.h"
#include "RooArgList.h"
#include "RooFitResult.h"
#include "RooArgSet.h"
#include "RooFit.h"    

using namespace RooFit;

void fit_X_b() {
    TFile *f = TFile::Open("../../selection/root_files/X/BDT/DATA_X_BDT_test4.root");
    TTree *tree = (TTree*)f->Get("tree");
    
    float lowBmass=3.75,highBmass=4.0,lowBDTscore=-1.0,highBDTscore=1.0;
    Int_t N=1000000;//N is the max of events
	Int_t BinNum=100;
    
    //define variables
    RooRealVar Bmass("Bmass", "Bmass", lowBmass, highBmass);
	RooRealVar BDT_score("BDT_score", "BDT_score", lowBDTscore, highBDTscore);
    
    //define the RooArgSet of the Dataset
	RooArgSet vars;
		vars.add(Bmass);
		vars.add(BDT_score);
    RooDataSet data("data", "data", vars,Import(*tree));
	RooDataSet* newdata = (RooDataSet*)data.reduce("(BDT_score>0.052)");

	//signal of X(3872)
	RooRealVar mean("mean","mean",3.87,3.83,3.91);
	RooRealVar sigma1("sigma1","sigma1",0.008,1e-05,0.02);
	RooGaussian Gaus1("Gaus1","Gaus1",Bmass,mean,sigma1);
	RooRealVar sigma2("sigma2","sigma2",0.004,1e-05,0.01);
	RooGaussian Gaus2("Gaus2","Gaus2",Bmass,mean,sigma2);
	RooRealVar frac("frac","frac",0.5,0.2,0.8);
	RooAddPdf sig("sig","sig",RooArgList(Gaus1,Gaus2),frac);

	//background 
	//RooRealVar lambda("lambda","lambda",1,1e-03,10);
	//RooExponential bkg("bkg","bkg",Bmass,lambda,true);
	// Chebychev polynomial background
	RooRealVar c1("c1", "coefficient #1", 0.15, -1.0, 1.0);
	RooRealVar c2("c2", "coefficient #2", -0.05, -1.0, 1.0);
	// You can add more coefficients (c3, c4, ...) for higher-order polynomials

	RooChebychev bkg("bkg", "Chebychev background", Bmass, RooArgList(c1, c2));


	//normalization to get n_events
	RooRealVar n_sig = RooRealVar("n_sig","n_sig",1000,0,N);
	RooRealVar n_bkg = RooRealVar("n_bkg","n_bkg",1000,0,N);

	RooExtendPdf sige("sige","sige",sig,n_sig);
	RooExtendPdf bkge("bkge","bkge",bkg,n_bkg);
	
	//add them together
	RooAddPdf tot("total","total",RooArgList(sige,bkge),RooArgList(n_sig,n_bkg));

	n_sig.setVal(0);
	n_sig.setConstant(true);
	mean.setConstant(true);
	sigma1.setConstant(true);
	sigma2.setConstant(true);
	frac.setConstant(true);

	RooFitResult* fit_b=tot.fitTo(*newdata,Save(),Minimizer("Minuit2", "Migrad"));

	TCanvas *c_Bmass_b = new TCanvas("c_Bmass_b","c_Bmass_b",10,10,800,600);

	RooPlot *frame_Bmass_b = Bmass.frame(RooFit::Title("Fit X"),Bins(BinNum));

	newdata->plotOn(frame_Bmass_b,DataError(RooAbsData::SumW2));

	tot.plotOn(frame_Bmass_b,Name("bkg"),LineColor(1));
	TLegend leg2(0.7,0.7,0.9,0.9);
	leg2.AddEntry(frame_Bmass_b->findObject("bkg"),"bkg","L");
	frame_Bmass_b->Draw();
	leg2.DrawClone();

	gSystem->mkdir("./pdf_X_sigma", kTRUE);
	c_Bmass_b->SaveAs("./pdf_X_sigma/Xmass_test4_b_v1.pdf");

	cout << "NLL (b)   = " << fit_b->minNll() << endl;

    f->Close();
}
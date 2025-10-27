#include "analysis_data_Bu.C"
void runjobs()
{
	TChain *chain = new TChain("Bfinder/ntKp","");
	TString inputFile;
	chain->Add("INPUTPATH/*.root");
	analysis_data_Bu a(chain);
	a.Loop("/user/u/u25lekai/work/PbPb/analysis_B/selection/submit/output/OUTNAME");
}

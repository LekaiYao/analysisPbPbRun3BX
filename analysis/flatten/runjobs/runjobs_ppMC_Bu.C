#include "../pre-select/flatten_ppMC_Bu.C"
void runjobs_ppMC_Bu()
{
	TChain *chain = new TChain("Bfinder/ntKp","");
	TString inputFile;
	chain->Add("/lstore/cms/lekai/Bmeson/MC/ppRef/Bu_phat5_Bfinder.root");
	flatten_ppMC_Bu a(chain);
	a.Loop("./root_files/Bu/MC_Bu.root");//called by Run.sh, the relative path is from flatten/
}

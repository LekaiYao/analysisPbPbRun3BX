#include "../pre-select/flatten_ppMC_X.C"
void runjobs_ppMC_X()
{
	TChain *chain = new TChain("Bfinder/ntmix","");
	TString inputFile;
	chain->Add("/lstore/cms/hlegoinha/X3872/MC_DATA/prompt_X3872_to_Jpsi_Rho_phat5_Bfinder.root");
	flatten_ppMC_X a(chain);
	a.Loop("./root_files/X/MC_X.root");//called by Run.sh, the relative path is from flatten/
}

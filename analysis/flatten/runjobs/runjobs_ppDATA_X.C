#include "../pre-select/flatten_ppDATA_X.C"
void runjobs_ppDATA_X()
{
	TChain *chain = new TChain("Bfinder/ntmix","");
	TString inputFile;
	chain->Add("/lstore/cms/hlegoinha/X3872/MC_DATA/DATA_ppRef_X3872.root");
	flatten_ppDATA_X a(chain);
	a.Loop("./root_files/X/DATA_X.root");//called by Run.sh, the relative path is from flatten/
}

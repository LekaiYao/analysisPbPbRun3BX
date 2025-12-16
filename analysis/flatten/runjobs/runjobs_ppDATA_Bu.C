#include "../pre-select/flatten_ppDATA_Bu.C"
void runjobs_ppDATA_Bu()
{
	TChain *chain = new TChain("Bfinder/ntKp","");
	TString inputFile;
	chain->Add("/lstore/cms/hlegoinha/Bmeson/MC_DATA/DATA_ppref_Bmeson/DATA_ppref_Bmeson.root");
	flatten_ppDATA_Bu a(chain);
	a.Loop("./root_files/Bu/DATA_Bu.root");//called by Run.sh, the relative path is from flatten/
}

#include "../pre-select/flatten_ppDATA_Bs.C"
void runjobs_ppDATA_Bs()
{
	TChain *chain = new TChain("Bfinder/ntphi","");
	TString inputFile;
	chain->Add("/lstore/cms/hlegoinha/Bmeson/MC_DATA/DATA_ppref_Bmeson/DATA_ppref_Bmeson.root");
	flatten_ppDATA_Bs a(chain);
	a.Loop("./root_files/Bs/DATA_Bs.root");//called by Run.sh, the relative path is from flatten/
}

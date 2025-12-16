#include "../pre-select/flatten_ppMC_Bs.C"
void runjobs_ppMC_Bs()
{
	TChain *chain = new TChain("Bfinder/ntphi","");
	TString inputFile;
	chain->Add("/lstore/cms/lekai/Bmeson/MC/ppRef/Bs_phat5_Bfinder.root");
	flatten_ppMC_Bs a(chain);
	a.Loop("./root_files/Bs/MC_Bs.root");//called by Run.sh, the relative path is from flatten/
}

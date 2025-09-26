#define analysis_X3872_cxx
#include "analysis_X3872.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void analysis_X3872::Loop()
{
//   In a ROOT session, you can do:
//      root> .L analysis_X3872.C
//      root> analysis_X3872 t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;
   TFile* outputFile = new TFile("MC_X3872.root", "RECREATE");
   TTree* tree = new TTree("tree", "tree");

   struct Out {
      Int_t RunNo;
      Int_t CentBin;
      Int_t EvtNo;
      Int_t LumiNo;
      Float_t PVx;
      Float_t PVy;
      Float_t PVz;
      Float_t PVxE;
      Float_t PVyE;
      Float_t PVzE;
      Float_t PVnchi2;
      Int_t nChargedTracks;
      Int_t nSelectedChargedTracks;
      Int_t Bsize;
      Int_t Bindex;
      Int_t Btype;
      Float_t Bmass;
      Float_t Bpt;
      Float_t By;
      Float_t BvtxX;
      Float_t BvtxY;
      Float_t Bd0;
      Float_t Bd0Err;
      Float_t Bdxyz;
      Float_t BdxyzErr;
      Float_t Bchi2ndf;
      Float_t Bchi2cl;
      Float_t Bdtheta;
      Float_t Bcos_dtheta;
      Float_t Balpha;
      Float_t BsvpvDistance;
      Float_t BsvpvDisErr;
      Float_t BsvpvDistance_2D;
      Float_t BsvpvDisErr_2D;
      Float_t Bnorm_svpvDistance;
      Float_t Bnorm_svpvDistance_2D;
      Float_t BQvalue;
      Float_t BQvalueuj;
      Float_t BQvaluemumu;
      Float_t Btrk1Pt;
      Float_t Btrk2Pt;
      Float_t BtrkPtimb;
      Float_t Btrk1Eta;
      Float_t Btrk2Eta;
      Float_t Btrk1PtErr;
      Float_t Btrk2PtErr;
      Float_t Btrk1Dz;
      Float_t Btrk2Dz;
      Float_t Btrk1DzError;
      Float_t Btrk2DzError;
      Float_t Btrk1Dxy;
      Float_t Btrk2Dxy;
      Float_t Bnorm_trk1Dxy;
      Float_t Bnorm_trk2Dxy;
      Float_t Btrk1DxyError;
      Float_t Btrk2DxyError;
      Float_t Btrk1nPixelLayer;
      Float_t Btrk2nPixelLayer;
      Float_t Btrk1nStripLayer;
      Float_t Btrk2nStripLayer;
      Float_t Btrk1Chi2ndf;
      Float_t Btrk2Chi2ndf;
      Bool_t Btrk1highPurity;
      Bool_t Btrk2highPurity;
      Float_t Btrk1dR;
      Float_t Btrk2dR;
      Float_t Btktkmass;
      Float_t BtktkmassKK;
      Float_t BtktkvProb;
      Float_t Btktkpt;
      Float_t Bdoubletmass;
      Float_t Bmu1pt;
      Float_t Bmu2pt;
      Bool_t Bmu1highPurity;
      Bool_t Bmu2highPurity;
      Bool_t Bmu1isAcc;
      Bool_t Bmu2isAcc;
      Bool_t Bmu1SoftMuID;
      Bool_t Bmu2SoftMuID;
      Bool_t Bmu1HybridSoftMuID;
      Bool_t Bmu2HybridSoftMuID;
      Bool_t Bmu1isTriggered;
      Bool_t Bmu2isTriggered;
      Float_t Bmumumass;
      Float_t Bujmass;
      Float_t BujvProb;
      Float_t Bgen;
      Int_t BgenIndex;
   };

   Out out;

   // Branch definitions
   tree->Branch("RunNo", &out.RunNo, "RunNo/I");
   tree->Branch("CentBin", &out.CentBin, "CentBin/I");
   tree->Branch("EvtNo", &out.EvtNo, "EvtNo/I");
   tree->Branch("LumiNo", &out.LumiNo, "LumiNo/I");
   tree->Branch("PVx", &out.PVx, "PVx/F");
   tree->Branch("PVy", &out.PVy, "PVy/F");
   tree->Branch("PVz", &out.PVz, "PVz/F");
   tree->Branch("PVxE", &out.PVxE, "PVxE/F");
   tree->Branch("PVyE", &out.PVyE, "PVyE/F");
   tree->Branch("PVzE", &out.PVzE, "PVzE/F");
   tree->Branch("PVnchi2", &out.PVnchi2, "PVnchi2/F");
   tree->Branch("nChargedTracks", &out.nChargedTracks, "nChargedTracks/I");
   tree->Branch("nSelectedChargedTracks", &out.nSelectedChargedTracks, "nSelectedChargedTracks/I");
   tree->Branch("Bsize", &out.Bsize, "Bsize/I");
   tree->Branch("Bindex", &out.Bindex, "Bindex/I");
   tree->Branch("Btype", &out.Btype, "Btype/I");
   tree->Branch("Bmass", &out.Bmass, "Bmass/F");
   tree->Branch("Bpt", &out.Bpt, "Bpt/F");
   tree->Branch("By", &out.By, "By/F");
   tree->Branch("BvtxX", &out.BvtxX, "BvtxX/F");
   tree->Branch("BvtxY", &out.BvtxY, "BvtxY/F");
   tree->Branch("Bd0", &out.Bd0, "Bd0/F");
   tree->Branch("Bd0Err", &out.Bd0Err, "Bd0Err/F");
   tree->Branch("Bdxyz", &out.Bdxyz, "Bdxyz/F");
   tree->Branch("BdxyzErr", &out.BdxyzErr, "BdxyzErr/F");
   tree->Branch("Bchi2ndf", &out.Bchi2ndf, "Bchi2ndf/F");
   tree->Branch("Bchi2cl", &out.Bchi2cl, "Bchi2cl/F");
   tree->Branch("Bdtheta", &out.Bdtheta, "Bdtheta/F");
   tree->Branch("Bcos_dtheta", &out.Bcos_dtheta, "Bcos_dtheta/F");
   tree->Branch("Balpha", &out.Balpha, "Balpha/F");
   tree->Branch("BsvpvDistance", &out.BsvpvDistance, "BsvpvDistance/F");
   tree->Branch("BsvpvDisErr", &out.BsvpvDisErr, "BsvpvDisErr/F");
   tree->Branch("BsvpvDistance_2D", &out.BsvpvDistance_2D, "BsvpvDistance_2D/F");
   tree->Branch("BsvpvDisErr_2D", &out.BsvpvDisErr_2D, "BsvpvDisErr_2D/F");
   tree->Branch("Bnorm_svpvDistance", &out.Bnorm_svpvDistance, "Bnorm_svpvDistance/F");
   tree->Branch("Bnorm_svpvDistance_2D", &out.Bnorm_svpvDistance_2D, "Bnorm_svpvDistance_2D/F");
   tree->Branch("BQvalue", &out.BQvalue, "BQvalue/F");
   tree->Branch("BQvalueuj", &out.BQvalueuj, "BQvalueuj/F");
   tree->Branch("BQvaluemumu", &out.BQvaluemumu, "BQvaluemumu/F");
   tree->Branch("Btrk1Pt", &out.Btrk1Pt, "Btrk1Pt/F");
   tree->Branch("Btrk2Pt", &out.Btrk2Pt, "Btrk2Pt/F");
   tree->Branch("BtrkPtimb", &out.BtrkPtimb, "BtrkPtimb/F");
   tree->Branch("Btrk1Eta", &out.Btrk1Eta, "Btrk1Eta/F");
   tree->Branch("Btrk2Eta", &out.Btrk2Eta, "Btrk2Eta/F");
   tree->Branch("Btrk1PtErr", &out.Btrk1PtErr, "Btrk1PtErr/F");
   tree->Branch("Btrk2PtErr", &out.Btrk2PtErr, "Btrk2PtErr/F");
   tree->Branch("Btrk1Dz", &out.Btrk1Dz, "Btrk1Dz/F");
   tree->Branch("Btrk2Dz", &out.Btrk2Dz, "Btrk2Dz/F");
   tree->Branch("Btrk1DzError", &out.Btrk1DzError, "Btrk1DzError/F");
   tree->Branch("Btrk2DzError", &out.Btrk2DzError, "Btrk2DzError/F");
   tree->Branch("Btrk1Dxy", &out.Btrk1Dxy, "Btrk1Dxy/F");
   tree->Branch("Btrk2Dxy", &out.Btrk2Dxy, "Btrk2Dxy/F");
   tree->Branch("Bnorm_trk1Dxy", &out.Bnorm_trk1Dxy, "Bnorm_trk1Dxy/F");
   tree->Branch("Bnorm_trk2Dxy", &out.Bnorm_trk2Dxy, "Bnorm_trk2Dxy/F");
   tree->Branch("Btrk1DxyError", &out.Btrk1DxyError, "Btrk1DxyError/F");
   tree->Branch("Btrk2DxyError", &out.Btrk2DxyError, "Btrk2DxyError/F");
   tree->Branch("Btrk1nPixelLayer", &out.Btrk1nPixelLayer, "Btrk1nPixelLayer/F");
   tree->Branch("Btrk2nPixelLayer", &out.Btrk2nPixelLayer, "Btrk2nPixelLayer/F");
   tree->Branch("Btrk1nStripLayer", &out.Btrk1nStripLayer, "Btrk1nStripLayer/F");
   tree->Branch("Btrk2nStripLayer", &out.Btrk2nStripLayer, "Btrk2nStripLayer/F");
   tree->Branch("Btrk1Chi2ndf", &out.Btrk1Chi2ndf, "Btrk1Chi2ndf/F");
   tree->Branch("Btrk2Chi2ndf", &out.Btrk2Chi2ndf, "Btrk2Chi2ndf/F");
   tree->Branch("Btrk1highPurity", &out.Btrk1highPurity, "Btrk1highPurity/O");
   tree->Branch("Btrk2highPurity", &out.Btrk2highPurity, "Btrk2highPurity/O");
   tree->Branch("Btrk1dR", &out.Btrk1dR, "Btrk1dR/F");
   tree->Branch("Btrk2dR", &out.Btrk2dR, "Btrk2dR/F");
   tree->Branch("Btktkmass", &out.Btktkmass, "Btktkmass/F");
   tree->Branch("BtktkmassKK", &out.BtktkmassKK, "BtktkmassKK/F");
   tree->Branch("BtktkvProb", &out.BtktkvProb, "BtktkvProb/F");
   tree->Branch("Btktkpt", &out.Btktkpt, "Btktkpt/F");
   tree->Branch("Bdoubletmass", &out.Bdoubletmass, "Bdoubletmass/F");
   tree->Branch("Bmu1pt", &out.Bmu1pt, "Bmu1pt/F");
   tree->Branch("Bmu2pt", &out.Bmu2pt, "Bmu2pt/F");
   tree->Branch("Bmu1highPurity", &out.Bmu1highPurity, "Bmu1highPurity/O");
   tree->Branch("Bmu2highPurity", &out.Bmu2highPurity, "Bmu2highPurity/O");
   tree->Branch("Bmu1isAcc", &out.Bmu1isAcc, "Bmu1isAcc/O");
   tree->Branch("Bmu2isAcc", &out.Bmu2isAcc, "Bmu2isAcc/O");
   tree->Branch("Bmu1SoftMuID", &out.Bmu1SoftMuID, "Bmu1SoftMuID/O");
   tree->Branch("Bmu2SoftMuID", &out.Bmu2SoftMuID, "Bmu2SoftMuID/O");
   tree->Branch("Bmu1HybridSoftMuID", &out.Bmu1HybridSoftMuID, "Bmu1HybridSoftMuID/O");
   tree->Branch("Bmu2HybridSoftMuID", &out.Bmu2HybridSoftMuID, "Bmu2HybridSoftMuID/O");
   tree->Branch("Bmu1isTriggered", &out.Bmu1isTriggered, "Bmu1isTriggered/O");
   tree->Branch("Bmu2isTriggered", &out.Bmu2isTriggered, "Bmu2isTriggered/O");
   tree->Branch("Bmumumass", &out.Bmumumass, "Bmumumass/F");
   tree->Branch("Bujmass", &out.Bujmass, "Bujmass/F");
   tree->Branch("BujvProb", &out.BujvProb, "BujvProb/F");
   tree->Branch("Bgen", &out.Bgen, "Bgen/F");
   tree->Branch("BgenIndex", &out.BgenIndex, "BgenIndex/I");

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // progress monitoring
      Long64_t step = std::max<Long64_t>(1, nentries/20);
      if (jentry % step == 0) {
         double progress = 100.0 * jentry / nentries;
         printf("%.0f%% finished\n", progress);
      }
      /////////////////////////////////////////////////////////////////////
      if(Bsize == 0) continue;
      Long64_t i_B = 0;
      for(i_B=0;i_B<Bsize;i_B++){
         //////////////////// set all the cuts (below)//////////////////
         //isMCsignal
         if(!((Bgen[i_B] == 23333) || (Bgen[i_B] == 24333) 
         || (Bgen[i_B] == 23433) || (Bgen[i_B] == 24433))) continue;
         
         //FIDreg, _RAW
         if(!((Bpt[i_B] >= 2 )&& (By[i_B]  <= 2.4))) continue;

         //ACCcuts_ppRef, _ACC
         if(!((Btrk1Eta[i_B] < 2.4 && Btrk2Eta[i_B] < 2.4)
         && (Btrk1Pt[i_B]  > 0.5 && Btrk2Pt[i_B]  > 0.5)
         && (Bmu1isAcc[i_B] == 1 && Bmu2isAcc[i_B] == 1))) continue;

         //SELcuts_ppRef, _SEL
         if(!((Btrk1PtErr[i_B] / Btrk1Pt[i_B]) < 0.1 && (Btrk2PtErr[i_B] / Btrk2Pt[i_B]) < 0.1
         && (Btrk1nPixelLayer[i_B] + Btrk1nStripLayer[i_B]) > 10
         && (Btrk2nPixelLayer[i_B] + Btrk2nStripLayer[i_B]) > 10
         && (Btrk1Chi2ndf[i_B]/(Btrk1nPixelLayer[i_B] + Btrk1nStripLayer[i_B])) < 0.18
         && (Btrk2Chi2ndf[i_B]/(Btrk2nPixelLayer[i_B] + Btrk2nStripLayer[i_B])) < 0.18
         && Btrk1highPurity[i_B] && Btrk2highPurity[i_B]
         && (Bmu1SoftMuID[i_B] == 1 && Bmu2SoftMuID[i_B] == 1)
         && abs(Bujmass[i_B] - 3.096916) < 0.15 
         && BujvProb[i_B] > 0.01)) continue;

         //TRGmatching, _TRG
         if(!(Bmu1isTriggered[i_B] == 1 && Bmu2isTriggered[i_B] == 1)) continue;

         //////////////////// set all the cuts (above)//////////////////

         //////////////////// fill all the variables //////////////////
         // Assignment code
         out.RunNo = RunNo;
         out.CentBin = CentBin;
         out.EvtNo = EvtNo;
         out.LumiNo = LumiNo;
         out.PVx = PVx;
         out.PVy = PVy;
         out.PVz = PVz;
         out.PVxE = PVxE;
         out.PVyE = PVyE;
         out.PVzE = PVzE;
         out.PVnchi2 = PVnchi2;
         out.nChargedTracks = nChargedTracks;
         out.nSelectedChargedTracks = nSelectedChargedTracks;
         out.Bsize = Bsize;
         out.Bindex = Bindex[i_B];
         out.Btype = Btype[i_B];
         out.Bmass = Bmass[i_B];
         out.Bpt = Bpt[i_B];
         out.By = By[i_B];
         out.BvtxX = BvtxX[i_B];
         out.BvtxY = BvtxY[i_B];
         out.Bd0 = Bd0[i_B];
         out.Bd0Err = Bd0Err[i_B];
         out.Bdxyz = Bdxyz[i_B];
         out.BdxyzErr = BdxyzErr[i_B];
         out.Bchi2ndf = Bchi2ndf[i_B];
         out.Bchi2cl = Bchi2cl[i_B];
         out.Bdtheta = Bdtheta[i_B];
         out.Bcos_dtheta = Bcos_dtheta[i_B];
         out.Balpha = Balpha[i_B];
         out.BsvpvDistance = BsvpvDistance[i_B];
         out.BsvpvDisErr = BsvpvDisErr[i_B];
         out.BsvpvDistance_2D = BsvpvDistance_2D[i_B];
         out.BsvpvDisErr_2D = BsvpvDisErr_2D[i_B];
         out.Bnorm_svpvDistance = Bnorm_svpvDistance[i_B];
         out.Bnorm_svpvDistance_2D = Bnorm_svpvDistance_2D[i_B];
         out.BQvalue = BQvalue[i_B];
         out.BQvalueuj = BQvalueuj[i_B];
         out.BQvaluemumu = BQvaluemumu[i_B];
         out.Btrk1Pt = Btrk1Pt[i_B];
         out.Btrk2Pt = Btrk2Pt[i_B];
         out.BtrkPtimb = BtrkPtimb[i_B];
         out.Btrk1Eta = Btrk1Eta[i_B];
         out.Btrk2Eta = Btrk2Eta[i_B];
         out.Btrk1PtErr = Btrk1PtErr[i_B];
         out.Btrk2PtErr = Btrk2PtErr[i_B];
         out.Btrk1Dz = Btrk1Dz[i_B];
         out.Btrk2Dz = Btrk2Dz[i_B];
         out.Btrk1DzError = Btrk1DzError[i_B];
         out.Btrk2DzError = Btrk2DzError[i_B];
         out.Btrk1Dxy = Btrk1Dxy[i_B];
         out.Btrk2Dxy = Btrk2Dxy[i_B];
         out.Bnorm_trk1Dxy = Bnorm_trk1Dxy[i_B];
         out.Bnorm_trk2Dxy = Bnorm_trk2Dxy[i_B];
         out.Btrk1DxyError = Btrk1DxyError[i_B];
         out.Btrk2DxyError = Btrk2DxyError[i_B];
         out.Btrk1nPixelLayer = Btrk1nPixelLayer[i_B];
         out.Btrk2nPixelLayer = Btrk2nPixelLayer[i_B];
         out.Btrk1nStripLayer = Btrk1nStripLayer[i_B];
         out.Btrk2nStripLayer = Btrk2nStripLayer[i_B];
         out.Btrk1Chi2ndf = Btrk1Chi2ndf[i_B];
         out.Btrk2Chi2ndf = Btrk2Chi2ndf[i_B];
         out.Btrk1highPurity = Btrk1highPurity[i_B];
         out.Btrk2highPurity = Btrk2highPurity[i_B];
         out.Btrk1dR = Btrk1dR[i_B];
         out.Btrk2dR = Btrk2dR[i_B];
         out.Btktkmass = Btktkmass[i_B];
         out.BtktkmassKK = BtktkmassKK[i_B];
         out.BtktkvProb = BtktkvProb[i_B];
         out.Btktkpt = Btktkpt[i_B];
         out.Bdoubletmass = Bdoubletmass[i_B];
         out.Bmu1pt = Bmu1pt[i_B];
         out.Bmu2pt = Bmu2pt[i_B];
         out.Bmu1highPurity = Bmu1highPurity[i_B];
         out.Bmu2highPurity = Bmu2highPurity[i_B];
         out.Bmu1isAcc = Bmu1isAcc[i_B];
         out.Bmu2isAcc = Bmu2isAcc[i_B];
         out.Bmu1SoftMuID = Bmu1SoftMuID[i_B];
         out.Bmu2SoftMuID = Bmu2SoftMuID[i_B];
         out.Bmu1HybridSoftMuID = Bmu1HybridSoftMuID[i_B];
         out.Bmu2HybridSoftMuID = Bmu2HybridSoftMuID[i_B];
         out.Bmu1isTriggered = Bmu1isTriggered[i_B];
         out.Bmu2isTriggered = Bmu2isTriggered[i_B];
         out.Bmumumass = Bmumumass[i_B];
         out.Bujmass = Bujmass[i_B];
         out.BujvProb = BujvProb[i_B];
         out.Bgen = Bgen[i_B];
         out.BgenIndex = BgenIndex[i_B];

         tree->Fill();
      }
   }
   tree->Write();
   outputFile->Close();
   std::cout << "OK" << std::endl;
}

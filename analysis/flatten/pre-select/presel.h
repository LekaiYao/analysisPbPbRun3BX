#ifndef PRESEL_H
#define PRESEL_H

#include <cmath>

// -------------------------------------------------------------
//  Acceptance cuts (ACCcuts_ppRef, _ACC)
// -------------------------------------------------------------
inline bool isACC_pp(int i_B,
                       const float* Btrk1Eta,
                       const float* Btrk2Eta,
                       const float* Btrk1Pt,
                       const float* Btrk2Pt,
                       const bool* Bmu1isAcc,
                       const bool* Bmu2isAcc)
{
    return ((Btrk1Eta[i_B] < 2.4 && Btrk2Eta[i_B] < 2.4)
         && (Btrk1Pt[i_B] > 0.5 && Btrk2Pt[i_B] > 0.5)
         && (Bmu1isAcc[i_B] == 1 && Bmu2isAcc[i_B] == 1));
}

inline bool isACC_pp_Bu(int i_B,
                       const float* Btrk1Eta,
                       const float* Btrk1Pt,
                       const bool* Bmu1isAcc,
                       const bool* Bmu2isAcc)
{
    return ((Btrk1Eta[i_B] < 2.4)
         && (Btrk1Pt[i_B] > 0.5)
         && (Bmu1isAcc[i_B] == 1 && Bmu2isAcc[i_B] == 1));
}

// -------------------------------------------------------------
//  Selection cuts (SELcuts_ppRef, _SEL)
// -------------------------------------------------------------
inline bool isSEL_pp(int i_B,
                       const float* Btrk1PtErr,
                       const float* Btrk2PtErr,
                       const float* Btrk1Pt,
                       const float* Btrk2Pt,
                       const float* Btrk1nPixelLayer,
                       const float* Btrk1nStripLayer,
                       const float* Btrk2nPixelLayer,
                       const float* Btrk2nStripLayer,
                       const float* Btrk1Chi2ndf,
                       const float* Btrk2Chi2ndf,
                       const bool* Btrk1highPurity,
                       const bool* Btrk2highPurity,
                       const bool* Bmu1SoftMuID,
                       const bool* Bmu2SoftMuID,
                       const float* Bujmass,
                       const float* BujvProb)
{
    return ((Btrk1PtErr[i_B] / Btrk1Pt[i_B]) < 0.1
         && (Btrk2PtErr[i_B] / Btrk2Pt[i_B]) < 0.1
         && (Btrk1nPixelLayer[i_B] + Btrk1nStripLayer[i_B]) > 10
         && (Btrk2nPixelLayer[i_B] + Btrk2nStripLayer[i_B]) > 10
         && (Btrk1Chi2ndf[i_B] / (Btrk1nPixelLayer[i_B] + Btrk1nStripLayer[i_B])) < 0.18
         && (Btrk2Chi2ndf[i_B] / (Btrk2nPixelLayer[i_B] + Btrk2nStripLayer[i_B])) < 0.18
         && Btrk1highPurity[i_B] && Btrk2highPurity[i_B]
         && (Bmu1SoftMuID[i_B] == 1 && Bmu2SoftMuID[i_B] == 1)
         && std::fabs(Bujmass[i_B] - 3.096916) < 0.15
         && BujvProb[i_B] > 0.01);
}

inline bool isSEL_pp_Bu(int i_B,
                       const float* Btrk1PtErr,
                       const float* Btrk1Pt,
                       const float* Btrk1nPixelLayer,
                       const float* Btrk1nStripLayer,
                       const float* Btrk1Chi2ndf,
                       const bool* Btrk1highPurity,
                       const bool* Bmu1SoftMuID,
                       const bool* Bmu2SoftMuID,
                       const float* Bujmass,
                       const float* BujvProb)
{
    return ((Btrk1PtErr[i_B] / Btrk1Pt[i_B]) < 0.1
         && (Btrk1nPixelLayer[i_B] + Btrk1nStripLayer[i_B]) > 10
         && (Btrk1Chi2ndf[i_B] / (Btrk1nPixelLayer[i_B] + Btrk1nStripLayer[i_B])) < 0.18
         && Btrk1highPurity[i_B]
         && (Bmu1SoftMuID[i_B] == 1 && Bmu2SoftMuID[i_B] == 1)
         && std::fabs(Bujmass[i_B] - 3.096916) < 0.15
         && BujvProb[i_B] > 0.01);
}

// -------------------------------------------------------------
//  isSpecCase cut
// -------------------------------------------------------------
inline bool isSpecCaseBs(int i_B, const float* Btktkmass)
{
    return ((std::fabs(Btktkmass[i_B] - 1.019455) < 0.015));
}



// -------------------------------------------------------------
//  isMCsignal cut
// -------------------------------------------------------------
inline bool isMCsignal(int i_B, const float* Bgen)
{
    return ((Bgen[i_B] == 23333) || (Bgen[i_B] == 24333)
         || (Bgen[i_B] == 23433) || (Bgen[i_B] == 24433));
}

// -------------------------------------------------------------
//  Fiducial region (FIDreg, _RAW)
// -------------------------------------------------------------
inline bool isFID(int i_B, const float* Bpt, const float* By)
{
    return ((Bpt[i_B] >= 2) && ((By[i_B]) <= 2.4));
}

// -------------------------------------------------------------
//  Trigger matching (TRGmatching, _TRG), only pp
// -------------------------------------------------------------
inline bool isTRG(int i_B, const bool* Bmu1isTriggered, const bool* Bmu2isTriggered)
{
    return ((Bmu1isTriggered[i_B] == 1 && Bmu2isTriggered[i_B] == 1));
}

#endif

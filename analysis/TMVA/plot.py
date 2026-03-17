import ROOT

f = ROOT.TFile.Open("../selection/root_files/X/BDT/DATA_X_BDT_test4.root")
t = f.Get("tree")

cuts = [0.0, 0.05, 0.10, 0.15, 0.20]

for cut in cuts:
    h = ROOT.TH1F("h","Bmass",50,3.75,4.0)
    t.Draw("Bmass>>h", f"BDT_score>{cut}")
    c = ROOT.TCanvas("c","c",800,600)
    h.Draw()
    tag = int(cut*100)
    c.SaveAs(f"./BDTplot/Bmass_cut{tag:03d}.png")
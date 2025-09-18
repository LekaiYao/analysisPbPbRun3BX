import ROOT
from ROOT import TMVA, TFile, TTree
from array import array
import sys
import os

ROOT.gROOT.SetBatch(True)

# -----------------------------
# Handle command-line argument
# -----------------------------
if len(sys.argv) != 2:
    print("Usage: python3 apply_BDT.py <run_label>")
    sys.exit(1)

run_label = sys.argv[1]

output_dir = "BDT_root"
os.makedirs(output_dir, exist_ok=True)

# -----------------------------
# Configuration parameters
# -----------------------------
model_xml_path = f"dataset/dataset_{run_label}/weights/TMVAClassification_BDTs.weights.xml"
tree_name = "tree"

# List of datasets (input_path, output_path)
# Comment out the ones you do not want to run
datasets = [
    #("../selection/root_files/sideband.root",   f"{output_dir}/sideband_X3872_BDT_{run_label}.root"),
    ("../selection/root_files/MC_X3872.root",  f"{output_dir}/MC_X3872_BDT_{run_label}.root"),
    ("../selection/root_files/MC_PSI2S.root", f"{output_dir}/MC_PSI2S_BDT_{run_label}.root"),
    #("../selection/root_files/DATA_XPSI_cut0.root", f"{output_dir}/DATA_XPSI_BDT_{run_label}.root"),
]

# -----------------------------
# Load trained model
# -----------------------------
TMVA.Tools.Instance()
TMVA.PyMethodBase.PyInitialize()

reader = TMVA.Reader("!Color:!Silent")

# -----------------------------
# Define input variables (must match training!)
# -----------------------------
B_trk1dR = array('f', [0.])
B_Qvalueuj = array('f', [0.])
#B_chi2cl = array('f', [0.])
#B_cos_dtheta = array('f', [0.])
#B_norm_trk1Dxy = array('f', [0.])
#B_norm_svpvDistance = array('f', [0.])

#B_chi2cl = array('f', [0.])
#B_norm_trk1Dxy = array('f', [0.])
#B_norm_svpvDistance = array('f', [0.])

B_mass = array('f', [0.])

reader.AddVariable("B_trk1dR",B_trk1dR)
reader.AddVariable("B_Qvalueuj",B_Qvalueuj)
#reader.AddVariable("B_chi2cl",B_chi2cl)
#reader.AddVariable("B_cos_dtheta",B_cos_dtheta)
#reader.AddVariable("B_norm_trk1Dxy",B_norm_trk1Dxy)
#reader.AddVariable("B_norm_svpvDistance",B_norm_svpvDistance)
#reader.AddVariable("B_alpha", B_alpha)
#reader.AddVariable("B_chi2cl",B_chi2cl)
#reader.AddVariable("B_norm_trk1Dxy",B_norm_trk1Dxy)
#reader.AddVariable("B_norm_svpvDistance",B_norm_svpvDistance)

reader.AddSpectator("B_mass", B_mass)

reader.BookMVA("BDT", model_xml_path)

# -----------------------------
# Loop over datasets
# -----------------------------
for input_data_path, output_root in datasets:
    # -----------------------------
    # Load input data
    # -----------------------------
    input_file = TFile.Open(input_data_path)
    tree = input_file.Get(tree_name)

    # -----------------------------
    # Prepare output file and tree
    # -----------------------------
    output_file = TFile.Open(output_root, "RECREATE")
    output_tree = TTree("tree", "Tree with B_mass and BDT_score")

    # Variables to store in output
    B_mass = array('f', [0.])
    bdt_score = array('f', [0.])

    output_tree.Branch("B_mass", B_mass, "B_mass/F")
    output_tree.Branch("BDT_score", bdt_score, "BDT_score/F")

    # -----------------------------
    # Loop over events
    # -----------------------------
    nentries = tree.GetEntries()
    print(f"Processing {nentries} entries from {input_data_path}...")

    for i in range(nentries):
        tree.GetEntry(i)

        B_mass[0] = getattr(tree, "B_mass")

        # Assign input variable values
        B_trk1dR[0] = getattr(tree, "B_trk1dR")
        B_Qvalueuj[0] = getattr(tree, "B_Qvalueuj")
        #B_chi2cl[0] = getattr(tree, "B_chi2cl")
        #B_cos_dtheta[0] = getattr(tree, "B_cos_dtheta")
        #B_norm_trk1Dxy[0] = getattr(tree, "B_norm_trk1Dxy")
        #B_norm_svpvDistance[0] = getattr(tree, "B_norm_svpvDistance")

        #B_chi2cl[0] = getattr(tree, "B_chi2cl")
        #B_norm_trk1Dxy[0] = getattr(tree, "B_norm_trk1Dxy")
        #B_norm_svpvDistance[0] = getattr(tree, "B_norm_svpvDistance")

        # Evaluate BDT
        bdt_score[0] = reader.EvaluateMVA("BDT")

        # Get B_mass
        B_mass[0] = getattr(tree, "B_mass")

        # Fill output tree
        output_tree.Fill()

    # -----------------------------
    # Save output
    # -----------------------------
    output_file.cd()
    output_tree.Write("", ROOT.TObject.kOverwrite)
    output_file.Close()

    print(f"Output ROOT file saved: {output_root}")

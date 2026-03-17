import ROOT
from ROOT import TMVA, TFile, TTree, gSystem
from array import array
import sys
import os
from cuts_config import cuts, cutb
from ROOT import TCut, TTreeFormula

ROOT.gROOT.SetBatch(True)

# Get the input argument from the command line
if len(sys.argv) != 3:
    print("Usage: python3 apply_BDT.py <trainNumber> <channel>")
    print("trainNumber:test1,train2,...")
    print("channel:0(X),1(Bu),2(Bd),3(Bs)")
    sys.exit(1)

trainNumber = sys.argv[1]
channel = sys.argv[2]

if (channel == "0") :
    channel = "X"
elif (channel == "1") :
    channel = "Bu"
elif (channel == "2") :
    channel = "Bd"
elif (channel == "3") :
    channel = "Bs"

model_xml_path = f"dataset/{channel}/dataset_{trainNumber}/weights/TMVAClassification_BDTs.weights.xml"
output_dir = f"../selection/root_files/{channel}/BDT"
os.makedirs(output_dir, exist_ok=True)
tree_name = "tree"

log_dir = f"log/{channel}"
os.makedirs(log_dir, exist_ok=True)
log_path = f"{log_dir}/apply_{trainNumber}.log"
sys.stdout = open(log_path, "w")
sys.stderr = sys.stdout
gSystem.RedirectOutput(log_path, "w")

# List of datasets (input_file, output_file)
# Comment out the ones you do not want to run
datasets = [
    (f"../selection/root_files/{channel}/sideband_{channel}_cut1.root",   f"{output_dir}/sideband_{channel}_BDT_{trainNumber}.root"),
    (f"../selection/root_files/{channel}/MC_{channel}_cut1.root",  f"{output_dir}/MC_{channel}_BDT_{trainNumber}.root"),
    (f"../selection/root_files/{channel}/DATA_{channel}_cut2.root", f"{output_dir}/DATA_{channel}_BDT_{trainNumber}.root"),
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
Btrk1dR = array('f', [0.])
Btrk1Pt = array('f', [0.])
Btrk2dR = array('f', [0.])
Btrk2Pt = array('f', [0.])
#BQvalueuj = array('f', [0.])
#Bchi2cl = array('f', [0.])
Bcos_dtheta = array('f', [0.])
#Bnorm_trk1Dxy = array('f', [0.])
#Bnorm_svpvDistance = array('f', [0.])

#Bnorm_trk1Dxy = array('f', [0.])
#Bnorm_svpvDistance = array('f', [0.])

Bmass = array('f', [0.])

reader.AddVariable("Btrk1dR",Btrk1dR)
reader.AddVariable("Btrk1Pt",Btrk1Pt)
reader.AddVariable("Btrk2dR",Btrk2dR)
reader.AddVariable("Btrk2Pt",Btrk2Pt)
#reader.AddVariable("BQvalueuj",BQvalueuj)
#reader.AddVariable("Bchi2cl",Bchi2cl)
reader.AddVariable("Bcos_dtheta",Bcos_dtheta)
#reader.AddVariable("Bnorm_trk1Dxy",Bnorm_trk1Dxy)
#reader.AddVariable("Bnorm_svpvDistance",Bnorm_svpvDistance)
#reader.AddVariable("Balpha", Balpha)
#reader.AddVariable("Bnorm_trk1Dxy",Bnorm_trk1Dxy)
#reader.AddVariable("Bnorm_svpvDistance",Bnorm_svpvDistance)

reader.AddSpectator("Bmass", Bmass)

reader.BookMVA("BDT", model_xml_path)

# -----------------------------
# Loop over datasets
# -----------------------------
for input_file, output_file in datasets:
    # -----------------------------
    # Load input data
    # -----------------------------
    input_root = TFile.Open(input_file)
    tree = input_root.Get(tree_name)

    # -----------------------------
    # Prepare output file and tree
    # -----------------------------
    output_root = TFile.Open(output_file, "RECREATE")
    output_tree = TTree("tree", "Tree with Bmass and BDT_score")

    # Variables to store in output
    Bmass = array('f', [0.])
    bdt_score = array('f', [0.])

    output_tree.Branch("Bmass", Bmass, "Bmass/F")
    output_tree.Branch("BDT_score", bdt_score, "BDT_score/F")

    # -----------------------------
    # Loop over events
    # -----------------------------
    nentries = tree.GetEntries()
    print(f"Processing {nentries} entries from {input_file}...")

    for i in range(nentries):
        tree.GetEntry(i)

        Bmass[0] = getattr(tree, "Bmass")

        # Assign input variable values
        Btrk1dR[0] = getattr(tree, "Btrk1dR")
        Btrk1Pt[0] = getattr(tree, "Btrk1Pt")
        Btrk2dR[0] = getattr(tree, "Btrk2dR")
        Btrk2Pt[0] = getattr(tree, "Btrk2Pt")
        #BQvalueuj[0] = getattr(tree, "BQvalueuj")
        #Bchi2cl[0] = getattr(tree, "Bchi2cl")
        Bcos_dtheta[0] = getattr(tree, "Bcos_dtheta")
        #Bnorm_trk1Dxy[0] = getattr(tree, "Bnorm_trk1Dxy")
        #Bnorm_svpvDistance[0] = getattr(tree, "Bnorm_svpvDistance")

        #Bnorm_trk1Dxy[0] = getattr(tree, "Bnorm_trk1Dxy")
        #Bnorm_svpvDistance[0] = getattr(tree, "Bnorm_svpvDistance")

        # Evaluate BDT
        bdt_score[0] = reader.EvaluateMVA("BDT")

        # Fill output tree
        output_tree.Fill()

    # -----------------------------
    # Save output
    # -----------------------------
    output_root.cd()
    output_tree.Write("", ROOT.TObject.kOverwrite)
    output_root.Close()
    gSystem.RedirectOutput("")

    print(f"Output ROOT file saved: {output_file}")

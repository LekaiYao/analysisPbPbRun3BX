import ROOT
from ROOT import TMVA, TFile, TTree, TCut, gSystem
import os.path
import sys

# Get the input argument from the command line
if len(sys.argv) != 3:
    print("Usage: python3 TMVA_BDTs.py <trainNumber> <channel>")
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

path_sig = f"../selection/root_files/{channel}/MC_{channel}.root"
path_bkg = f"../selection/root_files/{channel}/sideband_{channel}.root"

log_dir = f"log/{channel}"
os.makedirs(log_dir, exist_ok=True)
log_path = f"{log_dir}/train_{trainNumber}.log"
sys.stdout = open(log_path, "w")
sys.stderr = sys.stdout
gSystem.RedirectOutput(log_path, "w")

TMVA.Tools.Instance()               # need to run this two to load up TMVA
TMVA.PyMethodBase.PyInitialize()    # in PyROOT

# input PATH
dir = "../selection/root_files/"
data_file = "sideband.root"
mc_file = "MC_X3872.root"
 
# Open the ROOT files and access the TTree for data and MC
bkg = TFile.Open(path_bkg)
sig = TFile.Open(path_sig)
background = bkg.Get("tree")
signal = sig.Get("tree")

# directories where the results will be stored
os.makedirs("dataset/"+channel+"/dataset_" + trainNumber + "/results/rootfiles",exist_ok=True)
os.makedirs("dataset/"+channel+"/dataset_" + trainNumber + "/weights",exist_ok=True)
os.makedirs("dataset/"+channel+"/dataset_" + trainNumber + "/plots",exist_ok=True)

# Create a ROOT output file where TMVA will store ntuples, histograms, correlationMatrix, etc
outfname='dataset/'+channel+'/dataset_' + trainNumber + '/results/rootfiles/TMVA_BDT.root' 
output = TFile.Open(outfname, 'RECREATE')

cuts="(Bchi2cl>0.003 && BQvalueuj <= 0.2)&&(Bmass > 3.83 && Bmass < 3.91)" # MC
cutb="(Bchi2cl>0.003 && BQvalueuj <= 0.2)&&((Bmass < 3.83 && Bmass > 3.75)||(Bmass > 3.91&&Bmass < 4.0))" # data

mycutS=TCut(cuts)
mycutB=TCut(cutb)

factory = TMVA.Factory('TMVAClassification', output, '!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification')

# loads the outputs to the dir 'dataset'
dataloader = TMVA.DataLoader('dataset/'+channel+'/dataset_' + trainNumber)

# features to train the BDT (keep as your original)
dataloader.AddVariable("Btrk1dR")
dataloader.AddVariable("Btrk1Pt")
dataloader.AddVariable("Btrk2dR")
dataloader.AddVariable("Btrk2Pt")
dataloader.AddVariable("Bchi2cl")
#dataloader.AddVariable("Bcos_dtheta")
#dataloader.AddVariable("Bnorm_trk1Dxy")
#dataloader.AddVariable("Bnorm_svpvDistance")
#dataloader.AddVariable("BQvalueuj")


# NEW: add mass as spectator to monitor mass sculpting
dataloader.AddSpectator("Bmass")

signalWeight     = 1.0        # MC/signal
backgroundWeight = 1.0        # dataset sidebands/background

dataloader.AddSignalTree( signal, signalWeight )
dataloader.AddBackgroundTree( background, backgroundWeight )

# Count available events
sigCutEvents = signal.GetEntries(cuts)
bkgCutEvents = background.GetEntries(cutb)

# Train/test split (keep 70% idea but cap training sizes to reduce overfitting/imbalance)
# Caps chosen for your sample sizes: ~10000 signal, ~10000 background for training if available
sigTrainCap = min(10000, int(sigCutEvents * 0.7))
bkgTrainCap = min(10000, int(bkgCutEvents * 0.7))

# Make sure they are not zero and do not exceed totals
sigTrain = max(1, min(sigTrainCap, sigCutEvents))
bkgTrain = max(1, min(bkgTrainCap, bkgCutEvents))

# Prepare training and test trees (explicit training counts; keep NormMode=NumEvents)
dataloader.PrepareTrainingAndTestTree(
    mycutS, mycutB,
    "nTrain_Signal=%i:nTrain_Background=%i:SplitMode=Random:SplitSeed=12345:NormMode=NumEvents:!V" % (sigTrain, bkgTrain)
)


# Book methods
# (keep your BDT method string but with safer hyperparameters and bagging; retain VarTransform=Decorrelate as in your original)
factory.BookMethod(
    dataloader, "BDT", "BDTs",
    "!H:!V:"
    "NTrees=500:"
    "MinNodeSize=5%:"
    "MaxDepth=3:"
    "BoostType=AdaBoost:"
    "VarTransform=Decorrelate:"
    "SeparationType=GiniIndex:"
    "nCuts=30:"
    "UseBaggedBoost=True:"
    "BaggedSampleFraction=0.8"
)

# Run training,BDT test and evaluation
factory.TrainAllMethods()
factory.TestAllMethods()
factory.EvaluateAllMethods() # add full dataset

# Plot ROC Curves AND OTHERS
roc = factory.GetROCCurve(dataloader)
if roc:
    roc.SaveAs('dataset/'+channel+'/dataset_' + trainNumber + '/plots/ROC_ClassificationBDT.png')

# close the output file
output.Close()
gSystem.RedirectOutput(0)

# open the GUI interface
# TMVA.TMVAGui(outfname)

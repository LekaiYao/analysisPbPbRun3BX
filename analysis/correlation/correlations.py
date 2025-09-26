#!/usr/bin/env python3

import uproot
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import sys
import os

# ==========================
# CONFIGURATION
# ==========================
if len(sys.argv) != 3:
    print("Usage: python3 correlation.py <ANALYSIS_TYPE> <CHANNEL>")
    print("ANALYSIS_TYPE:signal, background, both")
    print("CHANNEL:0(X),1(Bu),2(Bd),3(Bs)")
    sys.exit(1)

ANALYSIS_TYPE = sys.argv[1]
CHANNEL = sys.argv[2]
#ANALYSIS_TYPE = "both"  # Options: "signal", "background", "both"
tree_name = "tree"
#X
if (CHANNEL == "0") :
    input_MCfile = "../selection/root_files/X/MC_X.root"
    input_bkgfile = "../selection/root_files/X/sideband_X.root"
    CHANNEL = "X"
#Bu
elif (CHANNEL == "1") :
    input_MCfile = "../selection/root_files/Bu/MC_Bu.root"
    input_bkgfile = "../selection/root_files/Bu/sideband_Bu.root"
    CHANNEL = "Bu"
#Bd
elif (CHANNEL == "2") :
    input_MCfile = "../selection/root_files/Bd/MC_Bd.root"
    input_bkgfile = "../selection/root_files/Bd/sideband_Bd.root"
    CHANNEL = "Bd"
#Bs
elif (CHANNEL == "3") :
    input_MCfile = "../selection/root_files/Bs/MC_Bs.root"
    input_bkgfile = "../selection/root_files/Bs/sideband_Bs.root"
    CHANNEL = "Bs"

# --------------------------
# Define dataset configs
# --------------------------
configs = {
    "signal": {
        "channel":CHANNEL,
        "root_file": input_MCfile,
        "is_mc": True,
        "use_sidebands": False,
        "output_suffix": "signal",
        "variables": [
            "Bpt", "By", "Bmass", "Balpha", "BQvalueuj", "BQvalue", "Bcos_dtheta", "BtrkPtimb",
            "Bchi2cl", "Btrk1dR", "Btrk2dR", "Btrk1Pt", "Btrk2Pt",
            "Bnorm_svpvDistance_2D", "Bnorm_svpvDistance", "Bnorm_trk1Dxy", "Bnorm_trk2Dxy",
            "Bgen", "Btrk1Eta", "Btrk2Eta", "Bmu1isAcc", "Bmu2isAcc",
            "Btrk1PtErr", "Btrk2PtErr", "Btrk1nPixelLayer", "Btrk1nStripLayer",
            "Btrk2nPixelLayer", "Btrk2nStripLayer", "Btrk1Chi2ndf", "Btrk2Chi2ndf",
            "Btrk1highPurity", "Btrk2highPurity", "Bmu1SoftMuID", "Bmu2SoftMuID",
            "Bujmass", "BujvProb", "Bmu1isTriggered", "Bmu2isTriggered"
        ]
    },
    "background": {
        "channel":CHANNEL,
        "root_file": input_bkgfile,
        "is_mc": False,
        "use_sidebands": True,
        "output_suffix": "background",
        "variables": [
            "Bpt", "By", "Bmass", "Balpha", "BQvalueuj", "BQvalue", "Bcos_dtheta", "BtrkPtimb",
            "Bchi2cl", "Btrk1dR", "Btrk2dR", "Btrk1Pt", "Btrk2Pt",
            "Bnorm_svpvDistance_2D", "Bnorm_svpvDistance", "Bnorm_trk1Dxy", "Bnorm_trk2Dxy",
            "Bmu1isTriggered", "Bmu2isTriggered"
        ]
    }
}

# ==========================
# Function to process one dataset
# ==========================
def process_dataset(cfg):
    channel = cfg["channel"]
    root_file = cfg["root_file"]
    variables = cfg["variables"]
    output_suffix = cfg["output_suffix"]

    print(f"\nChannel: {channel}")
    print(f"\nAnalysis Type: {output_suffix}")
    print(f"Input file: {root_file}")
    print(f"Is MC: {cfg['is_mc']}, Use sidebands: {cfg['use_sidebands']}")

    # Load ROOT file in chunks
    print(f"Opening {root_file} ...")
    with uproot.open(root_file) as file:
        if tree_name not in file:
            raise ValueError(f"TTree '{tree_name}' not found in file. Available: {list(file.keys())}")
        tree = file[tree_name]
        n_entries = tree.num_entries
        print(f"Total entries: {n_entries}")
        chunk_size = 500000
        parts = []
        for start in range(0, n_entries, chunk_size):
            end = min(start + chunk_size, n_entries)
            print(f"Chunk {start//chunk_size+1}: entries {start} to {end-1}")

            arrays = tree.arrays(variables, entry_start=start, entry_stop=end, library="np")
            data = {var: arrays[var] for var in variables}
            df_part = pd.DataFrame(data)
            df_part.dropna(inplace=True)
            parts.append(df_part)

        df_filtered = pd.concat(parts, ignore_index=True) if parts else pd.DataFrame(columns=variables)
        print(f"Total retained after all cuts: {len(df_filtered)}")

    # Correlation
    correlation_vars = [
        "Bpt", "By", "Bmass", "Balpha", "BQvalueuj", "BQvalue", "Bcos_dtheta",
        "BtrkPtimb", "Bchi2cl", "Btrk1dR", "Btrk2dR", "Btrk1Pt", "Btrk2Pt",
        "Bnorm_svpvDistance_2D", "Bnorm_svpvDistance", "Bnorm_trk1Dxy", "Bnorm_trk2Dxy"
    ]

    if df_filtered.empty:
        print("No events retained after cuts. Skipping plotting.")
    else:
        df_corr = df_filtered[correlation_vars]
        corr_matrix = df_corr.corr(method="pearson")
        print("\nCorrelation matrix:")
        print(corr_matrix)

        plt.figure(figsize=(16, 12))
        sns.heatmap(corr_matrix, annot=True, cmap="coolwarm", vmin=-1, vmax=1,
                    fmt='.3f', square=True, cbar_kws={"shrink": .8})
        plt.title("", fontsize=16)
        plt.xticks(rotation=45, ha='right')
        plt.yticks(rotation=0)
        plt.tight_layout()

        os.makedirs("Matrix_pdf", exist_ok=True)
        output_file = f"Matrix_pdf/{channel}_{output_suffix}_correlation_matrix.pdf"
        plt.savefig(output_file, dpi=300, bbox_inches='tight', format='pdf')
        print(f"Correlation matrix plot saved as: {output_file}")
        plt.close()

# ==========================
# Run analysis
# ==========================
if ANALYSIS_TYPE == "both":
    for key in ["signal", "background"]:
        process_dataset(configs[key])
elif ANALYSIS_TYPE in configs:
    process_dataset(configs[ANALYSIS_TYPE])
else:
    raise ValueError(f"Unknown ANALYSIS_TYPE: {ANALYSIS_TYPE}")

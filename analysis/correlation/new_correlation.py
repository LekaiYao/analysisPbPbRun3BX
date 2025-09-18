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
if len(sys.argv) != 2:
    print("Usage: python3 new_correlation.py <ANALYSIS_TYPE>")
    print("ANALYSIS_TYPE:MC_signal, Data_sidebands, both")
    sys.exit(1)

ANALYSIS_TYPE = sys.argv[1]
#ANALYSIS_TYPE = "both"  # Options: "MC_signal", "Data_sidebands", "both"
tree_name = "tree"
input_path = "../selection/root_files"

# --------------------------
# Define dataset configs
# --------------------------
configs = {
    "MC_signal": {
        "root_file": f"{input_path}/MC_X3872.root",
        "is_mc": True,
        "use_sidebands": False,
        "output_suffix": "MC_signal",
        "variables": [
            "B_pt", "B_y", "B_mass", "B_alpha", "B_Qvalueuj", "B_Qvalue", "B_cos_dtheta", "B_trkPtimb",
            "B_chi2cl", "B_trk1dR", "B_trk2dR", "B_trk1Pt", "B_trk2Pt",
            "B_norm_svpvDistance_2D", "B_norm_svpvDistance", "B_norm_trk1Dxy", "B_norm_trk2Dxy",
            "B_gen", "B_trk1Eta", "B_trk2Eta", "B_mu1isAcc", "B_mu2isAcc",
            "B_trk1PtErr", "B_trk2PtErr", "B_trk1nPixelLayer", "B_trk1nStripLayer",
            "B_trk2nPixelLayer", "B_trk2nStripLayer", "B_trk1Chi2ndf", "B_trk2Chi2ndf",
            "B_trk1highPurity", "B_trk2highPurity", "B_mu1SoftMuID", "B_mu2SoftMuID",
            "B_ujmass", "B_ujvProb", "B_mu1isTriggered", "B_mu2isTriggered"
        ]
    },
    "Data_sidebands": {
        "root_file": f"{input_path}/sideband.root",
        "is_mc": False,
        "use_sidebands": True,
        "output_suffix": "Data_sidebands",
        "variables": [
            "B_pt", "B_y", "B_mass", "B_alpha", "B_Qvalueuj", "B_Qvalue", "B_cos_dtheta", "B_trkPtimb",
            "B_chi2cl", "B_trk1dR", "B_trk2dR", "B_trk1Pt", "B_trk2Pt",
            "B_norm_svpvDistance_2D", "B_norm_svpvDistance", "B_norm_trk1Dxy", "B_norm_trk2Dxy",
            "B_mu1isTriggered", "B_mu2isTriggered"
        ]
    }
}

# ==========================
# Function to process one dataset
# ==========================
def process_dataset(cfg):
    root_file = cfg["root_file"]
    variables = cfg["variables"]
    output_suffix = cfg["output_suffix"]

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
        "B_pt", "B_y", "B_mass", "B_alpha", "B_Qvalueuj", "B_Qvalue", "B_cos_dtheta",
        "B_trkPtimb", "B_chi2cl", "B_trk1dR", "B_trk2dR", "B_trk1Pt", "B_trk2Pt",
        "B_norm_svpvDistance_2D", "B_norm_svpvDistance", "B_norm_trk1Dxy", "B_norm_trk2Dxy"
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
        output_file = f"Matrix_pdf/correlation_matrix_{output_suffix}_X3872.pdf"
        plt.savefig(output_file, dpi=300, bbox_inches='tight', format='pdf')
        print(f"Correlation matrix plot saved as: {output_file}")
        plt.close()

# ==========================
# Run analysis
# ==========================
if ANALYSIS_TYPE == "both":
    for key in ["MC_signal", "Data_sidebands"]:
        process_dataset(configs[key])
elif ANALYSIS_TYPE in configs:
    process_dataset(configs[ANALYSIS_TYPE])
else:
    raise ValueError(f"Unknown ANALYSIS_TYPE: {ANALYSIS_TYPE}")

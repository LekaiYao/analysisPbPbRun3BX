# cuts.py
# Dictionary: key = cut name, value = list of cut expressions

cuts = {
    #sideband cut
    "X_sideband_cut0": [
        "((tree.Bmass > 3.6) and (tree.Bmass < 3.66)) or ((tree.Bmass > 3.72) and (tree.Bmass < 3.83)) or ((tree.Bmass > 3.91) and (tree.Bmass < 4.0))",
    ],
    "Bu_sideband_cut0": [
        "((tree.Bmass > 5.0) and (tree.Bmass < 5.2)) or ((tree.Bmass > 5.4) and (tree.Bmass < 6.0))",
    ],
    "Bd_sideband_cut0": [
        "((tree.Bmass > 5.0) and (tree.Bmass < 5.2)) or ((tree.Bmass > 5.4) and (tree.Bmass < 6.0))",
    ],
    "Bs_sideband_cut0": [
        "((tree.Bmass > 5.0) and (tree.Bmass < 5.3)) or ((tree.Bmass > 5.44) and (tree.Bmass < 6.0))",
    ],
    #data cut
    "X_data_cut0": [
        "(tree.Bmass > 3.6) and (tree.Bmass < 4.0)",
        "tree.Bchi2cl >= 0.003",
        "tree.BQvalueuj <= 0.2",
    ],
    "X_data_cut1": [
        "(tree.Bmass > 3.6) and (tree.Bmass < 4.0)",
        "tree.Bchi2cl >= 0.003",
        "tree.BQvalueuj <= 0.095",
    ],
    "X_data_cut2": [
        "(tree.Bmass > 3.75) and (tree.Bmass < 4.0)",
        "tree.Bchi2cl >= 0.003",
        "tree.BQvalueuj <= 0.095",
    ],
    "Bu_data_cut0": [
        "(tree.Bmass > 5.0) and (tree.Bmass < 6.0)",
        "tree.Bchi2cl >= 0.003",
        "tree.Bnorm_svpvDistance>2",
    ],
    "Bu_data_cut1": [
        "(tree.Bmass > 5.18) and (tree.Bmass < 5.8)",
        "tree.Bchi2cl >= 0.003",
        "tree.Bnorm_svpvDistance>2",
        "tree.Bnorm_svpvDistance_2D>4",
        "tree.Btrk1dR<1.25",
    ],
    "Bd_data_cut0": [
        "(tree.Bmass > 5.0) and (tree.Bmass < 6.0)",
        "tree.Bchi2cl >= 0.003",
        "tree.Bnorm_svpvDistance>2",
    ],
    "Bs_data_cut0": [
        "(tree.Bmass > 5.0) and (tree.Bmass < 6.0)",
        "tree.Bchi2cl >= 0.003",
        "tree.Bnorm_svpvDistance>2",
    ],
    "Bs_data_cut1": [
        "(tree.Bmass > 5.0) and (tree.Bmass < 5.8)",
        "tree.Bchi2cl >= 0.003",
        "tree.Bnorm_svpvDistance>2",
        "tree.Bnorm_svpvDistance_2D>4",
        "tree.Btrk1dR<1.25",
    ],
    #MC cut
    "X_MC_cut0": [
        "(tree.Bmass > 3.83) and (tree.Bmass < 3.91)",
        "tree.Bchi2cl >= 0.003",
        "tree.BQvalueuj <= 0.2",
    ],
    "X_MC_cut1": [
        "(tree.Bmass > 3.83) and (tree.Bmass < 3.91)",
        "tree.Bchi2cl >= 0.003",
        "tree.BQvalueuj <= 0.095",
    ],
    "Bu_MC_cut0": [
        "(tree.Bmass > 5.2) and (tree.Bmass < 5.4)",
        "tree.Bchi2cl >= 0.003",
        "tree.Bnorm_svpvDistance>2",
    ],
    "Bu_MC_cut1": [
        "(tree.Bmass > 5.2) and (tree.Bmass < 5.4)",
        "tree.Bchi2cl >= 0.003",
        "tree.Bnorm_svpvDistance>2",
        "tree.Bnorm_svpvDistance_2D>4",
        "tree.Btrk1dR<1.25",
    ],
    "Bd_MC_cut0": [
        "(tree.Bmass > 5.2) and (tree.Bmass < 5.4)",
        "tree.Bchi2cl >= 0.003",
        "tree.Bnorm_svpvDistance>2",
    ],
    "Bs_MC_cut0": [
        "(tree.Bmass > 5.3) and (tree.Bmass < 5.44)",
        "tree.Bchi2cl >= 0.003",
        "tree.Bnorm_svpvDistance>2",
    ],
    "Bs_MC_cut1": [
        "(tree.Bmass > 5.3) and (tree.Bmass < 5.44)",
        "tree.Bchi2cl >= 0.003",
        "tree.Bnorm_svpvDistance>2",
        "tree.Bnorm_svpvDistance_2D>4",
        "tree.Btrk1dR<1.25",
    ],
    "PSI2S_MC_cut0": [
        "(tree.Bmass > 3.66) and (tree.Bmass < 3.72)",
        "tree.Bchi2cl >= 0.003",
        "tree.BQvalueuj <= 0.2",
    ],
    #X only cut
    "X_only_cut0": [
        "(tree.Bmass > 3.75) and (tree.Bmass < 4.0)",
        "tree.Bchi2cl >= 0.003",
        "tree.BQvalueuj <= 0.2",
    ],
}

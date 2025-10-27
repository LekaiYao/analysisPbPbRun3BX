import ROOT
import sys
from cuts import cuts

def select(channel,input_file,output_file,cutName):
    # Open input ROOT file
    f_in = ROOT.TFile.Open(input_file)
    tree = f_in.Get("tree")
    if not tree:
        print("Error: tree not found in", input_file)
        sys.exit(1)


    # Create output file and new tree
    f_out = ROOT.TFile(output_file, "RECREATE")
    new_tree = tree.CloneTree(0)

    nEntries = tree.GetEntries()
    kept = 0

    # Get cuts for this channel
    cutKeyName = channel + "_" + cutName
    channel_cuts = cuts.get(cutKeyName, [])
    print(f"Channel = {channel}, Active cuts = {channel_cuts}")

    for i in range(nEntries):
        tree.GetEntry(i)

        # Apply all cuts safely with tree context
        if all(eval(expr, {}, {"tree": tree}) for expr in channel_cuts):
            new_tree.Fill()
            kept += 1

        # Progress bar
        if i % (nEntries // 20 + 1) == 0:
            print(f"Processed {i}/{nEntries} ({100.0*i/nEntries:.1f}%)", end="\r")

    # Save results
    new_tree.Write()
    f_out.Close()
    f_in.Close()

    print(f"\nDone! Original entries = {nEntries}, kept entries = {kept}")


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python3 select.py <model> <cutNumber> <channel>")
        print("model:0(sideband),1(cutDATA),2(cutMC),3(Xonly)")
        print("cutNumber:n")
        print("channel:0(X),1(Bu),2(Bd),3(Bs),4(PSI2S)")
        print("Example: python3 select.py 0 0 1")
        sys.exit(1)

    model = sys.argv[1]
    if model == "0":
        cutNumber = "cut" + sys.argv[2]
        cutName = "sideband_cut" + sys.argv[2]
    elif model =="1":
        cutNumber = "cut" + sys.argv[2]
        cutName = "data_cut" + sys.argv[2]
    elif model =="2":
        cutNumber = "cut" + sys.argv[2]
        cutName = "MC_cut" + sys.argv[2]
    elif model =="3":
        cutNumber = "cut" + sys.argv[2]
        cutName = "only_cut" + sys.argv[2]
    
    channel = sys.argv[3]

    # Define input/output file paths
    if (model == "0") :#sideband
        input = "DATA"
        output = "sideband"
    elif (model == "1") :#cutDATA
        input = "DATA"
        output = "DATA"
    elif (model == "2") :#cutMC
        input = "MC"
        output = "MC"
    elif (model == "3") :#X only
        input = "DATA"
        output = "DATA_only"

    if (channel == "0") :
        channel = "X"
    elif (channel == "1") :
        channel = "Bu"
    elif (channel == "2") :
        channel = "Bd"
    elif (channel == "3") :
        channel = "Bs"
    
    #input_file = f"root_files/{channel}/{input}_{channel}.root"
    input_file = f"root_files/{channel}/{input}_{channel}_cut1.root"
    output_file = f"root_files/{channel}/{output}_{channel}_{cutNumber}.root"

    if (channel == "4") :#PSI2S
        channel = "PSI2S"
        input_file = f"root_files/X/{input}_{channel}.root"
        output_file = f"root_files/X/{output}_{channel}_{cutNumber}.root"
    select(channel,input_file,output_file,cutName)

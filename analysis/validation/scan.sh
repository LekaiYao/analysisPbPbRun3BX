#!/bin/bash

# Exit if any command fails
set -e

# Check argument
if [ $# -ne 2 ]; then
    echo "Usage: $0 <channel> <mode>"
    echo "channel:0(X),1(Bu),2(Bd),3(Bs)"
    echo "mode:0(SPlot and SBS), 1(SBS), 2(SPlot)"
    exit 1
fi

channel=$1
mode=$2

# Define template_name based on mode
case $mode in
    0)
        template_name="plot_all_template.C"
        ;;
    1)
        template_name="plot_sbs_template.C"
        ;;
    2)
        template_name="plot_splot_template.C"
        ;;
    *)
        echo "Invalid mode: $mode"
        exit 1
        ;;
esac

# Loop over each variable name in input.txt
while read varname; do
    # Create a new .C file by replacing the placeholder
    script_name="plot_${varname}.C"
    sed "s/@VARNAME@/$varname/g" ${template_name} > "$script_name"

    # Run the script using ROOT in batch channel
    root -l -b -q "${script_name}(${channel})"

    rm "$script_name"
    echo "Variable: $varname with channel=$channel finished."

done < input.txt

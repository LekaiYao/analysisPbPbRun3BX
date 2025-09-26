#!/bin/bash

# Exit if any command fails
set -e

# Check argument
if [ $# -ne 1 ]; then
    echo "Usage: $0 <mode>"
    echo "mode:0(X),1(Bu),2(Bd),3(Bs)"
    exit 1
fi

mode=$1

# Loop over each variable name in input.txt
while read varname; do
    echo "Processing variable: $varname with mode=$mode"

    # Create a new .C file by replacing the placeholder
    script_name="plot_${varname}.C"
    sed "s/@VARNAME@/$varname/g" plot_template.C > "$script_name"

    # Run the script using ROOT in batch mode
    root -l -b -q "${script_name}(${mode})"

    rm "$script_name"

done < input.txt

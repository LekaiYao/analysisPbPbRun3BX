#!/bin/bash

# Exit if any command fails
set -e

# Loop over each variable name in input.txt
while read varname; do
    echo "Processing variable: $varname"

    # Create a new .C file by replacing the placeholder
    script_name="plot_${varname}.C"
    sed "s/@VARNAME@/$varname/g" plot_template.C > "$script_name"

    # Run the script using ROOT in batch mode
    root -l -b -q "$script_name"
    rm "$script_name"

done < input.txt

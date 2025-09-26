#!/bin/bash

# Exit if any command fails
set -e

# Check argument
if [ $# -ne 1 ]; then
    echo "Usage: $0 <mode>"
    echo "mode:0(X+PSI),1(Bu),2(Bd),3(Bs),4(X)"
    exit 1
fi

mode=$1

TEMPLATE="optimization_template.C"

cat input.txt | while read rows
do
	var=$(echo $rows | awk 'BEGIN{FS="/"} {print $1}')
	min=$(echo $rows | awk 'BEGIN{FS="/"} {print $2}')
	max=$(echo $rows | awk 'BEGIN{FS="/"} {print $3}')
	step=$(echo $rows | awk 'BEGIN{FS="/"} {print $4}')
    script_name="optimization_${var}.C"
    cp $TEMPLATE $script_name

    sed -i -e "s/@VAR@/${var}/g" \
        -e "s/@MIN@/${min}/g" \
        -e "s/@MAX@/${max}/g" \
        -e "s/@STEP@/${step}/g" $script_name
    echo "Running $script_name..."
    root -l -b -q "$script_name(${mode})"
    rm "$script_name"
done


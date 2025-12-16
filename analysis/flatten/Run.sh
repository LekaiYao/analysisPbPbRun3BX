#!/bin/bash

# Check argument
if [ $# -ne 2 ]; then
    echo "Usage: $0 <channel> <mode>"
    echo "channel:0(X),1(Bu),2(Bd),3(Bs),4(PSI)"
    echo "mode:0(MC),1(DATA)"
    exit 1
fi

channel=$1
mode=$2

if [ "$channel" == "0" ]; then
    channel="X"
elif [ "$channel" == "1" ]; then
    channel="Bu"
elif [ "$channel" == "2" ]; then
    channel="Bd"
elif [ "$channel" == "3" ]; then
    channel="Bs"
elif [ "$channel" == "4" ]; then
    channel="PSI"
fi
if [ "$mode" == "0" ]; then
    mode="MC"
elif [ "$mode" == "1" ]; then
    mode="DATA"
fi

echo "Channel set to: $channel, Mode set to $mode"

root -l -b -q ./runjobs/runjobs_pp${mode}_${channel}.C
    
echo "pp ${mode} ${channel} finished"


: <<'COMMENT'
line_number=1
while IFS= read -r input_path; do
    if [ -z "$input_path" ]; then
        continue
    fi
    
    cp runjobs_template_${channel}.C runjobs_${line_number}.C
    
    sed -i "s|INPUTPATH|${input_path}|g" runjobs_${line_number}.C
    sed -i "s|OUTNAME|${channel}_pbpbDATA_set${line_number}|g" runjobs_${line_number}.C
    
    root -l -b -q runjobs_${line_number}.C
    
    echo "set${line_number} finished"

    ((line_number++))
done < list_${channel}.txt
COMMENT

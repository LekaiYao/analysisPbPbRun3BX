#!/bin/bash

line_number=1
while IFS= read -r input_path; do
    # 跳过空行
    if [ -z "$input_path" ]; then
        continue
    fi
    
    # 复制模板到runjobs.C
    cp runjobs_template.C runjobs.C
    
    # 替换INPUTPATH和OUTNAME
    sed -i "s|INPUTPATH|${input_path}|g" runjobs.C
    sed -i "s|OUTNAME|set${line_number}|g" runjobs.C
    
    # 运行runjobs.C
    root -l -b -q runjobs.C
    
    # 输出完成消息
    echo "set${line_number} finished"
    
    ((line_number++))
done < list.txt
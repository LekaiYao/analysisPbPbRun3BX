#! /bin/bash

##wait to be improved
flatten(){
    :
}

##wait to be merged
scan(){
    ##scan all variables
    pushd analysis/scan
    bash scan.sh &
    wait
    popd
}

##wait to be merged
correlation(){
    ##draw correlation matrix
    pushd analysis/correlation
    python3 correlations.py both
    popd
}

##wait to be merged
selection(){
    ##cut data and MC samples
    pushd analysis/selection
    root -b -l -q select.C
    popd
}

##wait to be merged
optimization(){
    ##search for optimal cuts
    pushd analysis/optimization
    bash new_Run.sh &
    wait
    popd
}

##wait to be merged
BDT(){
    ##BDT training and applying
    pushd analysis/TMVA
    python3 -u new_TMVA_BDTs.py test1 > log/test1.log 2>&1 &
    wait
    python3 new_apply_BDT.py test1
    popd
}

##wait to be improved
yield(){
    ##fit to cut data
    :
}



#UNCOMMENT ACCORDINGLY
#(Run by THIS ORDER!)

#flatten
#scan
#correlation
selection
#optimization
#BDT
#yield
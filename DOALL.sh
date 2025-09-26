#! /bin/bash

##wait to be improved
flatten(){
    :
}

##merge finished
scan(){
    ##scan all variables
    pushd analysis/scan
    bash scan.sh 3 & #0(X),1(Bu),2(Bd),3(Bs)
    wait
    popd
}

##merge finished
correlation(){
    ##draw correlation matrix
    pushd analysis/correlation
    python3 correlations.py both 0 #0(X),1(Bu),2(Bd),3(Bs)
    popd
}

##merge finished
selection(){
    ##cut data and MC samples
    pushd analysis/selection
    python3 select.py 0 0 0 #model:0(sideband),1(cutDATA),2(cutMC);cut number:0,1,...;channel:0(X),1(Bu),2(Bd),3(Bs),4(PSI)
    popd
}

##merge finished
optimization(){
    ##search for optimal cuts
    pushd analysis/optimization
    bash Run.sh 3 & #0(X+PSI),1(Bu),2(Bd),3(Bs),4(X)
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
#selection
#optimization
#BDT

#yield
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

##wait to be improved
validation(){
    ##MC validation: SPlot, Sideband Subtraction(SBS)
    #SPlot needs fit first, this part hasn't been merged
    pushd analysis/validation
    bash scan.sh 0 0 & #channel: 0(X),1(Bu),2(Bd),3(Bs); model: SPlot+SBS(0), SBS(1), SPlot(2)
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
    #0(X),1(Bu),2(Bd),3(Bs)
    pushd analysis/TMVA
    python3 -u TMVA_BDTs.py test1 0 &
    wait
    python3 apply_BDT.py test1 0 &
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
#validation
#correlation
#selection
#optimization
#BDT
#yield
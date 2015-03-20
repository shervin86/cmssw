#!/bin/bash
eval `scramv1 runtime -sh`
source /cvmfs/cms.cern.ch/crab3/crab.sh
voms-proxy-init -voms cms -out $HOME/gpi.out
#cd calibration/SANDBOX
#setenv PATH $PWD/bin:$PATH
#PATH=`echo $PATH | sed 's|/afs/cern.ch/project/eos/installation/pro/bin/||'`
PATH=$PATH:/afs/cern.ch/project/eos/installation/pro/bin/
PATH=$PATH:$CMSSW_BASE/src/Calibration/ALCARAW_RECO/bin
export ECALELFINIT=y

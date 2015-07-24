#!/bin/bash
# - difference between 2015B IOVs and choose 1 for transportation -> make plots
# - relative IC 2015B w.r.t. GT
# - relative IC 2015B w.r.t. 2012D last IOV
# - relative IC 2012D w.r.t. 2012D last IOV

# These as to agree:
# - transportation as 2015B abs / 2012D abs
# - transportation as 2015 rel / 2012 rel using 2012D last IOV as tag in rereco in both cases

# copy phiSym files with correction in local directory
outDir=data/RunII-IC/Run2015B_WF2/phiSym/; 
#for file in  /afs/cern.ch/work/s/spigazzi/public/phisymIC_noSumEtCuts_21072015/*REL*; do echo $file; ./scripts/correctPhiSymTxt.sh $file > ${outDir}/`basename $file`; diff $file ${outDir}/`basename $file`; done 

# run the manipulations and the dumps
python bin/combine_IC_new.py

exit 0
#for file in data/run-251*.dat; do sort -n -k 2 $file | awk -f awk/pm3d.awk > data/`basename $file .dat`.pm3d; done

# make maps to be plotted with gnuplot 
dumps_2015B_IOVs=data/work/run-251*.dat
for file in ${dumps_2015B_IOVs} data/work/ic_2015B*.dat data/work/ic_2012D*.dat data/work/ic_trans*.dat
do 
	 awk -v region="EB" -f awk/listToMap.awk $file > data/work/`basename $file .dat`-EB.map; 
	 awk -v region="EE+" -f awk/listToMap.awk $file > data/work/`basename $file .dat`-EEp.map; 
	 awk -v region="EE-" -f awk/listToMap.awk $file > data/work/`basename $file .dat`-EEm.map; 
done



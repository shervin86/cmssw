#!/bin/bash

file=$1


# get list of channels with little number of recHits
echo "============================== No recHits" 
grep -v '#' $file | awk '($6==0){print $0}' 


echo "============================== Low number of  recHits" 
grep -v '#' $file | awk '($6<10){print $0}' 


echo "============================== Correction < error"
grep -v '#' $file | awk '($6>0 && sqrt($4*$4)<$5/sqrt($6)){print $0, $5/sqrt($6)}' 
# farne il plot come mappa e 1D

cat > clean.awk <<EOF
(match(\$1,"#")){
print \$0
}

(! match(\$1,"#")){
err= (\$6>0) ? \$5/sqrt(\$6) : 999
value=\$4
absValue=sqrt(value * value)
if(\$6<10 || absValue <err || value==nan || value==-nan){
print \$1,\$2,\$3,0,\$5,\$6,\$7,\$8, err, value
} else{print \$0, err, value}
}

EOF

awk -f clean.awk $file > `dirname $file`/`basename $file .dat`-cleaned.dat




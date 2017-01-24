#!/bin/bash

file=$1
shiftEB=$2
shiftEE=$3

for pedshift in 0.1_0.1 0.3_0.5 0.5_1 1_3 1.5_5 2_10 0_0
do
	shiftEB=`echo $pedshift | cut -d '_' -f 1`
	shiftEE=`echo $pedshift | cut -d '_' -f 2`
#	echo $shiftEB $shiftEE
	awk -v pedShiftEB=${shiftEB} -v pedShiftEE=${shiftEE} '(NR<=62000){printf("%d\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\n", NR, $1+pedShiftEB,$2,$3+pedShiftEB,$4,$5+pedShiftEB,$6)}; (NR>62000){printf("%d\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\n", NR, $1+pedShiftEE,$2,$3+pedShiftEE,$4,$5+pedShiftEE,$6)}' $file > `dirname $file`/`basename $file .dat`_${shiftEB}_${shiftEE}.dat

	awk -v pedShiftEB=-${shiftEB} -v pedShiftEE=-${shiftEE} '(NR<=62000){printf("%d\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\n", NR, $1+pedShiftEB,$2,$3+pedShiftEB,$4,$5+pedShiftEB,$6)}; (NR>62000){printf("%d\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\n", NR, $1+pedShiftEE,$2,$3+pedShiftEE,$4,$5+pedShiftEE,$6)}' $file > `dirname $file`/`basename $file .dat`_-${shiftEB}_-${shiftEE}.dat
done

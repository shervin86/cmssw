#!/bin/bash

fileIn=$1
fileOut=$2


awk -f awk/correctPhiSymTxt.awk  ${fileIn} #> ${fileOut}

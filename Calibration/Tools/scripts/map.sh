#!/bin/bash
REGION=$2
FILE=$1

awk -v region="${REGION}" -f awk/listToMap.awk ${FILE}

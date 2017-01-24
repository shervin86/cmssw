#!/bin/bash
file=$1
 awk '(/<item[ >]/){print line; line=""};(/mean/ || /rms/){line=sprintf("%s\t%s", line, $0)}; END{print line}' $file | sed '/^$/ d;s|<[/[:alnum:]-]*>||g;s|[[:space:]*]| |g'

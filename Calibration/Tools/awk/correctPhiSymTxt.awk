(NF!=0){
	#correct for errors in error if no hits
	line=$0
	if( $9==0 && $5=="nan")
		line=sprintf("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s", $1, $2, $3, -1, 999, 999, $7, $8, $9, $10)
	
	#check the number of hits
	
	# check k-factor
	if($10 <0)
		line=sprintf("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s", $1, $2, $3, -1, 999, 999, $7, $8, $9, $10)


	if($9<50)
		line=sprintf("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s", $1, $2, $3, -1, 999, 999, $7, $8, $9, $10)

	if($5<0)
		line=sprintf("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s", $1, $2, $3, -1, 999, 999, $7, $8, $9, $10)

	print line
}

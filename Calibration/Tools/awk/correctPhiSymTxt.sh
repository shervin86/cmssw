(NF!=0){
	#correct for errors in error 
	if($9==0 && $5=="nan"){ 
			print $1, $2, $3, -1, 999, 999, $7, $8, $9, $10
		} else print $0
}

#each time the x axis changes, it adds a blank line
($3==0 && $1!=old){
	old=$1; 
	printf("\n")
};

($3!=0 && $2!=old){
	old=$2;
	printf("\n")
};
(($3!=0 && $2==old) ||($3==0 && $1==old)){
	print $0
}

#assume three column file 
# x y z
# to be transformed into an image
(NF!=0){
     if(	(region=="EB" && $3==0) || (region=="EE+" && $3==1) || (region=="EE-" && $3==-1) || (region=="EE" && $3!=0)){ 
	  z[$1,$2]=$4;
     }
}

END{
	x_min=1e20
	x_max=-1e20
	y_min=1e20
	y_max=-1e20

	for( coord in z){
		split(coord, axis, SUBSEP);
		x=axis[1];
		y=axis[2];
		#z=axis[3];
		
		if( x < x_min)	x_min=x;
		if( x > x_max)  x_max=x;
		if( y < y_min)	y_min=y;
		if( y > y_max)  y_max=y;
	}


	N = (y_max-y_min)*(x_max-x_min);
	# first row should contain the y coordinates for nonuniform matrix plotting in gnuplot
	printf("%d\t", N);
	for(x = x_min; x<=x_max; x++){
		printf("%.5f\t", x);
	}
	printf("\n");

	for(y = y_min; y<=y_max; y++){
		printf("%.5f\t", y);		
		for(x=x_min; x<=x_max; x++){
			printf("%.5f\t", z[x,y]);
		}
		printf("\n");
	}
}	

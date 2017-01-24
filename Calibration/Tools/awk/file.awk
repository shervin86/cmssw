($8==1 && ! match($9, "nan")){
    
     indexx=$2; iRing=$5; 
     sum[indexx,iRing]+=$9; num[indexx,iRing]+=1;
} 

END{ 
     for(indexx = 0; indexx<7; indexx++){ 
	  for(iRing=0; iRing<40; iRing++){ 
	       if(num[indexx,iRing]!=0) 	       print indexx, iRing, sum[indexx,iRing]/num[indexx,iRing]; 
	  }
	  print ""
	  print ""

     } 
}

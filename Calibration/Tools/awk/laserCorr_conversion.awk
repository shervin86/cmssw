#60 100 1 1.28879 0.0000035 0.0000035 0.0000000 1 12959 2.65084552765

# The format is simple :
# 1) Fed number
# 2) channel number = (CCU-1)*25+(vfe number -1)*5 +  channel number
# 3)  EB : iphi(1..360), EE : ix(-50..-1, 1..50)
# 4 ) EB : iz*ieta(-85..-1, 1..85), EE iz*iy(-100..-1, 1..100)
# 5) Field correction
#
# FED 601-609 : EE-
# FED 610-627 : EB-
# FED 628-645 : EB+
# FED 646-654 : EE+
#

(NF!=0 && !/#/){
	if ($1>609 && $1 < 646){ # EB
			ix=$3;
		iy=$4;
		iz=0
	}else{
		iz= ($1<=609) ? -1 : 1;
		ix= ($3<0) ? $3+51 : $3+50
		iy= $4*iz
	}

	print ix, iy, iz, $5, 0
}


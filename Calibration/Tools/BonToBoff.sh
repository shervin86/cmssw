#!/bin/bash
# This script is meant to convert conditions for 3.8T to conditions for 0T data taking, using the most up-to-date conditions at 3.8T

dir=BonBoff
if [ ! -d "$dir" ];then
	mkdir $dir
fi


# uses the most up-to-date GT to find the most up-to-date ECAL conditions
GT=76X_dataRun2_v15 # GT used for Winter15 rereco
echo "GT is: $GT"

# verify that conddb command is defined
which conddb > /dev/null || {
	echo "[ERROR] conddb command is not defined" >> /dev/stderr
	exit 1

}

###############################
# get the name of the IC tag
ICtag=`conddb list $GT | grep EcalIntercalibConstantsRcd | awk '{print $3}'`
echo "IC tag in GT is: $ICtag"


line=`conddb list $ICtag | sed '/^$/ d' | tail -1`
echo "Last payload is:"
echo "$line"
ICpayload=`echo $line | awk '{print $4}'`

#echo $ICpayload
conddb dump $ICpayload > $dir/EcalIntercalibConstants_Bon.xml

########################
###############################
# get the name of the Alpha tag
Alphatag=`conddb list $GT | grep EcalLaserAlphas | awk '{print $3}'`
echo "Alpha tag in GT is: $Alphatag"


line=`conddb list $Alphatag | sed '/^$/ d' | tail -1`
echo "Last payload is:"
echo "$line"
Alphapayload=`echo $line | awk '{print $4}'`

#echo $Alphapayload
conddb dump $Alphapayload > $dir/EcalLaserAlphas_Bon.xml

###############################
# get the name of the ADCtoGeV tag
ADCtoGeVtag=`conddb list $GT | grep EcalADCToGeVConstantRcd | awk '{print $3}'`
echo "ADCtoGeV tag in GT is: $ADCtoGeVtag"


line=`conddb list $ADCtoGeVtag | sed '/^$/ d' | tail -1`
echo "Last payload is:"
echo "$line"
ADCtoGeVpayload=`echo $line | awk '{print $4}'`

#echo $ADCtoGeVpayload
conddb dump $ADCtoGeVpayload > $dir/EcalADCtoGeV_Bon.xml

cat > $dir/BonBoff.py <<EOF
from ROOT import *
gSystem.Load("libFWCoreFWLite.so")
AutoLibraryLoader.enable()
gSystem.Load("libCalibrationTools.so")

all = DSAll()
eb = DSIsBarrel()
ee  = DSIsEndcap()

rings = DRings()
rings.setEERings("./data/eerings.dat")


dir = "BonBoff"
outDir = dir

icBon = IC()
icBon.setRings(rings)
icBon.readXMLFile( dir + '/EcalIntercalibConstants_Bon.xml')

alpha = IC()
alpha.setRings(rings)
alpha.readXMLFile(dir + '/EcalLaserAlphas_Bon.xml')


Bcorr = IC()
Bcorr.setRings(rings)
Bcorr.readTextFile('Boffon_2015-sorted.dat')


icBon.BonToBoff(Bcorr, alpha, ee) # apply corrections only to EE
kEB = icBon.Normalize(eb)
kEE = icBon.Normalize(ee)
icBon.dump(dir + '/EcalIntercalibConstants_Boff.dat', all)
IC.dumpXML(icBon, dir + '/EcalIntercalibConstants_Boff.xml', all, False)

# ic_new = IC()
# ic_new.setRings(rings)



# ic_new.readTextFile('ic_new.dat')
# ic_new.Normalize(eb);
# ic_new.Normalize(ee);
# ic_new.dump('ic_new-norm.dump',all)

# IC.dumpXML(ic_new, 'ic_new.xml', all, False)



print "EB=", kEB
print "EE=", kEE


EOF

python $dir/BonBoff.py | grep '=' > $dir/ADCcorr.dat




EB=`grep value $dir/EcalADCtoGeV_Bon.xml | sed 's|<E[BE]value->\([0-9.e-]*\)</E[BE]value->|\1|' | sed '2 d'`
EE=`grep value $dir/EcalADCtoGeV_Bon.xml | sed 's|<E[BE]value->\([0-9.e-]*\)</E[BE]value->|\1|' | sed '1 d'`
kEB=`grep EB $dir/ADCcorr.dat | cut -d '=' -f 2`
kEE=`grep EE $dir/ADCcorr.dat | cut -d '=' -f 2`

EBnew=`echo $EB $kEB | awk '{print $1*$2}'` 
EEnew=`echo $EE $kEE | awk '{print $1*$2}'`

echo $EB $EBnew $kEB
echo $EE $EEnew $kEE

cat > $dir/EcalADCtoGeV_Boff.xml <<EOF
<EcalADCToGeVConstant>
 <EcalCondHeader>
    <method></method>
    <version></version>
    <datasource></datasource>
    <since>0</since>
    <tag></tag>
    <date></date>
  </EcalCondHeader>

  <BarrelValue>$EBnew</BarrelValue>
  <EndcapValue>$EEnew</EndcapValue>

</EcalADCToGeVConstant>

EOF
cat $dir/EcalADCtoGeV_Boff.xml

echo "
cmsRun xmlToSqlite_cfg.py tag=EcalADCToGeVConstant_2015_Boff fileName=/afs/cern.ch/user/s/shervin/scratch1/ICcomb/CMSSW_7_4_6_patch6/src/Calibration/Tools/BonBoff/EcalADCtoGeV_Boff.xml record=EcalADCToGeVConstantRcd
"

exit 0
1556  conddb dump 3fb14d460c8deb01747bb904b62208bbf1431b0e > EcalTimeCalibConstants_v01_express-253984.dump
 1558  conddb dump 3fb14d460c8deb01747bb904b62208bbf1431b0e > /tmp/shervin/EcalTimeCalibConstants_v01_express-253984.dump
 1830  conddb dump EcalIntercalibConstants_Run1_Run2_V01_offline 
 1831  conddb dump EcalIntercalibConstants_Run1_Run2_V01_offline > EcalIntercalibConstants_Run1_Run2_V01_offline.dump
 1899  conddb dump EcalIntercalibConstants_Run1_Run2_V01_offline  > EcalIntercalibConstants_Run1_Run2_V01_offline.dump 
 1903  conddb dump 92e1a22958e84b0622ce1b81b5bc695add9183a6 > EcalIntercalibConstants_Run1_Run2_V01_offline.dump 
 1915  conddb dump 9a4cff24a815df56453a822aec1d08e236944723 > EcalLaserAlphas_EB_sic1_btcp152_EE_sic1_btcp116.dump
 1961  conddb dump 35bf46d41ea5012c732c499825340aafc3ab0d29 > EcalADCToGeVConstant_2015_Bon.xml
 1978  conddb dump 35bf46d41ea5012c732c499825340aafc3ab0d29 > EcalADCToGeVConstant_2015_Bon.xml

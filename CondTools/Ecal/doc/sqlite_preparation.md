# How to produce an xml file starting from a txt file of the form
```
ix iy iz IC ICerror
```

use the script in 
`/afs/cern.ch/cms/CAF/CMSALCA/ALCA_ECALCALIB/RunII-IC/txtToXml.sh`

# How to create the sqlite file from the xml file
cmsRun test/testEcalIntercalibConstants.py tag=EcalIntercalibConstants_Cal_Nov2015_EoP_v7 fileName=yourFile.xml 

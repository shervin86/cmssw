from ROOT import *
gSystem.Load("libFWCoreFWLite.so")
AutoLibraryLoader.enable()
gSystem.Load("libCalibrationTools.so")

############################## selectors
all = DSAll()
eb = DSIsBarrel()
eep = DSIsEndcapPlus()
eem = DSIsEndcapMinus()

rings = DRings()
rings.setEERings("./data/eerings.dat")


dir = "data/RunII-IC/Run2015B_WF2/" 
outDir = "data/work2/"

#print "Reading IC 2012 abs"
ic_2012D = IC()
ic_2012D.setRings(rings)

ic_2012D.PrintInfos()

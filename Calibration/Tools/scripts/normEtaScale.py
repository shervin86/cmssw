from ROOT import *
gSystem.Load("libFWCoreFWLite.so")
AutoLibraryLoader.enable()
gSystem.Load("libCalibrationTools.so")

all = DSAll()
eb = DSIsBarrel()
eep = DSIsEndcapPlus()
eem = DSIsEndcapMinus()

rings = DRings()
rings.setEERings("./data/eerings.dat")

import sys

ic = IC()
ic.setRings(rings)
IC.readTextFile(sys.argv[1], ic)
IC.scaleEta(ic, ic, True)
IC.dump(ic, sys.argv[2], all)


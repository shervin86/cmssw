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


dir = "data/RunII-IC/Run2015B_WF2/" 
outDir = "data/work/"

file_2012D  = dir + "/EcalIntercalibConstants_2012ABCD_offline-IOV_00208296_inf-bis.dat"
ic_2012D = IC()
ic_2012D.setRings(rings)
ic_2012D.readTextFile(       file_2012D       )


#file_2015_GT = dir + "/EcalIntercalibConstants_V1_express-IOV_251004_252036-bis.txt"
file_2015_GT = file_2012D

ic_2015GT = IC()
ic_2015GT.setRings(rings)
ic_2015GT.readTextFile(      file_2015_GT     )

file_2012DphiSym_rel = dir + "phiSym/2012D_208538_208686_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat"
ic_2012DphiSym_rel = IC()
ic_2012DphiSym_rel.setRings(rings)
ic_2012DphiSym_rel.readTextFile(file_2012DphiSym_rel)
#ic_2012DphiSym_rel.dump('test2.dat', all)
#ic_2012DphiSym_rel.unscaleEta()

ic_2012DphiSym_abs = ic_2012DphiSym_rel * ic_2012D
#ic_2012DphiSym_rel.dump('test.dat', all)


file_2015BphiSym_rel2 = dir + "/phiSym/2015B_251252_251252_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat"
file_2015BphiSym_rel = dir + "/phiSym/2015B_251562_251562_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat"
ic_2015BphiSym_rel = IC()
ic_2015BphiSym_rel2 = IC()
ic_2015BphiSym_rel.setRings(rings)
ic_2015BphiSym_rel2.setRings(rings)
ic_2015BphiSym_rel.readTextFile( file_2015BphiSym_rel )
ic_2015BphiSym_rel2.readTextFile( file_2015BphiSym_rel2 )
#ic_2015BphiSym_rel.unscaleEta() # normalize all the new ICs to have etaScale=1

ic_2015BphiSym_abs = ic_2015BphiSym_rel * ic_2015GT
ic_2015BphiSym_abs2 = ic_2015BphiSym_rel2 * ic_2015GT
#ic_2015BphiSym_abs.unscaleEta()

#################### Check if phiSym absolute IC are done with the right reference tag
ic_2015BphiSym_abs_check = IC()
ic_2015BphiSym_abs_check.setRings(rings)
file_2015BphiSym_abs_check = dir + "/phiSym/2015B_251252_251252_newThr_74X_dataRun2_Prompt_v0_ABS_kCh_22072015.dat"
ic_2015BphiSym_abs_check.readTextFile( file_2015BphiSym_abs_check )

#if( not (ic_2015BphiSym_abs_check / ic_2015BphiSym_abs).isUnit()):
#    print "[ERROR] phiSym is not using the right reference tag to calculate the absolute values"
#    (ic_2015BphiSym_abs_check / ic_2015BphiSym_abs).dump('data/icPhiSym_abs_check_ratio.dat', all)

# _unscaledEta = with average over the ring =1
# with ref 251562
#WF2_2015_ICtransp2012_v1: 2012* (2015prompt * phiSym2015/(2012*phiSym2012))
#WF2_2015_ICtransp2012_v2: 2012* (2015prompt * phiSym2015/(2012*phiSym2012))_unscaledEta
#WF2_2015_ICtransp2012_v3: 2012* ( (2015prompt * phiSym2015)_unscaledEta / (2012*phiSym2012)_unscaledEta )
#WF2_2015_ICtransp2012_v4: 2012* ( (2015prompt * phiSym2015)_unscaledEta / (2012*phiSym2012)_unscaledEta )_without etaScale

# with ref 251252
#WF2_2015_ICtransp2012_v5: 2012* (2015prompt * phiSym2015/(2012*phiSym2012))
#WF2_2015_ICtransp2012_v6: 2012* (2015prompt * phiSym2015/(2012*phiSym2012))_without etaScale
#WF2_2015_ICtransp2012_v7: 2012* ( (2015prompt * phiSym2015)_unscaledEta / (2012*phiSym2012)_unscaledEta )
#WF2_2015_ICtransp2012_v8: 2012* ( (2015prompt * phiSym2015)_unscaledEta / (2012*phiSym2012)_unscaledEta )_without etaScale

# with ref 251562
#WF2_2015_ICtransp2012_v9:  2012* (phiSym2015/phiSym2012))
#WF2_2015_ICtransp2012_v10: 2012* (phiSym2015/phiSym2012))_unscaledEta



# pi0

#ic_transport_2012_2015_ref = IC()
#ic_transport_2012_2015_ref.setRings(rings)

ic_2015BphiSym_abs_noEta = ic_2015BphiSym_abs
ic_2015BphiSym_abs_noEta.unscaleEta()
ic_2012DphiSym_abs_noEta = ic_2012DphiSym_abs
ic_2012DphiSym_abs_noEta.unscaleEta()
ic_2015BphiSym_abs2_noEta = ic_2015BphiSym_abs2
ic_2015BphiSym_abs2_noEta.unscaleEta()


ic_transport_2012_2015_ref = (ic_2015BphiSym_abs/ic_2012DphiSym_abs) 
ic_transport_2012_2015_ref_rel = ic_2015BphiSym_rel/ic_2012DphiSym_rel
ic_transport_check = ic_transport_2012_2015_ref_rel / ic_transport_2012_2015_ref
#ic_transport_check.removeOutliers()
ic_transport_check.dump(outDir + 'ic_transport_check.dat', all, False)

ic_transport_2012_2015_ref.dumpOutliers(ic_transport_2012_2015_ref, 0.1, 3)

ic_transport_2012_2015_ref2 = (ic_2015BphiSym_abs2/ic_2012DphiSym_abs) 
ic_transport_2012_2015_absnoEta_ref=ic_2015BphiSym_abs_noEta/ic_2012DphiSym_abs_noEta
ic_transport_2012_2015_absnoEta_ref2=ic_2015BphiSym_abs2_noEta/ic_2012DphiSym_abs_noEta

print "Dumping"
ic_transport_2012_2015_ref.dump(outDir+ 'ic_transport_2012_2015_ref.dat', all)
ic_transport_2012_2015_ref_rel.dump(outDir + 'ic_transport_2012_2015_ref_rel.dat' , all)
ic_transport_2012_2015_ref2.dump(outDir+ 'ic_transport_2012_2015_ref2.dat', all)
ic_transport_2012_2015_absnoEta_ref.dump(outDir+ 'ic_transport_2012_2015_absnoEta_ref.dat', all)
ic_transport_2012_2015_absnoEta_ref2.dump(outDir+ 'ic_transport_2012_2015_absnoEta_ref2.dat', all)

WF2_2015_ICtransp2012_v1 = ic_2012DphiSym_abs * ic_transport_2012_2015_ref

ic_transport_2012_2015_ref.unscaleEta()
#IC.scaleEta(ic_transport_2012_2015, ic_transport_2012_2015, True)
ic_transport_2012_2015_ref.dump(outDir+'ic_transport_2012_2015_ref-unscaled.dat', all)
WF2_2015_ICtransp2012_v2 = ic_2012DphiSym_abs * ic_transport_2012_2015_ref

WF2_2015_ICtransp2012_v3 = ic_2012DphiSym_abs * ic_transport_2012_2015_absnoEta_ref
WF2_2015_ICtransp2012_v4 = WF2_2015_ICtransp2012_v3
WF2_2015_ICtransp2012_v4.unscaleEta()

WF2_2015_ICtransp2012_v5 = ic_2012DphiSym_abs * ic_transport_2012_2015_ref2

ic_transport_2012_2015_ref2.unscaleEta()
ic_transport_2012_2015_ref2.dump(outDir+'ic_transport_2012_2015_ref2-unscaled.dat', all)
WF2_2015_ICtransp2012_v6 = ic_2012DphiSym_abs * ic_transport_2012_2015_ref2


WF2_2015_ICtransp2012_v7 = ic_2012DphiSym_abs * ic_transport_2012_2015_absnoEta_ref2
WF2_2015_ICtransp2012_v8 = WF2_2015_ICtransp2012_v7
WF2_2015_ICtransp2012_v8.unscaleEta()

WF2_2015_ICtransp2012_v9 = ic_2012DphiSym_abs * ic_transport_2012_2015_ref_rel
ic_transport_2012_2015_ref_rel_noEta = ic_transport_2012_2015_ref_rel
ic_transport_2012_2015_ref_rel_noEta.unscaleEta()
WF2_2015_ICtransp2012_v10 = ic_2012DphiSym_abs * ic_transport_2012_2015_ref_rel_noEta
ic_transport_2012_2015_ref_rel_noEta.dump(outDir + "ic_transport_2012_2015_ref_rel_noEta.dat", all)


WF2_2015_ICtransp2012_v1.dump(outDir + 'WF2_2015_ICtransp2012_v1.dat', all)
WF2_2015_ICtransp2012_v2.dump(outDir + 'WF2_2015_ICtransp2012_v2.dat', all)
WF2_2015_ICtransp2012_v3.dump(outDir + 'WF2_2015_ICtransp2012_v3.dat', all)
WF2_2015_ICtransp2012_v4.dump(outDir + 'WF2_2015_ICtransp2012_v4.dat', all)
WF2_2015_ICtransp2012_v5.dump(outDir + 'WF2_2015_ICtransp2012_v5.dat', all)
WF2_2015_ICtransp2012_v6.dump(outDir + 'WF2_2015_ICtransp2012_v6.dat', all)
WF2_2015_ICtransp2012_v7.dump(outDir + 'WF2_2015_ICtransp2012_v7.dat', all)
WF2_2015_ICtransp2012_v8.dump(outDir + 'WF2_2015_ICtransp2012_v8.dat', all)
WF2_2015_ICtransp2012_v9.dump(outDir + 'WF2_2015_ICtransp2012_v9.dat', all)
WF2_2015_ICtransp2012_v10.dump(outDir + 'WF2_2015_ICtransp2012_v10.dat', all)



IC.dumpXML(WF2_2015_ICtransp2012_v1, outDir + "WF2_2015_ICtransp2012_v1.xml", all, False)
IC.dumpXML(WF2_2015_ICtransp2012_v2, outDir + "WF2_2015_ICtransp2012_v2.xml", all, False)
IC.dumpXML(WF2_2015_ICtransp2012_v3, outDir + "WF2_2015_ICtransp2012_v3.xml", all, False)
IC.dumpXML(WF2_2015_ICtransp2012_v4, outDir + "WF2_2015_ICtransp2012_v4.xml", all, False)
IC.dumpXML(WF2_2015_ICtransp2012_v5, outDir + "WF2_2015_ICtransp2012_v5.xml", all, False)
IC.dumpXML(WF2_2015_ICtransp2012_v6, outDir + "WF2_2015_ICtransp2012_v6.xml", all, False)
IC.dumpXML(WF2_2015_ICtransp2012_v7, outDir + "WF2_2015_ICtransp2012_v7.xml", all, False)
IC.dumpXML(WF2_2015_ICtransp2012_v8, outDir + "WF2_2015_ICtransp2012_v8.xml", all, False)
IC.dumpXML(WF2_2015_ICtransp2012_v9, outDir + "WF2_2015_ICtransp2012_v9.xml", all, False)
IC.dumpXML(WF2_2015_ICtransp2012_v10, outDir + "WF2_2015_ICtransp2012_v10.xml", all, False)




ic_2015BpiZero_rel = IC()
ic_2015BpiZero_rel.setRings(rings)

ic_2015BpiZero_rel.dump(outDir + "ic_2015BpiZero_rel.dat", all)

ic_2015BpiZero_abs = ic_2015GT * ic_2015BpiZero_rel
ic_2015BpiZero_abs.dump(outDir + "ic_2015BpiZero_abs.dat", all)

ic_2015B_piZero_over_phiSym_rel = ic_2015BphiSym_rel / ic_2015BpiZero_rel
ic_2015B_piZero_over_phiSym_rel.dump(outDir + "ic_2015B_piZero_over_phiSym_rel.dat", all)

ic_2015B_piZero_0T_rel = IC()
ic_2015B_piZero_0T_rel.setRings(rings)
ic_2015B_piZero_0T_rel.dump(outDir + "ic_2015B_piZero_0T_rel.dat", all)




                   
'''
#################### Define the IOV to be used for comparison with 2012D:
ic_2015BphiSym_rel2 = IC(rings)
ic_2015BphiSym_rel2.readTextFile(dir+'/phiSym/2015B_251562_251562_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat')
ic_2015BphiSym_rel2.unscaleEta()

ic_2015BphiSym_abs2 = ic_2015BphiSym_rel2*ic_2015GT

ic_2015BphiSym_abs.dump(outDir + "/ic_2015BphiSym_abs.dat", all)
ic_2015BphiSym_rel.dump(outDir + "/ic_2015BphiSym_rel.dat", all)

ic_2015BphiSym_abs2.dump(outDir + "/ic_2015BphiSym_abs2.dat", all)
ic_2015BphiSym_rel2.dump(outDir + "/ic_2015BphiSym_rel2.dat", all)

ic_2012DphiSym_abs.dump(outDir + "/ic_2012DphiSym_abs.dat", all)
ic_2012DphiSym_rel.dump(outDir + "/ic_2012DphiSym_rel.dat", all)

ic_transport_2012_2015.dump(outDir + "/ic_transport_2012_2015.dat", all)
'''

'''
### Removing bugs
for file in  /afs/cern.ch/work/s/spigazzi/public/phisymIC_noSumEtCuts_22072015/*ABS*; do echo $file; awk '{if($9==0 && $5=="nan"){ print $1, $2, $3, -1, 999, 999, $7, $8, $9, $10} else print $0}' $file > data/RunII-IC/Run2015B_WF2/phiSym/`basename $file`; done
'''


''' 
### Plotting
p [0:360][-85:85][0:*] '< scripts/map.sh data/run-251251.dat EB'  nonuniform matrix using 1:2 with image
 awk '($1!=old){old=$1; printf("\n")};($1==old){print $0}' > data/run-251251.pm3d
sp [0:360][-85:85][0:10]'data/run-251251.pm3d' u 2:1:($3==0 ? $4 : 1/0) with pm3d notitle
'''

'''

files_IOVs_2015BphiSym_rel = [
    dir+'/phiSym/2015B_251244_251244_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat',
    dir+'/phiSym/2015B_251251_251251_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat',
    dir+'/phiSym/2015B_251252_251252_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat',
    dir+'/phiSym/2015B_251521_251521_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat',
    dir+'/phiSym/2015B_251522_251522_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat',
    dir+'/phiSym/2015B_251548_251548_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat',
    dir+'/phiSym/2015B_251559_251559_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat',
    dir+'/phiSym/2015B_251560_251560_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat',
    dir+'/phiSym/2015B_251561_251561_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat',
    dir+'/phiSym/2015B_251562_251562_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat'
    ]

runs_IOVS_2015BphiSym_rel = [
    '251244',
    '251251',
    '251252',
    '251521',
    '251522',
    '251548',
    '251559',
    '251560',
    '251561',
    '251562'
]


#################### Check stability of phiSym over several IOVs
i=0

for file in files_IOVs_2015BphiSym_rel:
#    print runs_IOVS_2015BphiSym_abs[i], file

    mic = IC(rings)
    mic.readTextFile(file)
    mic.unscaleEta()
    
    mic_rel=mic/ic_2015BphiSym_rel
    mic_rel.dump(outDir + '/run-'+runs_IOVS_2015BphiSym_rel[i]+'.dat', all)
    i+=1
    #

'''
'''
#################### Define the IOV to be used for comparison with 2012D:
ic_2015BphiSym_rel_ref = IC(rings)
ic_2015BphiSym_rel_ref.readTextFile(dir+'/phiSym/2015B_251252_251252_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat')
ic_2015BphiSym_rel_ref2.readTextFile(dir+'/phiSym/2015B_251252_251562_newThr_74X_dataRun2_Prompt_v0_REL_kCh_22072015.dat')
ic_2015BphiSym_rel_ref.unscaleEta()
ic_2015BphiSym_rel_ref2.unscaleEta()
ic_2015BphiSym_abs_ref = ic_2015BphiSym_rel_ref*ic_2015GT
ic_2015BphiSym_abs_ref2 = ic_2015BphiSym_rel_ref2*ic_2015GT

ic_2015BphiSym_abs_ref.dump(outDir + "/ic_2015BphiSym_abs_ref.dat", all)
ic_2015BphiSym_rel_ref.dump(outDir + "/ic_2015BphiSym_rel_ref.dat", all)

ic_2015BphiSym_abs_ref2.dump(outDir + "/ic_2015BphiSym_abs_ref2.dat", all)
ic_2015BphiSym_rel_ref2.dump(outDir + "/ic_2015BphiSym_rel_ref2.dat", all)




# 
# nel GT 2015B il tag di IC e' diverso rispetto a quello di 2012D, quindi le etaScale sono diverse
# per fare il confronto 2012D/2015D bisogna rigirare tutto il 2015D con il giusto tag di IC oppure confrontare avendo normalizzato le etaScale

'''

'''
icGiulia = IC()
icGiulia.readTextFile(fileGiulia, icGiulia)
#IC.dump(icGiulia, dir+'/icGiulia.txt', all)

icSpigazzi = IC()
IC.readTextFile(fileSpigazziRing, icSpigazzi)


ic_scale = IC()
res = IC()
res.setRings(rings)
icdiff = IC()

check = IC()

icSpigazziChannel = IC()
IC.readTextFile(fileSpigazziChannel, icSpigazziChannel)

icGiulia.scaleEta(icGiulia, icGiulia, True)
icSpigazzi.dump(icSpigazzi, 'tmp/icSpigazzi-noNorm.dat', all)
icSpigazzi.scaleEta(icSpigazzi, icSpigazzi, True)
icSpigazzi.scaleEta(icSpigazziChannel, icSpigazziChannel, True)

icGiulia.dump(icGiulia, 'tmp/icGiulia-norm.dat', all)
icSpigazzi.dump(icSpigazzi, 'tmp/icSpigazzi-norm.dat', all)
icSpigazziChannel.dump(icSpigazziChannel, 'tmp/icSpigazziChannel-norm.dat', all)

icRatio = IC()
icRecipr = IC()
IC.reciprocal(icSpigazzi, icRecipr)
IC.multiply(icGiulia, icRecipr, icRatio)
IC.dumpEtaScale(icRatio, "tmp/icRatio-etaScale.dat", False) # print etaRing and scale

icRatioSpigazzi = IC()
IC.multiply(icSpigazziChannel, icRecipr, icRatioSpigazzi)
IC.dumpEtaScale(icRatioSpigazzi, "tmp/icRatioSpigazzi-etaScale.dat", False)
IC.dump(icRatioSpigazzi, "tmp/icRatioSpigazzi.dat", all)


IC.readTextFile(file1, ic1)
IC.readTextFile(file2, ic2)
#IC.readTextFile(file3, ic3)

IC.readTextFile(filec, check)
IC.readTextFile(file_scale, ic_scale)

IC.scaleEta(check, check, True)
IC.scaleEta(check, ic_scale)
IC.dump(check, "phipiz.txt", all)

p = TProfile("p", "p", 800, -200, 200)

fout = TFile("combo_histos.root", "recreate")

p1 = p.Clone("p1")
p2 = p.Clone("p2")
pr = p.Clone("pr")
ps = p.Clone("ps")
pc = p.Clone("pc")
pd = p.Clone("pd")

IC.profileEta(ic1, p1, all)
IC.profileEta(ic2, p2, all)

IC.scaleEta(ic1, ic1, True)
IC.scaleEta(ic1, ic_scale)

IC.scaleEta(ic2, ic2, True)
IC.scaleEta(ic2, ic_scale)

IC.combine(ic1, ic2, res)
IC.scaleEta(res, res, True)
IC.scaleEta(res, ic_scale)

print "overall average:", IC.average(res, all)
print "     EB average:", IC.average(res, eb)
print "    EE+ average:", IC.average(res, eep)
print "    EE- average:", IC.average(res, eem)

IC.profileEta(res, pr, all)
IC.profileEta(ic_scale, ps, all)
IC.profileEta(check, pc, all)

IC.dump(res, "combined.txt", all)

IC.multiply(res, -1, res)
IC.add(res, check, icdiff)
IC.profileEta(icdiff, pd, all)
h = TH1F("h", "h", 1000, -.1, .1)
IC.constantDistribution(icdiff, h, all)

fout.Write()
#fout.Close()
'''

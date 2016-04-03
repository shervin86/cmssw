
import FWCore.ParameterSet.Config as cms

from PhysicsTools.TagAndProbe.treeMakerOptionsHLT_cfi import *
from PhysicsTools.TagAndProbe.treeContent_cfi import *
from PhysicsTools.TagAndProbe.electronIDModulesHLT_cfi import *

def myFunc(process):
    isMC = False
    options = {}
        
    options['HLTProcessName']          = "HLT"
    options['ELECTRON_COLL']           = "slimmedElectrons"
    options['ELECTRON_CUTS']           = "(abs(eta)<2.5) && (ecalEnergy*sin(superClusterPosition.theta)>10.0)"
    options['ELECTRON_TAG_CUTS']       = "(abs(eta)<=2.5) && !(1.4442<=abs(eta)<=1.566) && pt >= 25.0"
    options['SUPERCLUSTER_COLL']       = "reducedEgamma:reducedSuperClusters"
    options['SUPERCLUSTER_CUTS']       = "abs(eta)<2.5 && !(1.4442< abs(eta) <1.566) && et>10.0"
    options['useAOD']                  = cms.bool(False)
    options['OUTPUTEDMFILENAME']       = 'edmFile.root'
    options['DEBUG']                   = cms.bool(False)

    if (isMC):
        options['INPUT_FILE_NAME']     = "/store/relval/CMSSW_8_0_0/RelValZEE_13/MINIAODSIM/PU25ns_80X_mcRun2_asymptotic_v4-v1/10000/44A7587D-D1DA-E511-ADF4-0CC47A4C8ECA.root"
        options['OUTPUT_FILE_NAME']    = "TnPTree_mc.root"
        options['TnPPATHS']            = cms.vstring("HLT_Ele23_WP75_Gsf_v*")
        options['TnPHLTTagFilters']    = cms.vstring("hltEle23WP75GsfTrackIsoFilter")
        options['TnPHLTProbeFilters']  = cms.vstring()
        options['HLTFILTERTOMEASURE']  = cms.vstring("")
        options['EVENTSToPROCESS']     = cms.untracked.VEventRange()
    else:
        options['INPUT_FILE_NAME']     = "/store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/244/00000/12EE24E2-8F27-E511-80D1-02163E013793.root"
        options['OUTPUT_FILE_NAME']    = "TnPTree_data.root"
        options['TnPPATHS']            = ["",]
        options['TnPHLTTagFilters']    = ["hltEle23WPLooseGsfTrackIsoFilter"]
        options['TnPHLTProbeFilters']  = cms.vstring()
        options['HLTFILTERTOMEASURE']  = cms.vstring("")
        options['EVENTSToPROCESS']     = cms.untracked.VEventRange()
        
    ###################################################################
        
    setModules(process, options)
        
    ###################################################################
    ## ID
    ###################################################################
        
    setIDs(process, options)
        
    ###################################################################
    ## SEQUENCES
    ###################################################################
        
    process.egmGsfElectronIDs.physicsObjectSrc = cms.InputTag(options['ELECTRON_COLL'])
    process.ele_sequence = cms.Sequence(
        process.goodElectrons +
        process.egmGsfElectronIDSequence +
        process.goodElectronsPROBECutBasedVeto +
        process.goodElectronsPROBECutBasedLoose +
        process.goodElectronsPROBECutBasedMedium +
        process.goodElectronsPROBECutBasedTight +
        process.goodElectronsTAGCutBasedVeto +
        process.goodElectronsTAGCutBasedLoose +
        process.goodElectronsTAGCutBasedMedium +
        process.goodElectronsTAGCutBasedTight +
        process.goodElectronsTagHLT +
        process.goodElectronsProbeHLT 
        )
        
    ###################################################################
    ## TnP PAIRS
    ###################################################################
        
    process.allTagsAndProbes = cms.Sequence(process.tagTightRECO)
    process.mc_sequence = cms.Sequence()
         
    ##########################################################################
    ## TREE MAKER OPTIONS
    ##########################################################################
    if (not isMC):
        mcTruthCommonStuff = cms.PSet(
            isMC = cms.bool(False)
            )
        
    process.GsfElectronToRECO = cms.EDAnalyzer("TagProbeFitTreeProducer",
                                               mcTruthCommonStuff, CommonStuffForGsfElectronProbe,
                                               tagProbePairs = cms.InputTag("tagTightRECO"),
                                               arbitration   = cms.string("Random2"),
                                               flags         = cms.PSet(passingVeto   = cms.InputTag("goodElectronsPROBECutBasedVeto"),
                                                                        passingLoose  = cms.InputTag("goodElectronsPROBECutBasedLoose"),
                                                                        passingMedium = cms.InputTag("goodElectronsPROBECutBasedMedium"),
                                                                        passingTight  = cms.InputTag("goodElectronsPROBECutBasedTight"),
                                                                        ),                                               
                                               allProbes     = cms.InputTag("goodElectronsProbeHLT"),
                                               )
        
    setattr(process.GsfElectronToRECO.variables, "probe_hlt_et", cms.InputTag("hltVarHelper:hltet"))
    setattr(process.GsfElectronToRECO.variables, "probe_hlt_eta", cms.InputTag("hltVarHelper:hlteta"))
    setattr(process.GsfElectronToRECO.variables, "probe_hlt_phi", cms.InputTag("hltVarHelper:hltphi"))
    setattr(process.GsfElectronToRECO.variables, "probe_hlt_sieie", cms.InputTag("hltVarHelper:hltsieie"))
    setattr(process.GsfElectronToRECO.variables, "probe_hlt_ecaliso", cms.InputTag("hltVarHelper:hltecaliso"))
    setattr(process.GsfElectronToRECO.variables, "probe_hlt_hcaliso", cms.InputTag("hltVarHelper:hlthcaliso"))
    setattr(process.GsfElectronToRECO.variables, "probe_hlt_hlthoe", cms.InputTag("hltVarHelper:hlthoe"))
    setattr(process.GsfElectronToRECO.variables, "probe_hlt_tkiso", cms.InputTag("hltVarHelper:hlttkiso"))
    setattr(process.GsfElectronToRECO.variables, "probe_hlt_deta", cms.InputTag("hltVarHelper:hltdeta"))
    setattr(process.GsfElectronToRECO.variables, "probe_hlt_detaseed", cms.InputTag("hltVarHelper:hltdetaseed"))
    setattr(process.GsfElectronToRECO.variables, "probe_hlt_dphi", cms.InputTag("hltVarHelper:hltdphi"))
    setattr(process.GsfElectronToRECO.variables, "probe_hlt_eop", cms.InputTag("hltVarHelper:hlteop"))
    setattr(process.GsfElectronToRECO.variables, "probe_hlt_mishits", cms.InputTag("hltVarHelper:hltmishits"))
    process.GsfElectronToRECO.vertexCollection = cms.InputTag("offlineSlimmedPrimaryVertices")
    #process.GsfElectronToRECO.pfMet = cms.InputTag("pfMET")
    
    if (isMC):
        process.GsfElectronToRECO.eventWeight   = cms.InputTag("generator")
        process.GsfElectronToRECO.PUWeightSrc   = cms.InputTag("pileupReweightingProducer","pileupWeights")
        
    process.tree_sequence = cms.Sequence(process.GsfElectronToRECO)
        
    ##########################################################################
    ## PATHS
    ##########################################################################
        
    #process.out = cms.OutputModule("PoolOutputModule", 
    #                               fileName = cms.untracked.string("pippo.root"),
    #                               #                       fileName = cms.untracked.string(options['OUTPUTEDMFILENAME']),
    #                               #                       SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring("p"))
    #                       )
    
    #if (not options['DEBUG']):
    #    outpath.remove(out)
        
    if (isMC):
        pass
    #    process.pTnP = cms.Sequence(
    #        process.sampleInfo +
    #        process.hltFilter +
    #        process.ele_sequence + 
    #        #process.sc_sequence +
    #        process.allTagsAndProbes +
    #        #pileupReweightingProducer +
    #        process.mc_sequence +
    #        #process.eleVarHelper +
    #        process.hltVarHelper +
    #        process.tree_sequence
    #        )
    else:
        process.pTnP = cms.Path(
            process.sampleInfo +
            process.ele_sequence + 
            process.allTagsAndProbes +
            process.mc_sequence +
            #process.eleVarHelper +
            process.hltVarHelper +
            process.tree_sequence
            )
        
    process.TFileService = cms.Service(
        "TFileService", fileName = cms.string(options['OUTPUT_FILE_NAME']),
        closeFileFast = cms.untracked.bool(True)
        )
    
    

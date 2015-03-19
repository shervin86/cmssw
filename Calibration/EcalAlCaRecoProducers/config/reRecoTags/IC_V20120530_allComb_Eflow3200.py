import FWCore.ParameterSet.Config as cms

from CondCore.DBCommon.CondDBSetup_cfi import *
###
### Comments
### TESTING THE IC!
### Using last laser tag available for 2011
### this laser tag has been used to derive the ICs
###
RerecoGlobalTag = cms.ESSource("PoolDBESSource",
                               CondDBSetup,
                               connect = cms.string('frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'),
                               globaltag = cms.string('GR_R_42_V24::All'),
                               toGet = cms.VPSet(
    cms.PSet(record = cms.string("EcalIntercalibConstantsRcd"),
             tag = cms.string("EcalIntercalibConstants_V20120530_ELEnew_PZold_PS_EtaScaleAllR9_EnergyFlow3200TimeCorr"),
             connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_ECAL")
             ),
    cms.PSet(record = cms.string("EcalLaserAPDPNRatiosRcd"),
             tag = cms.string("EcalLaserAPDPNRatios_data_20120131_158851_183320"),
             connect = cms.untracked.string("frontier://PromptProd/CMS_COND_42X_ECAL_LAS")
             ),
    cms.PSet(record = cms.string('EcalLaserAlphasRcd'),
#             tag = cms.string('EcalLaserAlphas_EB_sic1_btcp152_EE_sic1_btcp116'),
             tag = cms.string('EcalLaserAlphas_EB_sic_btcp152_EE_sic1_btcp118'),
             connect = cms.untracked.string('frontier://FrontierInt/CMS_COND_ECAL')
             )
    ),
                               BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService')
                               )

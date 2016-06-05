import FWCore.ParameterSet.Config as cms
import os, sys, imp, re
import FWCore.ParameterSet.VarParsing as VarParsing

############################################################
### SETUP OPTIONS

options = VarParsing.VarParsing('standard')
options.register('tag',
                 "",
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "name of the tag")
options.register('firstRun',
                 '1',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "")
options.register('fileName',
                 '',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "xml file path")
options.register('record',
                 'EcalIntercalibConstantsRcd',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "record: EcalIntercalibConstantsRcd or EcalTimeCalibConstantsRcd")
### setup any defaults you want
options.output="output/ecalTiming.root"
options.secondaryOutput="ntuple.root"

options.parseArguments()
print options

process = cms.Process("ProcessOne")

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('*'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG')
    ),
    destinations = cms.untracked.vstring('cout')
)

process.source = cms.Source("EmptyIOVSource",
    lastValue = cms.uint64(100000000000),
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(100000000000),
    interval = cms.uint64(1)
)

process.load("CondCore.DBCommon.CondDBCommon_cfi")

tagName = options.tag

process.CondDBCommon.connect = 'sqlite_file:'+options.output

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
  process.CondDBCommon, 
  logconnect = cms.untracked.string('sqlite_file:log.db'),   
  toPut = cms.VPSet(
    cms.PSet(
      record = cms.string(options.record),
      tag = cms.string(tagName)
    )
  )
)

if(options.record=='EcalTimeCalibConstantsRcd'):
    process.mytest = cms.EDAnalyzer("EcalTimeCalibConstantsAnalyzer",
                                    record = cms.string('EcalTimeCalibConstantsRcd'),
                                    loggingOn= cms.untracked.bool(True),
                                    SinceAppendMode=cms.bool(True),
                                    Source=cms.PSet(
            xmlFile = cms.untracked.string(options.fileName),
            since = cms.untracked.int64(int(options.firstRun)),
            )                            
                                    )
else:
    process.mytest = cms.EDAnalyzer("ExTestEcalIntercalibAnalyzer",
                                    record = cms.string(options.record),
                                    loggingOn= cms.untracked.bool(True),
                                    IsDestDbCheckedInQueryLog=cms.untracked.bool(True),
                                    SinceAppendMode=cms.bool(True),
                                    Source=cms.PSet(
            InputFile = cms.string(options.fileName),
            firstRun = cms.string(options.firstRun),
            )                            
                                    )


process.p = cms.Path(process.mytest)

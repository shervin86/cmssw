# 
#
# Read from xml and insert into database using
# PopCon 
#
# This is a template, generate real test using
#
# sed 's/EcalIntercalibConstants/your-record/g' testTemplate.py > testyourrecord.py
#
# Stefano Argiro', $Id: testEcalIntercalibConstants.py,v 1.1 2008/11/14 15:46:03 argiro Exp $
# Modified by Shervin Nourbakhsh 
#

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
                 1,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "")
options.register('fileName',
                 '',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "xml file path")
                 
### setup any defaults you want
options.output="output/ecalTiming.root"
options.secondaryOutput="ntuple.root"

options.parseArguments()
print options

process = cms.Process("TEST")


tagName = options.tag

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('*'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG')
    ),
    destinations = cms.untracked.vstring('cout')
)


process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.CondDBCommon.connect = 'sqlite_file:'+tagName+'.db'
process.CondDBCommon.DBParameters.authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')

process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(100000000000),
    lastValue = cms.uint64(100000000000),
    interval = cms.uint64(1)
)


process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDBCommon,
    #timetype = cms.untracked.string('runnumber'),
    toPut = cms.VPSet(
        cms.PSet(
            record = cms.string('EcalIntercalibConstantsRcd'),
            tag = cms.string(tagName)
            )
        ),
  logconnect= cms.untracked.string('sqlite_file:logtestEcalIntercalibConstants.db')                                     
)

process.mytest = cms.EDAnalyzer("EcalIntercalibConstantsAnalyzer",
    record = cms.string('EcalIntercalibConstantsRcd'),
    loggingOn= cms.untracked.bool(True),
    SinceAppendMode=cms.bool(True),
    Source=cms.PSet(
        InputFile = cms.string(options.fileName),
        #firstRun = cms.string(options.firstRun),
        xmlFile = cms.untracked.string(options.fileName),
        since = cms.untracked.int64(options.firstRun),
        )                            
)

process.p = cms.Path(process.mytest)





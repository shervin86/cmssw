#include "CommonTools/CandAlgos/interface/ModifyObjectValueBase.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"

#include "CondFormats/DataRecord/interface/EcalADCToGeVConstantRcd.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsMCRcd.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "CondFormats/EcalObjects/interface/EcalADCToGeVConstant.h"
#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/EcalObjects/interface/EcalIntercalibConstantsMC.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/Provenance/interface/RunLumiEventNumber.h"

#include "RecoEgamma/EgammaTools/interface/RecHitSlewRateCorrector.h"

//*************************************************
//  Author: Peter Hansen (phansen@physics.umn.edu)
//*************************************************
namespace {
	const edm::EDGetTokenT<edm::ValueMap<float> > empty_token;
	const static edm::InputTag empty_tag("");
}

class EGSlewRateModifier : public ModifyObjectValueBase
{
public:
	struct electron_config {
		edm::InputTag electron_src;
		edm::EDGetTokenT<edm::View<pat::Electron> > tok_electron_src;
	};

	struct photon_config {
		edm::InputTag photon_src ;
		edm::EDGetTokenT<edm::View<pat::Photon> > tok_photon_src;
	};

	EGSlewRateModifier(const edm::ParameterSet& conf);

	void setEvent(const edm::Event&) override final;
	void setEventContent(const edm::EventSetup&) override final;
	void setConsumes(edm::ConsumesCollector&) override final;

	void modifyObject(pat::Electron&) const override final;
	void modifyObject(pat::Photon&) const override final;

private:
	const EcalRecHit * getSeedRecHit(reco::SuperClusterRef supercluster) const;
	float getCorrectedRecHitEnergy(const EcalRecHit * rh) const;

	electron_config _e_conf;
	photon_config   _ph_conf;

	edm::InputTag _RecHitCollectionEB_src;
	edm::InputTag _RecHitCollectionEE_src;
	edm::EDGetTokenT<EcalRecHitCollection> _tok_RecHitCollectionEB ;
	edm::EDGetTokenT<EcalRecHitCollection> _tok_RecHitCollectionEE ;
	const EcalRecHitCollection * _ebrechits;
	const EcalRecHitCollection * _eerechits;

	edm::ESHandle<EcalLaserDbService> _laser;
	edm::ESHandle<EcalIntercalibConstants> _ical;
	edm::ESHandle<EcalADCToGeVConstant> _agc;

	edm::Timestamp _time;
	edm::RunNumber_t _run;
	edm::LuminosityBlockNumber_t _lumi;
	edm::EventNumber_t _evt;

	RecHitSlewRateCorrector _corrector;
	
	bool _isMC;
};

DEFINE_EDM_PLUGIN(ModifyObjectValueFactory,
                  EGSlewRateModifier,
                  "EGSlewRateModifier");

EGSlewRateModifier::EGSlewRateModifier(const edm::ParameterSet& conf) :
	ModifyObjectValueBase(conf)
{
	if( conf.exists("electron_config") ) {
		const edm::ParameterSet& electrons = conf.getParameter<edm::ParameterSet>("electron_config");
		if( electrons.exists("electronSrc") ) _e_conf.electron_src = electrons.getParameter<edm::InputTag>("electronSrc");
	}
	if( conf.exists("photon_config") ) {
		const edm::ParameterSet& photons = conf.getParameter<edm::ParameterSet>("photon_config");
		if( photons.exists("photonSrc") ) _ph_conf.photon_src = photons.getParameter<edm::InputTag>("photonSrc");
	}
	_RecHitCollectionEB_src = conf.getParameter<edm::InputTag>("ecalRecHitsEB");
	_RecHitCollectionEE_src = conf.getParameter<edm::InputTag>("ecalRecHitsEE");

}

void EGSlewRateModifier::setEvent(const edm::Event& iEvent)
{
	_isMC = !iEvent.isRealData();
	edm::Handle<EcalRecHitCollection> ebrechithandle;
	iEvent.getByToken(_tok_RecHitCollectionEB, ebrechithandle);
	_ebrechits = ebrechithandle.product();

	edm::Handle<EcalRecHitCollection> eerechithandle;
	iEvent.getByToken(_tok_RecHitCollectionEE, eerechithandle);
	_eerechits = eerechithandle.product();

	_time = iEvent.eventAuxiliary().time();

	_run = iEvent.eventAuxiliary().run();
	_lumi = iEvent.eventAuxiliary().luminosityBlock();
	_evt = iEvent.eventAuxiliary().event();
}

void EGSlewRateModifier::setEventContent(const edm::EventSetup& iSetup)
{
	using namespace edm;

	iSetup.get<EcalLaserDbRecord>().get(_laser);
	iSetup.get<EcalIntercalibConstantsRcd>().get(_ical);
	iSetup.get<EcalADCToGeVConstantRcd>().get(_agc);

}

void EGSlewRateModifier::setConsumes(edm::ConsumesCollector& sumes)
{
	//setup electrons
	if( !(empty_tag == _e_conf.electron_src) ) _e_conf.tok_electron_src = sumes.consumes<edm::View<pat::Electron> >(_e_conf.electron_src);
	//consumes RecHitCollection

	// setup photons
	if( !(empty_tag == _ph_conf.photon_src) ) _ph_conf.tok_photon_src = sumes.consumes<edm::View<pat::Photon> >(_ph_conf.photon_src);
	//consumes RecHitCollection
	_tok_RecHitCollectionEB = sumes.consumes<EcalRecHitCollection>(_RecHitCollectionEB_src);
	_tok_RecHitCollectionEE = sumes.consumes<EcalRecHitCollection>(_RecHitCollectionEE_src);

}

void EGSlewRateModifier::modifyObject(pat::Electron& ele) const
{
	// Assume that the multiplicative factor from the regression 
	// that doesn't differ much when supercluster raw energy changes
	// I.E.
	// E = F(E_raw,others)*E_raw
	// E_raw' = E_raw + (E'_seed - E_seed)
	//
	// Ideally:
	// E' = F(E_raw',others')*E_raw'
	//
	// but assuming 
	// F(E_raw',others') = F(E_raw,others)
	// we write
	// E' = F(E_raw,others)*(E_raw + E'_seed - E_seed)

	if(_isMC) return;
	if(!ele.ecalDrivenSeed()) return;
	if(ele.energy()<100) return;
	const EcalRecHit * rechit = getSeedRecHit(ele.superCluster());

	float correctedEcalEnergy = ele.correctedEcalEnergy(); // E
	float regression_factor = correctedEcalEnergy / ele.superCluster()->rawEnergy(); //F(E_raw,others)
	float deltaESeed = getCorrectedRecHitEnergy(rechit) - rechit->energy(); //(E'_seed - E_seed)

	float newEnergy = regression_factor * ( ele.superCluster()->rawEnergy() + deltaESeed ); // E'

	auto p4 = ele.p4();
	ele.setCorrectedEcalEnergy(newEnergy);
	auto kind = reco::GsfElectron::P4_COMBINATION;
	// setCorrectedEcalEnergy does not correctly modify p4 so we do it ourselves
	ele.setP4(kind, p4*newEnergy/correctedEcalEnergy, ele.p4Error(kind), true);
}


void EGSlewRateModifier::modifyObject(pat::Photon& pho) const
{
	if(_isMC) return;
	if(pho.energy()<100) return;

	// see comments in void EGSlewRateModifier::modifyObject(pat::Electron& ele) const
	const EcalRecHit * rechit = getSeedRecHit(pho.superCluster());

	float correctedEcalEnergy = pho.energy();
	float regression_factor = correctedEcalEnergy / pho.superCluster()->rawEnergy();
	float deltaESeed = getCorrectedRecHitEnergy(rechit) - rechit->energy();

	float newEnergy = regression_factor * ( pho.superCluster()->rawEnergy() + deltaESeed );

	auto kind = pho.getCandidateP4type();
	pho.setCorrectedEnergy(kind, newEnergy, pho.getCorrectedEnergyError(kind), true);
}

const EcalRecHit * EGSlewRateModifier::getSeedRecHit(reco::SuperClusterRef supercluster) const
{
	auto detid = supercluster->seed()->seed();
//	assert(!detid.null());
	const EcalRecHit * rh = NULL;
	if (detid.subdetId() == EcalBarrel) {
		auto rh_i =  _ebrechits->find(detid);
		assert( rh_i != _ebrechits->end());
		rh =  &(*rh_i);
	} else {
		auto rh_i =  _eerechits->find(detid);
		assert( rh_i != _eerechits->end());
		rh =  &(*rh_i);
	}
	return rh;
}

float EGSlewRateModifier::getCorrectedRecHitEnergy(const EcalRecHit * rh) const
{
	auto lc = _laser->getLaserCorrection(rh->id(), _time);
	auto icalMap = _ical->getMap();
	auto it = icalMap.find(rh->id());
	assert(it != icalMap.end());
	auto ic = (it != icalMap.end()) ? (*it) : 0;
	auto agc = (rh->id().subdetId() == EcalBarrel) ? float(_agc->getEBValue()) : float(_agc->getEEValue());
	return _corrector.RecHitCorrectedEnergy(rh, lc, ic, agc);
}

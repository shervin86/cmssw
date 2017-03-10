#ifndef CalibratedPhotonProducer_h
#define CalibratedPhotonProducer_h

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "EgammaAnalysis/ElectronTools/interface/PhotonEnergyCalibratorRun2.h"

#include <vector>

template<typename T>
class CalibratedPhotonProducerRun2T: public edm::stream::EDProducer<> {
public:
  explicit CalibratedPhotonProducerRun2T( const edm::ParameterSet & ) ;
  virtual ~CalibratedPhotonProducerRun2T();
  virtual void produce( edm::Event &, const edm::EventSetup & ) override ;

private:
  edm::EDGetTokenT<edm::View<T> > thePhotonToken;
  PhotonEnergyCalibratorRun2 theEnCorrectorRun2;
	edm::EDGetTokenT<EcalRecHitCollection> recHitCollectionEBToken_;
	edm::EDGetTokenT<EcalRecHitCollection> recHitCollectionEEToken_;

};

template<typename T>
CalibratedPhotonProducerRun2T<T>::CalibratedPhotonProducerRun2T( const edm::ParameterSet & conf ) :
  thePhotonToken(consumes<edm::View<T> >(conf.getParameter<edm::InputTag>("photons"))),
  theEnCorrectorRun2(conf.getParameter<bool>("isMC"), conf.getParameter<bool>("isSynchronization"), conf.getParameter<std::string >("correctionFile")),
  recHitCollectionEBToken_(consumes<EcalRecHitCollection>(conf.getParameter<edm::InputTag>( "recHitCollectionEB" ))),
  recHitCollectionEEToken_(consumes<EcalRecHitCollection>(conf.getParameter<edm::InputTag>( "recHitCollectionEE" )))
 {

  produces<std::vector<T> >();
}

template<typename T>
CalibratedPhotonProducerRun2T<T>::~CalibratedPhotonProducerRun2T()
{}

template<typename T>
void
CalibratedPhotonProducerRun2T<T>::produce( edm::Event & iEvent, const edm::EventSetup & iSetup ) {

  edm::Handle<edm::View<T> > in;
  iEvent.getByToken(thePhotonToken, in);

	edm::Handle<EcalRecHitCollection> recHitCollectionEBHandle;
	edm::Handle<EcalRecHitCollection> recHitCollectionEEHandle;

	iEvent.getByToken(recHitCollectionEBToken_, recHitCollectionEBHandle);
	iEvent.getByToken(recHitCollectionEEToken_, recHitCollectionEEHandle);

  std::auto_ptr<std::vector<T> > out(new std::vector<T>());
  out->reserve(in->size());   
  
  for (const T &pho : *in) {
    out->push_back(pho);
	const EcalRecHitCollection* recHits = (pho.isEB()) ? recHitCollectionEBHandle.product() : recHitCollectionEEHandle.product();

    theEnCorrectorRun2.calibrate(out->back(), iEvent.id().run(), recHits, iEvent.streamID());
  }
    
  iEvent.put(out);
}

typedef CalibratedPhotonProducerRun2T<reco::Photon> CalibratedPhotonProducerRun2;
typedef CalibratedPhotonProducerRun2T<pat::Photon> CalibratedPatPhotonProducerRun2;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(CalibratedPhotonProducerRun2);
DEFINE_FWK_MODULE(CalibratedPatPhotonProducerRun2);

#endif

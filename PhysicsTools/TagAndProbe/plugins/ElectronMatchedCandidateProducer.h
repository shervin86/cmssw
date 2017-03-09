#ifndef PhysicsTools_TagAndProbe_ElectronMatchedCandidateProducer_h
#define PhysicsTools_TagAndProbe_ElectronMatchedCandidateProducer_h

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

template <class T>
class ElectronMatchedCandidateProducer : public edm::EDProducer {
  
  typedef std::vector<T> TCollection;
  typedef edm::Ref<TCollection> TRef;
  typedef edm::RefVector<TCollection> TRefVector;
    
 public:
  explicit ElectronMatchedCandidateProducer(const edm::ParameterSet&);
  ~ElectronMatchedCandidateProducer();

 private:
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  
  edm::EDGetTokenT<TRefVector> electronCollectionToken_;
  edm::EDGetTokenT<reco::RecoEcalCandidateCollection> scCollectionToken_;
  StringCutObjectSelector<reco::RecoEcalCandidate> candSelector_;
};


template <class T>
ElectronMatchedCandidateProducer<T>::ElectronMatchedCandidateProducer(const edm::ParameterSet &params):
  electronCollectionToken_(consumes<TRefVector>(params.getUntrackedParameter<edm::InputTag>("ReferenceElectronCollection"))),
  scCollectionToken_(consumes<reco::RecoEcalCandidateCollection> (params.getParameter<edm::InputTag>("src"))),
  candSelector_(params.getParameter<std::string>("cut")) 
{
  produces<edm::RefVector<reco::RecoEcalCandidateCollection> >("superclusters");
  produces<TRefVector>("electrons");
}

template <class T>
ElectronMatchedCandidateProducer<T>::~ElectronMatchedCandidateProducer()
{}

template <class T>
void ElectronMatchedCandidateProducer<T>::produce(edm::Event &event,
						  const edm::EventSetup &eventSetup) {

  std::auto_ptr<edm::RefVector<reco::RecoEcalCandidateCollection> > outCol (new edm::RefVector<reco::RecoEcalCandidateCollection>);
  std::auto_ptr<TRefVector> outCol2 (new TRefVector);
  
  // Read electrons
  edm::Handle<TRefVector> electrons;
  event.getByToken(electronCollectionToken_, electrons);
  
  //Read candidates
  edm::Handle<reco::RecoEcalCandidateCollection> recoCandColl;
  event.getByToken(scCollectionToken_ , recoCandColl);

  for (size_t sc=0; sc<recoCandColl->size(); sc++) {
    
    reco::RecoEcalCandidateRef ref(recoCandColl, sc);
    if (candSelector_(*ref)) {

      for (size_t elec=0; elec<electrons->size(); elec++) {
	if ((*electrons)[elec]->superCluster() == ref->superCluster()) {
	  outCol->push_back(ref);
	  outCol2->push_back((*electrons)[elec]);
	}
      } 
    } 
  }

  event.put(outCol, "superclusters");
  event.put(outCol2, "electrons");
}

#endif



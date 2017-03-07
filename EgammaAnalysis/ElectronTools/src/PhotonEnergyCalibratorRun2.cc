#include "EgammaAnalysis/ElectronTools/interface/PhotonEnergyCalibratorRun2.h"
#include <CLHEP/Random/RandGaussQ.h>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/Exception.h"

PhotonEnergyCalibratorRun2::PhotonEnergyCalibratorRun2(bool isMC, bool synchronization, 
													   std::string correctionFile,
													   const EcalRecHitCollection* recHits_
						       ) :
  isMC_(isMC), synchronization_(synchronization),
  rng_(0),
  _correctionRetriever(correctionFile), // here is opening the files and reading thecorrections
  _recHits(recHits_)
{
  if(isMC_) {
    _correctionRetriever.doScale = false; 
    _correctionRetriever.doSmearings = true;
  } else {
    _correctionRetriever.doScale = true; 
    _correctionRetriever.doSmearings = false;
  }
}

PhotonEnergyCalibratorRun2::~PhotonEnergyCalibratorRun2()
{}

void PhotonEnergyCalibratorRun2::initPrivateRng(TRandom *rnd) {
     rng_ = rnd;   
}

void PhotonEnergyCalibratorRun2::calibrate(reco::Photon &photon, unsigned int runNumber, edm::StreamID const & id) const {
    float smear = 0.0, scale = 1.0;
    float aeta = std::abs(photon.superCluster()->eta()); //, r9 = photon.getR9();
    float et = photon.getCorrectedEnergy(reco::Photon::P4type::regression2)/cosh(aeta);
	DetId seedDetId = photon.superCluster()->seed()->seed();
	EcalRecHitCollection::const_iterator seedRecHit = _recHits->find(seedDetId);
	unsigned int gainSeedSC=0;
	if(seedRecHit->checkFlag(EcalRecHit::kHasSwitchToGain6)) gainSeedSC |= 0x01;
	if(seedRecHit->checkFlag(EcalRecHit::kHasSwitchToGain1)) gainSeedSC |= 0x02;

  scale = _correctionRetriever.ScaleCorrection(runNumber, photon.isEB(), photon.full5x5_r9(), aeta, et, gainSeedSC);
  smear = _correctionRetriever.getSmearingSigma(runNumber, photon.isEB(), photon.full5x5_r9(), aeta, et, gainSeedSC, 0., 0.); 
    
    double newEcalEnergy, newEcalEnergyError;
    if (isMC_) {
      double corr = 1.0 + smear * gauss(id);
      newEcalEnergy      = photon.getCorrectedEnergy(reco::Photon::P4type::regression2) * corr;
      newEcalEnergyError = std::hypot(photon.getCorrectedEnergyError(reco::Photon::P4type::regression2) * corr, smear * newEcalEnergy);
    } else {
      newEcalEnergy      = photon.getCorrectedEnergy(reco::Photon::P4type::regression2) * scale;
      newEcalEnergyError = std::hypot(photon.getCorrectedEnergyError(reco::Photon::P4type::regression2) * scale, smear * newEcalEnergy);
    }
    photon.setCorrectedEnergy(reco::Photon::P4type::regression2, newEcalEnergy, newEcalEnergyError, true);

}

double PhotonEnergyCalibratorRun2::gauss(edm::StreamID const& id) const {
  if (synchronization_) return 1.0;
  if (rng_) {
    return rng_->Gaus();
  } else {
    edm::Service<edm::RandomNumberGenerator> rng;
    if ( !rng.isAvailable() ) {
      throw cms::Exception("Configuration")
	<< "XXXXXXX requires the RandomNumberGeneratorService\n"
	"which is not present in the configuration file.  You must add the service\n"
	"in the configuration file or remove the modules that require it.";
    }
    CLHEP::RandGaussQ gaussDistribution(rng->getEngine(id), 0.0, 1.0);
    return gaussDistribution.fire();
  }
}


#include "EgammaAnalysis/ElectronTools/interface/ElectronEnergyCalibratorRun2.h"
#include <CLHEP/Random/RandGaussQ.h>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/Exception.h"

ElectronEnergyCalibratorRun2::ElectronEnergyCalibratorRun2(EpCombinationToolSemi &combinator, 
							   bool isMC, 
							   bool synchronization, 
							   std::string correctionFile
							   ) :
  epCombinationTool_(&combinator),
  isMC_(isMC), synchronization_(synchronization),
  rng_(0),
  _correctionRetriever(correctionFile) // here is opening the files and reading the corrections
{
  if(isMC_) {
    _correctionRetriever.doScale = false; 
    _correctionRetriever.doSmearings = true;
  } else {
    _correctionRetriever.doScale = true; 
    _correctionRetriever.doSmearings = false;
  }
}

ElectronEnergyCalibratorRun2::~ElectronEnergyCalibratorRun2()
{}

void ElectronEnergyCalibratorRun2::initPrivateRng(TRandom *rnd) 
{
  rng_ = rnd;   
}

void ElectronEnergyCalibratorRun2::calibrate(reco::GsfElectron &electron, unsigned int runNumber, edm::StreamID const &id) const
{
  float smear = 0.0, scale = 1.0;
  float aeta = std::abs(electron.superCluster()->eta()); 
  float et = electron.correctedEcalEnergy()/cosh(aeta);
  
  scale = _correctionRetriever.ScaleCorrection(runNumber, electron.isEB(), electron.full5x5_r9(), aeta, et);
  smear = _correctionRetriever.getSmearingSigma(runNumber, electron.isEB(), electron.full5x5_r9(), aeta, et, 0., 0.); 
  
  double newEcalEnergy, newEcalEnergyError;
  if (isMC_) {
    double corr = 1.0 + smear * gauss(id);
    newEcalEnergy      = electron.correctedEcalEnergy() * corr;
    newEcalEnergyError = std::hypot(electron.correctedEcalEnergyError() * corr, smear * newEcalEnergy);
  } else {
    newEcalEnergy      = electron.correctedEcalEnergy() * scale;
    newEcalEnergyError = std::hypot(electron.correctedEcalEnergyError() * scale, smear * newEcalEnergy);
  }
  electron.setCorrectedEcalEnergy(newEcalEnergy);
  electron.setCorrectedEcalEnergyError(newEcalEnergyError);
  std::pair<float, float> combinedMomentum = epCombinationTool_->combine(electron);

  math::XYZTLorentzVector oldFourMomentum = electron.p4();
  math::XYZTLorentzVector newFourMomentum = math::XYZTLorentzVector(oldFourMomentum.x()*combinedMomentum.first/oldFourMomentum.t(),
								    oldFourMomentum.y()*combinedMomentum.first/oldFourMomentum.t(),
								    oldFourMomentum.z()*combinedMomentum.first/oldFourMomentum.t(),
								    combinedMomentum.first); 
  electron.correctMomentum(newFourMomentum, electron.trackMomentumError(), combinedMomentum.second);
}

double ElectronEnergyCalibratorRun2::gauss(edm::StreamID const& id) const 
{
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


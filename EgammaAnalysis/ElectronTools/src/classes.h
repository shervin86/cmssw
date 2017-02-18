#include "EgammaAnalysis/ElectronTools/interface/SimpleElectron.h"
#include "EgammaAnalysis/ElectronTools/interface/EpCombinationTool.h"
#include "EgammaAnalysis/ElectronTools/interface/EpCombinationToolSemi.h"
#include "EgammaAnalysis/ElectronTools/interface/ElectronEnergyCalibratorRun2.h"
#include "EgammaAnalysis/ElectronTools/interface/PhotonEnergyCalibratorRun2.h"

namespace {
  struct dictionaryfuffa {
    EpCombinationToolSemi fuffaElectronCombinatorSemi;
    ElectronEnergyCalibratorRun2 fuffaElectronCalibrator;
    PhotonEnergyCalibratorRun2 fuffaPhotonCalibrator;
  };
}

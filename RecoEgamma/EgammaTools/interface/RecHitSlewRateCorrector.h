#ifndef __RecHitSlewRateCorrector_h__
#define __RecHitSlewRateCorrector_h__

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"

class RecHitSlewRateCorrector
{

public:
	RecHitSlewRateCorrector();
	~RecHitSlewRateCorrector();

	float RecHitCorrectedEnergy(const EcalRecHit *rh, float lc, float ic, float agc) const;

private:
	float MultiFitParametricCorrection(float amplitude_multifit_intime_uncal, float chi2, uint32_t recoflag) const;
	double CorrectionFunction1(double x, double chi2) const;
	double CorrectionFunction2(double x, double chi2) const;
	double CorrectionFunction3(double x, double chi2) const;

};

#endif

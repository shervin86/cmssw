#include "RecoEgamma/EgammaTools/interface/RecHitSlewRateCorrector.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"

RecHitSlewRateCorrector::RecHitSlewRateCorrector() {};
RecHitSlewRateCorrector::~RecHitSlewRateCorrector() {};

float RecHitSlewRateCorrector::RecHitCorrectedEnergy(const EcalRecHit *rh, float lc, float ic, float agc) const
{

	float calib = rh->energy();
	uint32_t recoflag = 0;
	if (rh->checkFlag(EcalRecHit::kHasSwitchToGain6)) recoflag |= 0x1 << EcalUncalibratedRecHit::kHasSwitchToGain6;
	if (rh->checkFlag(EcalRecHit::kHasSwitchToGain1)) recoflag |= 0x1 << EcalUncalibratedRecHit::kHasSwitchToGain1;
	return calib / MultiFitParametricCorrection(calib / lc / ic / agc, rh->chi2(), recoflag);

}

float RecHitSlewRateCorrector::MultiFitParametricCorrection(float amplitude_multifit_intime_uncal, float chi2, uint32_t recoflag) const
{

	float x = amplitude_multifit_intime_uncal;
	bool has_g6 = ((recoflag / 16) % 2 == 1);
	bool has_g1 = ((recoflag / 32) % 2 == 1);

	if (!has_g1 && !has_g6) return 1; // no gain switch
	else if (has_g6 && !has_g1) {
		if (x > 5000 && chi2 < 250) return 1;
		else return CorrectionFunction1(x, chi2);
	} else if (!has_g6 && has_g1) {
		if (chi2 > 6000) return CorrectionFunction1(x, chi2);
		else if (x < 4000 || x > 7000) return 1;
		else return CorrectionFunction3(x, chi2);
	} else if (has_g1 && has_g6) return CorrectionFunction2(x, chi2);
	else return 1;

}

double RecHitSlewRateCorrector::CorrectionFunction1(double x, double chi2) const
{
	if (x < 4000) return 1;
	if (x > 10000) x = 10000;
	double p0   =    0.0567521;
	double p1   =  0.000609019;
	double p2   = -1.35626e-07;
	double p3   =  1.21114e-11;
	double p4   = -3.84392e-16;
	return p0 + p1 * x + p2 * x * x + p3 * x * x * x + p4 * x * x * x * x;
}
double RecHitSlewRateCorrector::CorrectionFunction2(double x, double chi2) const
{
	if (x < 7000) return 1;
	if (x > 18000) x = 18000;
	double p0   =    0.770166;
	double p1   = 7.22759e-05;
	double p2   = -6.8392e-09;
	double p3   = 1.67209e-13;
	return p0 + p1 * x + p2 * x * x + p3 * x * x * x;
}
double RecHitSlewRateCorrector::CorrectionFunction3(double x, double chi2) const
{
	if (x < 4000 || x > 7000) return 1;
	double p0   =       -2.391;
	double p1   =  0.000997096;
	double p2   = -8.34263e-08;
	return p0 + p1 * x + p2 * x * x;
}

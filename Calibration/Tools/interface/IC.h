#ifndef IC_HH
#define IC_HH

//
// Federico Ferri, CEA-Saclay Irfu/SPP, 14.12.2011
// federico.ferri@cern.ch
//

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/EcalObjects/interface/EcalIntercalibErrors.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TRandom.h"
#include "TTree.h"

#include "Calibration/Tools/interface/DRings.h"


class DS;
/**
 * Error values:
 * - 98: IC set to 1 by SetToUnit method
 * - 99:  channel removed because outlier
 * - 101: channel with only one valid value in product
 * - 997: invalid, but with error <900, set to this value in dump
 * - 999: not calibrated by the calibrator
 * - 1000: value not present in the file provided by the calibrator
 * - 1001: channel set as invalid from other IC set
 * - 1010: channel with no error
 * - 1015: channel with no kFactor from phisym
 * - 1020: channel with no hits from timing
 * - 1021: channel with error larger than time shift
 */

class IC
{
public:
	enum EcalPart { kAll, kEB, kEE };

	enum kERROR{
		kOUTLIER=99,
		kINVALIDPROD=100,
		kNOERROR=1010
	};
		
	typedef struct Coord {
		int ix_;
		int iy_;
		int iz_;
	} Coord;

	static void coord(DetId id, Coord * c);


	IC(bool isTime = false);
	//IC(const DRings &dr){ dr_ = dr; idr_=true;} ///< constructor with rings, missing constructor function

	void PrintInfos(void);
	inline void SetIsTime()
	{
		_isTime = true;
	};
	EcalIntercalibConstants & ic()
	{
		return _ic;
	}
	EcalIntercalibErrors & eic()
	{
		return _eic;
	}

	const EcalIntercalibConstants & ic() const
	{
		return _ic;
	}
	const EcalIntercalibErrors & eic() const
	{
		return _eic;
	}
	const std::vector<DetId> & ids() const
	{
		return _detId;
	}
	void setRings(const DRings & dr)
	{
		dr_ = dr;
		idr_ = true;
	}

	// plotters
	static void constantMap(const IC & a, TH2F * h, DS & d, bool errors = false);
	static void constantDistribution(const IC & a, TH1F * h, DS & d, bool errors = false);
	static void profileEta(const IC & a, TProfile * h, DS & d, bool errors = false);
	static void profilePhi(const IC & a, TProfile * h, DS & d, bool errors = false);
	static void profileSM(const IC & a, TProfile * h, DS & d, bool errors = false);

	static bool isValid(float v, float e, bool isTime = false);

	// IC manipulation
	bool operator ==(const IC &b);
	static void reciprocal(const IC & a, IC & res);
	static void multiply(const IC & a, float c, IC & res, DS & d);
	static void multiply(const IC & a, const IC & b, IC & res);
	IC operator *(const IC &b);
	IC operator /(const IC &b);
	void operator /=(const IC &b);
	void operator /=(float val);
	IC operator +(const IC &b);
	IC operator -(const IC &b);

	static void add(const IC & a, const IC & b, IC & res);
	static void combine(const IC & a, const IC & b, IC & res, bool arithmetic = false); // N.B. arithmetic average is for value and errrors
	IC combine(const IC& a, const IC&b, bool arithmetic = false);
	IC combine(const IC & a, const IC & b, const IC& c, bool arithmetic);

	static void fillHoles(const IC & a, const IC & b, IC & res);
	void fillHoles(const IC & b, bool preserveErrors = true); ///< fill the non-calibrated channels with the values of a second set of ICs

	static void removeOutliers(const IC & a, IC & res, float min = 0.4, float max = 2.5);
	void removeOutliers(float min = 0.4, float max = 2.5);
	static void smear(const IC & a, float sigma, IC & res);
	static void smear(const IC & a, IC & res);

	// tools
	IC getPhiScale(DS& selector) const;
	IC getEtaScale();
	static void applyEtaScale(IC & ic);
	static void scaleEta(IC & ic, const IC & ic_scale, bool reciprocalScale = false);
	inline void scaleEta(const IC& ic_scale)
	{
		scaleEta(*this, ic_scale, false);
	}
	inline void unscaleEta(void)
	{
		*this /= getEtaScale();
		//scaleEta(*this, *this, true);
	}
	static void applyTwoCrystalEffect(IC & ic);
	static void setToUnit(IC & ic, DS & selector);
	void setToUnit(DS& selector)
	{
		setToUnit(*this, selector);
	}

	bool isUnit(void);

	void dump(const IC & a, const char * fileName, DS & d, bool invalid = true);
	inline void dump(const char *fileName, DS &d, bool invalid = true) //invalid to indicate if you want to dump invalid values
	{
		dump(*this, fileName, d, invalid);
	}

	static void dumpXML(const IC & a, const char * fileName, DS & d, bool errors = false);
	static void readSimpleTextFile(const char * fileName, IC & ic);
	void readSimpleTextFile(const char * fileName);
	static void readTextFile(const char * fileName, IC & ic);
	void readTextFile(const char * fileName);
	static void readXMLFile(const char * fileName, IC & ic);
	void readXMLFile(const char *fileName);
	static void readCmscondXMLFile(const char * fileName, IC & ic);
	static void readEcalChannelStatusFromTextFile(const char * fileName, EcalChannelStatus & channelStatus);
	static void makeRootTree(TTree & t, const IC & ic);

	// dumps for checking
	static void dumpEtaScale(const IC & a, const char * fileName, bool allIC = false);
	void dumpEtaScale(const char *fileName, DS& selector);
	void dumpPhiScale(const char *fileName, DS& selector);
	static void dumpOutliers(const IC & a, float min = 0.4, float max = 2.5);

	static float average(const IC & a, DS & d, bool errors = false);
	float average(DS& selector, bool errors = false)
	{
		return average(*this, selector, errors);
	}

	float Normalize(DS& selector); ///< calculate the mean over the region and renormalize to 1 that region and returns the mean value used for the normalization
	void BonToBoff(const IC& Bcorr, const IC& alphas, DS& selector);
	void PrintNewCalibrated(const IC& ref) const;

	void SetInvalids(const IC&ref); ///<this method sets as invalid the IDs that are invalid in the ref IC set
	void RemoveNonSignificant(void); ///< if shifts are smaller than error, don't change the channel

private:
	EcalIntercalibConstants   _ic;
	EcalIntercalibErrors     _eic;
	static DRings dr_;
	static bool idr_;
	static std::vector<DetId>     _detId;
	bool _isTime;
	double _defaultValue, _noValue;
	
};

#endif

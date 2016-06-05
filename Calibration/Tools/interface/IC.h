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

class IC
{
public:
	enum EcalPart { kAll, kEB, kEE };

	typedef struct Coord {
		int ix_;
		int iy_;
		int iz_;
	} Coord;

	static void coord(DetId id, Coord * c);


	IC(bool isTime=false);
	//IC(const DRings &dr){ dr_ = dr; idr_=true;} ///< constructor with rings, missing constructor function

	void PrintInfos(void);

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

	static bool isValid(float v, float e, bool isTime=false);

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
	void dumpEtaScale(const char *fileName)
	{
		dumpEtaScale(*this, fileName);
	}
	static void dumpOutliers(const IC & a, float min = 0.4, float max = 2.5);

	static float average(const IC & a, DS & d, bool errors = false);
	float average(DS& selector, bool errors = false)
	{
		return average(*this, selector, errors);
	}

	float Normalize(DS& selector);
	void BonToBoff(const IC& Bcorr, const IC& alphas, DS& selector);
	void PrintNewCalibrated(const IC& ref) const;

private:
	EcalIntercalibConstants   _ic;
	EcalIntercalibErrors     _eic;
	static DRings dr_;
	static bool idr_;
	static std::vector<DetId>     _detId;
	bool _isTime;
};

#endif

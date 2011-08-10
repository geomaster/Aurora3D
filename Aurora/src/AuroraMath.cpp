#include "AuroraMath.h"
using namespace Aurora;

Real Math::rsqrt_e(Real v, int PrecisionLevel)
{
#	if AURORA_USE_DOUBLE_PREICSION != 1 // Only applicable to IEEE 754-* floats
    long i;
    float half, castBack;
    const float threeOverTwo = 1.5f;
 
    half = v * Real(0.5);
    i  = *reinterpret_cast<long*>(&v);
    i  = 0x5f3759df - ( i >> 1 );				// Well, fuck.
    castBack = *reinterpret_cast<Real*>(&i);

	Real sq = castBack * castBack;
	for (int j = 0; j < PrecisionLevel; ++j)
		castBack  = castBack * (threeOverTwo - (half * sq)); // Newton's method

    return castBack;
#	else
	return rsqrt(v);
#	endif
}
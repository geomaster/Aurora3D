/*
    This file is part of Aurora Game Engine.

    Aurora Game Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Aurora Game Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Aurora Game Engine.  If not, see <http://www.gnu.org/licenses/>.

	Copyright (C) David Davidovic (Geomaster) 2011.

*/
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

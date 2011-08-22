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
#include "AuroraQuaternion.h"
using namespace Aurora;

Quaternion Quaternion::Zero(
	Real( 0.0 ), Real( 0.0 ), Real( 0.0 ), Real( 0.0 )
);

Quaternion Quaternion::Identity(
	Real( 1.0 ), Real( 0.0 ), Real( 0.0 ), Real( 0.0 )
);

Real Quaternion::getNorm() const
{
#	if AURORA_CACHING_LEVEL >= 1
	if (!mNormDirty)
		return mNorm;
#	endif

	Real norm = *mW * *mW + *mX * *mX + *mY * *mY + *mZ * *mZ;

#	if AURORA_CACHING_LEVEL >= 1
	mNorm = norm;
	mNormDirty = false;
#	endif

	return norm;
}

Real Quaternion::getLength() const
{
#	if AURORA_CACHING_LEVEL >= 1
	if (!mLengthDirty) return mLength;
	else if (!mNormDirty)
	{
		mLengthDirty = false;
		mLength = Math::sqrt(mNorm);
		return mLength;
	}
#	endif

	Real norm = *mW * *mW + *mX * *mX + *mY * *mY + *mZ * *mZ;
	Real len = Math::sqrt(norm);

#	if AURORA_CACHING_LEVEL >= 1
	mNorm = norm;
	mLength = Math::sqrt(norm);
	mNormDirty = mLengthDirty = false;
#	endif

	return len;
}

void Quaternion::normalizeApproximate()
{
#	if AURORA_SSE_ENABLED
	Real norm = getNorm();
	if (Math::notEqual(norm, Real( 1.0 )) && Math::notEqual(norm, Real( 0.0 )))
		SSE::packedNormalizeV4Approx(mComponents, norm, mComponents);
#	else
	if (Math::notEqual(norm, Real( 1.0 )) && Math::notEqual(norm, Real( 0.0 )))
		normalize();
#	endif

#	if AURORA_CACHING_LEVEL >= 1
	mLengthDirty = mNormDirty = true; // can't set it to 1 because it's approximate
#	endif
}

void Quaternion::normalize()
{
	Real norm = getNorm();

	if (Math::notEqual(norm, Real( 1.0 )) && Math::notEqual(norm, Real( 0.0 )))
	{
		Real invLength = Real( 1.0 ) / Math::sqrt(norm);
		*mW *= invLength;
		*mX *= invLength;
		*mY *= invLength;
		*mZ *= invLength;

#		if AURORA_CACHING_LEVEL >= 1
		mLengthDirty = mNormDirty = false;
		mLength = mNorm = Real( 1.0 );
#		endif
	}
}

Quaternion Quaternion::getNormalized() const
{
	Quaternion q(*this);
	q.normalize();
	return q;
}

Quaternion Quaternion::getNormalizedApproximate() const
{
	Quaternion q(*this);
	q.normalizeApproximate();
	return q;
}

void Quaternion::invert()
{
	Real invLsq = Real( 1.0 ) / getNorm();
	*mW *= invLsq;
	*mX *= -invLsq;
	*mY *= -invLsq;
	*mZ *= -invLsq;

	// The length remains the same
}

Quaternion Quaternion::getInverse() const
{
	Quaternion q(*this);
	q.invert();
	return q;
}

void Quaternion::invertUnit()
{
	*mX = -*mX;
	*mY = -*mY;
	*mZ = -*mZ;
}

Quaternion Quaternion::getInverseUnit() const
{
	Quaternion q(*this);
	q.invertUnit();
	return q;
}

Matrix3 Quaternion::toRotationMatrix3() const
{
	Real w2 = *mW * *mW, x2 = *mX * *mX, y2 = *mY * *mY, z2 = *mZ * *mZ;
	Real twoXy = Real( 2.0 ) * *mX * *mY, twoWz = Real( 2.0 ) * *mW * *mZ, twoXz = Real( 2.0 ) * *mX * *mZ, twoWy = Real( 2.0 ) * *mW * *mY, twoYz = Real( 2.0 ) * *mY * *mZ, twoWx = Real( 2.0 ) * *mW * *mX;

	return Matrix3(
		w2 + x2 - y2 - z2, twoXy - twoWz, twoXz + twoWy,
		twoXy + twoWz, w2 - x2 + y2 - z2, twoYz + twoWx,
		twoXz - twoWy, twoYz - twoWx, w2 - x2 - y2 + z2
	);
}

Matrix3 Quaternion::toRotationMatrix3Unit() const
{
	Real twoX2 = Real( 2.0 ) * *mX * *mX, twoY2 = Real( 2.0 ) * *mY * *mY, twoZ2 = Real( 2.0 ) * *mZ * *mZ;
	Real twoXy = Real( 2.0 ) * *mX * *mY, twoWz = Real( 2.0 ) * *mW * *mZ, twoXz = Real( 2.0 ) * *mX * *mZ, twoWy = Real( 2.0 ) * *mW * *mY, twoYz = Real( 2.0 ) * *mY * *mZ, twoWx = Real( 2.0 ) * *mW * *mX;

	return Matrix3(
		Real( 1.0 ) - twoY2 - twoZ2, twoXy - twoWz, twoXz + twoWy,
		twoXy + twoWz, Real( 1.0 ) - twoX2 - twoZ2, twoYz + twoWx,
		twoXz - twoWy, twoYz - twoWx, Real( 1.0 ) - twoX2 - twoY2
	);

}

Matrix4 Quaternion::toRotationMatrix4() const
{
	Real w2 = *mW * *mW, x2 = *mX * *mX, y2 = *mY * *mY, z2 = *mZ * *mZ;
	Real twoXy = Real( 2.0 ) * *mX * *mY, twoWz = Real( 2.0 ) * *mW * *mZ, twoXz = Real( 2.0 ) * *mX * *mZ, twoWy = Real( 2.0 ) * *mW * *mY, twoYz = Real( 2.0 ) * *mY * *mZ, twoWx = Real( 2.0 ) * *mW * *mX;

	return Matrix4(
		w2 + x2 - y2 - z2, twoXy - twoWz, twoXz + twoWy, 0,
		twoXy + twoWz, w2 - x2 + y2 - z2, twoYz + twoWx, 0,
		twoXz - twoWy, twoYz - twoWx, w2 - x2 - y2 + z2, 0,
		0, 0, 0, 1
	);
}

Matrix4 Quaternion::toRotationMatrix4Unit() const
{
	Real twoX2 = Real( 2.0 ) * *mX * *mX, twoY2 = Real( 2.0 ) * *mY * *mY, twoZ2 = Real( 2.0 ) * *mZ * *mZ;
	Real twoXy = Real( 2.0 ) * *mX * *mY, twoWz = Real( 2.0 ) * *mW * *mZ, twoXz = Real( 2.0 ) * *mX * *mZ, twoWy = Real( 2.0 ) * *mW * *mY, twoYz = Real( 2.0 ) * *mY * *mZ, twoWx = Real( 2.0 ) * *mW * *mX;

	return Matrix4(
		Real( 1.0 ) - twoY2 - twoZ2, twoXy - twoWz, twoXz + twoWy, 0,
		twoXy + twoWz, Real( 1.0 ) - twoX2 - twoZ2, twoYz + twoWx, 0,
		twoXz - twoWy, twoYz - twoWx, Real( 1.0 ) - twoX2 - twoY2, 0,
		0, 0, 0, 1
	);

}

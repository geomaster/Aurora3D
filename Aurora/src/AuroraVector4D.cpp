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
#include "AuroraVector4D.h"
using namespace Aurora;

/* Constant static members */
const Vector4D Vector4D::Zero(
	Real( 0.0 ), Real( 0.0 ), Real( 0.0 ), Real( 0.0 )
	);

/* Member functions */
Real Vector4D::getLengthSquared() const
{
#	if AURORA_CACHING_LEVEL >= 1
	if (!mLSQDirty) return mLengthSquared;
#	endif

	float lenSq = (*mX) * (*mX) + (*mY) * (*mY) + (*mZ) * (*mZ) + (*mW) * (*mW);

#	if AURORA_CACHING_LEVEL >= 1
	mLengthSquared = lenSq;
	mLSQDirty = false;
#	endif

	return lenSq;
}

Real Vector4D::getLength() const
{
#	if AURORA_CACHING_LEVEL >= 1
	if (!mLengthDirty) return mLength;
#	endif

	float len = Math::sqrt(getLengthSquared());

#	if AURORA_CACHING_LEVEL >= 1
	mLengthDirty = false;
	mLength = len;
#	endif

	return len;

}

void Vector4D::normalize()
{
	Real lenSq = getLengthSquared();

	// First I do a test to see if the vector is already normalized; this saves us
	// from the second comparison if it indeed is (it is more common to try to
	// normalize a 1- than 0-length vector).
	if (!Math::equal(lenSq, 1.0) && !Math::equal(lenSq, 0.0))
	{
		float invLen;
#		if AURORA_CACHING_LEVEL >= 1
		if (!mLengthDirty) invLen = Real(1.0) / mLength; else
#		endif
		invLen = Real(1.0) / Math::sqrt(lenSq);

#		if AURORA_CACHING_LEVEL >= 1
		mLength = mLengthSquared = 1.0;
		mLengthDirty = mLSQDirty = false;
#		endif

		*mX *= invLen, *mY *= invLen, *mZ *= invLen, *mW *= invLen;
	}
}

Vector4D Vector4D::getNormalized() const
{
	// To be honest, this /does/ include a little performance loss.
	Vector4D v(*this);
	v.normalize();
	return v;
}

Real Vector4D::dot(const Vector4D& other) const
{
	return (*mX) * (*other.mX) + (*mY) * (*other.mY) + (*mZ) * (*other.mZ) + (*mW) * (*other.mW);
}

Angle Vector4D::getAngleTo(const Vector4D& other) const
{
	// A·B = |A| * |B| * cos θ, θ = arccos (A·B / (|A| * |B|))

	// NB that A·B / (|A| * |B|) cannot generate an invalid value for Math::acos
	// because of the Cauchy-Schwarz inequality
	return Radian(Math::acos(dot(other) / (getLength() * other.getLength())));
}

Real Vector4D::getDistanceTo(const Vector4D& other) const
{
	Vector4D diff(*other.mX - *mX, *other.mY - *mY, *other.mZ - *mZ, *other.mW - *mW);
	return diff.getLength();
}

Real Vector4D::getSquaredDistanceTo(const Vector4D& other) const
{
	Vector4D diff(*other.mX - *mX, *other.mY - *mY, *other.mZ - *mZ, *other.mW - *mW);
	return diff.getLengthSquared();
}

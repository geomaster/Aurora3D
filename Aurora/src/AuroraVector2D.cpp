#include "AuroraVector2D.h"
using namespace Aurora;

/* Constant static members */
const Vector2D Vector2D::Zero(
	Real( 0.0 ), Real( 0.0 )
	);

const Vector2D Vector2D::UnitX(
	Real( 1.0 ), Real( 0.0 )
	);

const Vector2D Vector2D::UnitY(
	Real( 0.0 ), Real( 1.0 )
	);

/* Member functions */
Real Vector2D::getLengthSquared() const
{
#	if AURORA_CACHING_LEVEL >= 1
	if (!mLSQDirty) return mLengthSquared;
#	endif

	float lenSq = (*mX) * (*mX) + (*mY) * (*mY);

#	if AURORA_CACHING_LEVEL >= 1
	mLengthSquared = lenSq;
	mLSQDirty = false;
#	endif

	return lenSq;
}

Real Vector2D::getLength() const
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

void Vector2D::normalize()
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

		*mX *= invLen, *mY *= invLen;
	}
}

Vector2D Vector2D::getNormalized() const
{
	// To be honest, this /does/ include a little performance loss.
	Vector2D v(*this);
	v.normalize();
	return v;
}

Real Vector2D::dot(const Vector2D& other) const
{
	return (*mX) * (*other.mX) + (*mY) * (*other.mY);
}

Angle Vector2D::getAngleTo(const Vector2D& other) const
{
	// A·B = |A| * |B| * cos θ, θ = arccos (A·B / (|A| * |B|))

	// NB that A·B / (|A| * |B|) cannot generate an invalid value for Math::acos
	// because of the Cauchy-Schwarz inequality
	return Radian(Math::acos(dot(other) / (getLength() * other.getLength())));
}

Real Vector2D::getDistanceTo(const Vector2D& other) const
{
	Vector2D diff(*other.mX - *mX, *other.mY - *mY);
	return diff.getLength();
}

Real Vector2D::getSquaredDistanceTo(const Vector2D& other) const
{
	Vector2D diff(*other.mX - *mX, *other.mY - *mY);
	return diff.getLengthSquared();
}

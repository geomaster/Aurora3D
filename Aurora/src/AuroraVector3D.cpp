#include "AuroraVector3D.h"
using namespace Aurora;

/* Constant static members */
const Vector3D Vector3D::Zero(
	Real( 0.0 ), Real( 0.0 ), Real( 0.0 )
	);

const Vector3D Vector3D::UnitX(
	Real( 1.0 ), Real( 0.0 ), Real( 0.0 )
	);

const Vector3D Vector3D::UnitY(
	Real( 0.0 ), Real( 1.0 ), Real( 0.0 )
	);

const Vector3D Vector3D::UnitZ(
	Real( 0.0 ), Real( 0.0 ), Real( 1.0 )
	);

/* Member functions */
Real Vector3D::getLengthSquared() const
{
#	if AURORA_CACHING_LEVEL >= 1
	if (!mLSQDirty) return mLengthSquared;
#	endif

	float lenSq = (*mX) * (*mX) + (*mY) * (*mY) + (*mZ) * (*mZ);

#	if AURORA_CACHING_LEVEL >= 1
	mLengthSquared = lenSq;
	mLSQDirty = false;
#	endif

	return lenSq;
}

Real Vector3D::getLength() const
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

void Vector3D::normalize()
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

		*mX *= invLen, *mY *= invLen, *mZ *= invLen;
	}
}

Vector3D Vector3D::getNormalized() const
{
	// To be honest, this /does/ include a little performance loss.
	Vector3D v(*this);
	v.normalize();
	return v;
}

Real Vector3D::dot(const Vector3D& other) const
{
	return (*mX) * (*other.mX) + (*mY) * (*other.mY) + (*mZ) * (*other.mZ);
}

Vector3D Vector3D::crossProduct(const Vector3D& other) const
{
	// (a1)   (b1)    (a2*b3 - a3*b2)
	// (a2) × (b2) =  (a3*b1 - a1*b3)
	// (a3)   (b3)    (a1*b2 - a2*b1)
	// (NOT commutative)

	return Vector3D(
		(*mY) * (*other.mZ) - (*mZ) * (*other.mY),
		(*mZ) * (*other.mX) - (*mX) * (*other.mZ),
		(*mX) * (*other.mY) - (*mY) * (*other.mX)
	);
}

Angle Vector3D::getAngleTo(const Vector3D& other) const
{
	// A·B = |A| * |B| * cos θ, θ = arccos (A·B / (|A| * |B|))

	// NB that A·B / (|A| * |B|) cannot generate an invalid value for Math::acos
	// because of the Cauchy-Schwarz inequality
	return Radian(Math::acos(dot(other) / (getLength() * other.getLength())));
}

Real Vector3D::getDistanceTo(const Vector3D& other) const
{
	Vector3D diff(*other.mX - *mX, *other.mY - *mY, *other.mZ - *mZ);
	return diff.getLength();
}

Real Vector3D::getSquaredDistanceTo(const Vector3D& other) const
{
	Vector3D diff(*other.mX - *mX, *other.mY - *mY, *other.mZ - *mZ);
	return diff.getLengthSquared();
}

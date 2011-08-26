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
#ifndef __AURORA_VECTOR3D_H__
#define __AURORA_VECTOR3D_H__
#include "AuroraMath.h"
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"
#include <cstring>

namespace Aurora
{
	// Caching behavior for Vector3D:
	// (1) normal caching: caches its squared length and length to aviod recomputation
	// (2) agressive caching: no additional caching measures, same as (1)

	// You are maybe wondering why I didn't use SSE optimizations here. The answer
	// is simple: they're slow. They are built to operate on 4 single-precision floats
	// in one instruction; however, when operating on only 2 or 3 floats, SSE's slower
	// than standard FPU instructions. The speed gain of SSE is slight and makes sense
	// only when doing a lot of computations at high speeds (60 Hz, for example).

	class AURORA_LIBRARY Vector3D : virtual public SSEAlloc
	{
	public:
		const static Vector3D Zero;
		const static Vector3D UnitX;
		const static Vector3D UnitY;
		const static Vector3D UnitZ;

	protected:
#		if AURORA_SSE_ENABLED
		AURORA_SSE_ALIGN Real mComponents[4];
#		else
		Real mComponents[3];
#		endif

		inline void initValues(bool dty = false)
		{
#			if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mLSQDirty = dty;
			mLength = mLengthSquared = 0.0;
#			endif

#			if AURORA_SSE_ENABLED
			mComponents[3] = Real( 1.0 );
#			endif

			mX = mComponents, mY = mX + 1, mZ = mY + 1;
		}

		// Keep record of the addresses to avoid offseting every time we need a component
		Real *mX, *mY, *mZ;

		// On normal & aggressive caching, we will keep this length and length**2 variables.
		// We'll cache them after they're fetched.
#		if AURORA_CACHING_LEVEL != 0
		mutable bool mLengthDirty;
		mutable bool mLSQDirty;
		mutable Real mLengthSquared;
		mutable Real mLength;
#		endif

	public:
		inline Vector3D()
		{
			initValues();
			*mX = *mY = *mZ = 0.0;
		}

		inline Vector3D(Real X, Real Y, Real Z)
		{
			initValues(true);
			*mX = X;
			*mY = Y;
			*mZ = Z;
		}

		inline Vector3D(const Vector3D& other)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = other.mLengthDirty;
			mLSQDirty = other.mLSQDirty;
			if (!mLengthDirty)
				mLength = other.mLength;
			if (!mLSQDirty)
				mLengthSquared = other.mLengthSquared;
		#	endif

		#	if AURORA_SSE_ENABLED
			mComponents[3] = Real( 1.0 );
		#	endif

			mX = mComponents, mY = mX + 1, mZ = mY + 1;
			memcpy(mComponents, other.mComponents, 3 * sizeof(Real));
		}

		explicit inline Vector3D(Real Scalar)
		{
			mX = mComponents, mY = mX + 1, mZ = mY + 1;
			*mX = *mY = *mZ = Scalar;

		#	if AURORA_SSE_ENABLED
			mComponents[3] = Real( 1.0 );
		#	endif

		#	if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mLSQDirty = false;
			mLength = Scalar * Sqr3;
			mLengthSquared = Scalar * Scalar * Real(3.0);
		#	endif
		}

		explicit inline Vector3D(const Real *Components)
		{
			initValues(true);
			memcpy(mComponents, Components, 3 * sizeof(Real));
		}


		inline Real getX() const
		{
			return *mX;
		}

		inline Real getY() const
		{
			return *mY;
		}

		inline Real getZ() const
		{
			return *mZ;
		}

		inline void setX(Real X)
		{
			*mX = X;
		#	if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mLSQDirty = true;
		#	endif
		}

		inline void setY(Real Y)
		{
			*mY = Y;
		#	if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mLSQDirty = true;
		#	endif
		}

		inline void setZ(Real Z)
		{
			*mZ = Z;
		#	if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mLSQDirty = true;
		#	endif
		}


		Real getLength() const ;
		Real getLengthSquared() const;
		void normalize();
		Vector3D getNormalized() const;

		Real dot(const Vector3D& other) const;
		Vector3D crossProduct(const Vector3D& other) const;

		Angle getAngleTo(const Vector3D& other) const;
		Real getDistanceTo(const Vector3D& other) const;
		Real getSquaredDistanceTo(const Vector3D& other) const;

		bool equals(const Vector3D& other, Real Epsilon = AURORA_FPTOLERANCE) const
		{
			return (Math::equal(*mX, *other.mX, Epsilon) && Math::equal(*mY, *other.mY, Epsilon) && Math::equal(*mZ, *other.mZ));
		}

		inline Vector3D operator + (const Vector3D& other) const
		{
			return Vector3D(*mX + *other.mX, *mY + *other.mY, *mZ + *other.mZ);
		}

		inline Vector3D operator - (const Vector3D& other) const
		{
			return Vector3D(*mX - *other.mX, *mY - *other.mY, *mZ - *other.mZ);
		}

		inline Vector3D operator - () const
		{
			return Vector3D(-*mX, -*mY, -*mZ);
		}

		inline Vector3D operator * (const Vector3D& other) const
		{
			return Vector3D(*mX * *other.mX, *mY * *other.mY, *mZ * *other.mZ);
		}

		inline Vector3D operator * (Real Scalar) const
		{
			return Vector3D(*mX * Scalar, *mY * Scalar, *mZ * Scalar);
		}

		inline Vector3D operator / (const Vector3D& other) const
		{
			return Vector3D(*mX / *other.mX, *mY / *other.mY, *mZ / *other.mZ);
		}

		inline Vector3D operator / (Real Scalar) const
		{
			return Vector3D(*mX / Scalar, *mY / Scalar, *mZ / Scalar);
		}

		inline const Vector3D& operator = (const Vector3D& other)
		{
			memcpy(mComponents, other.mComponents, 3 * sizeof(Real));
		#	if AURORA_CACHING_LEVEL >= 1
			mLSQDirty = other.mLSQDirty;
			mLengthDirty = other.mLengthDirty;
			if (!mLengthDirty)
				mLength = other.mLength;
			if (mLSQDirty)
				mLengthSquared = other.mLengthSquared;
		#	endif

			return *this;
		}

		inline const Vector3D& operator +=(const Vector3D& other)
		{
			*mX += *other.mX, *mY += *other.mY, *mZ += *other.mZ;
			return *this;
		}

		inline const Vector3D& operator -=(const Vector3D& other)
		{
			*mX -= *other.mX, *mY -= *other.mY, *mZ -= *other.mZ;
			return *this;
		}

		inline const Vector3D& operator *=(const Vector3D& other)
		{
			*mX *= *other.mX, *mY *= *other.mY, *mZ *= *other.mZ;
			return *this;
		}

		inline const Vector3D& operator *=(Real Scalar)
		{
			*mX *= Scalar, *mY *= Scalar, *mZ *= Scalar;
			return *this;
		}

		inline const Vector3D& operator /=(const Vector3D& other)
		{
			*mX /= *other.mX, *mY /= *other.mY, *mZ /= *other.mZ;
			return *this;
		}

		inline const Vector3D& operator /=(Real Scalar)
		{
			*mX /= Scalar, *mY /= Scalar, *mZ /= Scalar;
			return *this;
		}

		inline bool operator ==(const Vector3D& other)
		{
			return *mX == *other.mX && *mY == *other.mY && *mZ == *other.mZ;
		}

	};
}
#endif // __AURORA_VECTOR3D_H__

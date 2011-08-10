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
#ifndef __AURORA_VECTOR4D_H__
#define __AURORA_VECTOR4D_H__
#include "AuroraMath.h"
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"
#if AURORA_SSE_ENABLED
#include "AuroraSSE.h"
#endif
#include <cstring>

namespace Aurora
{
	// Caching behavior for Vector4D:
	// (1) normal caching: caches its squared length and length to aviod recomputation
	// (2) agressive caching: no additional caching measures, same as (1)

	class AURORA_LIBRARY Vector4D : virtual public SSEAlloc
	{
	public:
		const static Vector4D Zero;

	protected:
		AURORA_SSE_ALIGN Real mComponents[4];

		inline void initValues(bool dty = false)
		{
#			if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mLSQDirty = dty;
			mLength = mLengthSquared = 0.0;
#			endif

			mX = mComponents, mY = mX + 1, mZ = mY + 1, mW = mZ + 1;
		}

		// Keep record of the addresses to avoid offseting every time we need a component
		Real *mX, *mY, *mZ, *mW;

		// On normal & aggressive caching, we will keep this length and length**2 variables.
		// We'll cache them after they're fetched.
#		if AURORA_CACHING_LEVEL != 0
		mutable bool mLengthDirty;
		mutable bool mLSQDirty;
		mutable Real mLengthSquared;
		mutable Real mLength;
#		endif

	public:
		inline Vector4D()
		{
			initValues();
			*mX = *mY = *mZ = *mW = 0.0;
		}

		inline Vector4D(Real X, Real Y, Real Z, Real W)
		{
			initValues(true);
			*mX = X;
			*mY = Y;
			*mZ = Z;
			*mW = W;
		}

		inline Vector4D(const Vector4D& other)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = other.mLengthDirty;
			mLSQDirty = other.mLSQDirty;
			if (!mLengthDirty)
				mLength = other.mLength;
			if (!mLSQDirty)
				mLengthSquared = other.mLengthSquared;
		#	endif

			mX = mComponents, mY = mX + 1, mZ = mY + 1, mW = mZ + 1;
			memcpy(mComponents, other.mComponents, 4 * sizeof(Real));
		}

		explicit inline Vector4D(Real Scalar)
		{
			mX = mComponents, mY = mX + 1, mZ = mY + 1, mW = mZ + 1;
			*mX = *mY = *mZ = *mW = Scalar;

		#	if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mLSQDirty = false;
			mLength = Scalar * Real(2.0);
			mLengthSquared = Scalar * Scalar * Real(4.0);
		#	endif

		}

		explicit inline Vector4D(const Real *Components)
		{
			initValues(true);
			memcpy(mComponents, Components, 4 * sizeof(Real));
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

		inline Real getW() const
		{
			return *mW;
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

		inline void setW(Real W)
		{
			*mW = W;
		#	if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mLSQDirty = true;
		#	endif
		}

		Real getLength() const ;
		Real getLengthSquared() const;
		void normalize();
		Vector4D getNormalized() const;

		Real dot(const Vector4D& other) const;

		Angle getAngleTo(const Vector4D& other) const;
		Real getDistanceTo(const Vector4D& other) const;
		Real getSquaredDistanceTo(const Vector4D& other) const;

		inline Vector4D operator + (const Vector4D& other) const
		{
		#	if AURORA_SSE_ENABLED
			Vector4D r;
			SSE::packedAdd(mComponents, other.mComponents, r.mComponents);
			return r;
		#	else
			return Vector4D(*mX + *other.mX, *mY + *other.mY, *mZ + *other.mZ, *mW + *other.mW);
		#	endif
		}

		inline Vector4D operator - (const Vector4D& other) const
		{
		#	if AURORA_SSE_ENABLED
			Vector4D r;
			SSE::packedSubtract(mComponents, other.mComponents, r.mComponents);
			return r;
		#	else
			return Vector4D(*mX - *other.mX, *mY - *other.mY, *mZ - *other.mZ, *mW - *other.mW);
		#	endif
		}

		inline Vector4D operator - () const
		{
			return Vector4D(-*mX, -*mY, -*mZ, -*mW);
		}

		inline Vector4D operator * (const Vector4D& other) const
		{
		#	if AURORA_SSE_ENABLED
			Vector4D r;
			SSE::packedMultiply(mComponents, other.mComponents, r.mComponents);
			return r;
		#	else
			return Vector4D(*mX * *other.mX, *mY * *other.mY, *mZ * *other.mZ, *mW * *other.mW);
		#	endif
		}

		inline Vector4D operator * (Real Scalar) const
		{
			return Vector4D(*mX * Scalar, *mY * Scalar, *mZ * Scalar, *mW * Scalar);
		}

		inline Vector4D operator / (const Vector4D& other) const
		{
		#	if AURORA_SSE_ENABLED
			Vector4D r;
			SSE::packedDivide(mComponents, other.mComponents, r.mComponents);
			return r;
		#	else
			return Vector4D(*mX / *other.mX, *mY / *other.mY, *mZ / *other.mZ, *mW / *other.mW);
		#	endif
		}

		inline Vector4D operator / (Real Scalar) const
		{
			return Vector4D(*mX / Scalar, *mY / Scalar, *mZ / Scalar, *mW  / Scalar);
		}

		inline const Vector4D& operator = (const Vector4D& other)
		{
			memcpy(mComponents, other.mComponents, 4 * sizeof(Real));
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

		inline const Vector4D& operator +=(const Vector4D& other)
		{
		#	if AURORA_SSE_ENABLED
			SSE::packedAdd(mComponents, other.mComponents, mComponents);
		#	else
			*mX += *other.mX, *mY += *other.mY, *mZ += *other.mZ, *mW += *other.mW;
		#	endif
			return *this;
		}

		inline const Vector4D& operator -=(const Vector4D& other)
		{
		#	if AURORA_SSE_ENABLED
			SSE::packedSubtract(mComponents, other.mComponents, mComponents);
		#	else
			*mX -= *other.mX, *mY -= *other.mY, *mZ -= *other.mZ, *mW -= *other.mW;
		#	endif
			return *this;
		}

		inline const Vector4D& operator *=(const Vector4D& other)
		{
		#	if AURORA_SSE_ENABLED
			SSE::packedMultiply(mComponents, other.mComponents, mComponents);
		#	else
			*mX *= *other.mX, *mY *= *other.mY, *mZ *= *other.mZ, *mW *= *other.mW;
		#	endif
			return *this;
		}

		inline const Vector4D& operator *=(Real Scalar)
		{
			*mX *= Scalar, *mY *= Scalar, *mZ *= Scalar, *mW *= Scalar;
			return *this;
		}

		inline const Vector4D& operator /=(const Vector4D& other)
		{
		#	if AURORA_SSE_ENABLED
			SSE::packedDivide(mComponents, other.mComponents, mComponents);
		#	else
			*mX /= *other.mX, *mY /= *other.mY, *mZ /= *other.mZ, *mW /= *other.mW;
		#	endif
			return *this;
		}

		inline const Vector4D& operator /=(Real Scalar)
		{
			*mX /= Scalar, *mY /= Scalar, *mZ /= Scalar, *mW /= Scalar;
			return *this;
		}

		inline bool operator ==(const Vector4D& other)
		{
#			if AURORA_SSE_ENABLED
			return SSE::packedEqual(mComponents, other.mComponents);
#			else
			return *mX == *other.mX && *mY == *other.mY && *mZ == *other.mZ && *mW == *other.mW;
#			endif
		}

	};
}
#endif // __AURORA_VECTOR4D_H__

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
#ifndef __AURORA_VECTOR2D_H__
#define __AURORA_VECTOR2D_H__
#include "AuroraMath.h"
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"
#include <cstring>

namespace Aurora
{
	// Caching behavior for Vector2D:
	// (1) normal caching: caches its squared length and length to aviod recomputation
	// (2) agressive caching: no additional caching measures, same as (1)

	// You are maybe wondering why I didn't use SSE optimizations here. The answer
	// is simple: they're slow. They are built to operate on 4 single-precision floats
	// in one instruction; however, when operating on only 2 or 3 floats, SSE's slower
	// than standard FPU instructions. The speed gain of SSE is slight and makes sense
	// only when doing a lot of computations at high speeds (60 Hz, for example).

	class AURORA_LIBRARY Vector2D : virtual public SSEAlloc
	{
	public:
		const static Vector2D Zero;
		const static Vector2D UnitX;
		const static Vector2D UnitY;

	protected:
		Real mComponents[2];

		inline void initValues(bool dty = false)
		{
#			if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mLSQDirty = dty;
			mLength = mLengthSquared = 0.0;
#			endif

			mX = mComponents, mY = mX + 1;
		}

		// Keep record of the addresses to avoid offseting every time we need a component
		Real *mX, *mY;

		// On normal & aggressive caching, we will keep this length and length**2 variables.
		// We'll cache them after they're fetched.
#		if AURORA_CACHING_LEVEL != 0
		mutable bool mLengthDirty;
		mutable bool mLSQDirty;
		mutable Real mLengthSquared;
		mutable Real mLength;
#		endif

	public:
		inline Vector2D()
		{
			initValues();
			*mX = *mY = 0.0;
		}

		inline Vector2D(Real X, Real Y)
		{
			initValues(true);
			*mX = X;
			*mY = Y;
		}

		inline Vector2D(const Vector2D& other)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = other.mLengthDirty;
			mLSQDirty = other.mLSQDirty;
			if (!mLengthDirty)
				mLength = other.mLength;
			if (!mLSQDirty)
				mLengthSquared = other.mLengthSquared;
		#	endif

			mX = mComponents, mY = mX + 1;
			memcpy(mComponents, other.mComponents, 2 * sizeof(Real));
		}

		explicit inline Vector2D(Real Scalar)
		{
			mX = mComponents, mY = mX + 1;
			*mX = *mY = Scalar;

		#	if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mLSQDirty = false;
			mLength = Scalar * Sqr2;
			mLengthSquared = Scalar * Scalar * Real(2.0);
		#	endif

		}

		explicit inline Vector2D(const Real *Components)
		{
			initValues(true);
			memcpy(mComponents, Components, 2 * sizeof(Real));
		}


		Real getX() const
		{
			return *mX;
		}

		Real getY() const
		{
			return *mY;
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

		Real getLength() const;
		Real getLengthSquared() const;
		void normalize();
		Vector2D getNormalized() const;

		Real dot(const Vector2D& other) const ;
		Angle getAngleTo(const Vector2D& other) const;
		Real getDistanceTo(const Vector2D& other) const;
		Real getSquaredDistanceTo(const Vector2D& other) const;

		inline Vector2D operator + (const Vector2D& other) const
		{
			return Vector2D(*mX + *other.mX, *mY + *other.mY);
		}

		inline Vector2D operator - (const Vector2D& other) const
		{
			return Vector2D(*mX - *other.mX, *mY - *other.mY);
		}

		inline Vector2D operator - () const
		{
			return Vector2D(-*mX, -*mY);
		}

		inline Vector2D operator * (const Vector2D& other) const
		{
			return Vector2D(*mX * *other.mX, *mY * *other.mY);
		}

		inline Vector2D operator * (Real Scalar) const
		{
			return Vector2D(*mX * Scalar, *mY * Scalar);
		}

		inline Vector2D operator / (const Vector2D& other) const
		{
			return Vector2D(*mX / *other.mX, *mY / *other.mY);
		}

		inline Vector2D operator / (Real Scalar) const
		{
			return Vector2D(*mX / Scalar, *mY / Scalar);
		}

		inline const Vector2D& operator = (const Vector2D& other)
		{
			memcpy(mComponents, other.mComponents, 2 * sizeof(Real));
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

		inline const Vector2D& operator +=(const Vector2D& other)
		{
			*mX += *other.mX, *mY += *other.mY;
			return *this;
		}

		inline const Vector2D& operator -=(const Vector2D& other)
		{
			*mX -= *other.mX, *mY -= *other.mY;
			return *this;
		}

		inline const Vector2D& operator *=(const Vector2D& other)
		{
			*mX *= *other.mX, *mY *= *other.mY;
			return *this;
		}

		inline const Vector2D& operator *=(Real Scalar)
		{
			*mX *= Scalar, *mY *= Scalar;
			return *this;
		}

		inline const Vector2D& operator /=(const Vector2D& other)
		{
			*mX /= *other.mX, *mY /= *other.mY;
			return *this;
		}

		inline const Vector2D& operator /=(Real Scalar)
		{
			*mX /= Scalar, *mY /= Scalar;
			return *this;
		}

		inline bool operator ==(const Vector2D& other)
		{
			return *mX == *other.mX && *mY == *other.mY;
		}

	};
}
#endif // __AURORA_VECTOR2D_H__

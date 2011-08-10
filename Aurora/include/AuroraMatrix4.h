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
#ifndef __AURORA_MATRIX4_H__
#define __AURORA_MATRIX4_H__
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"
#include "AuroraVector3D.h"
#include "AuroraMath.h"
#include "AuroraMatrix3.h"
#if AURORA_SSE_ENABLED
#include "AuroraSSE.h"
#endif
#include <cassert>

namespace Aurora
{
	// Caching behavior for Matrix4:
	// (1) normal caching: keeps track of the determinant and the inverse so they need not
	// be computed again.
	// (2) same as (1)

	// Row-major matrix i.e. Mat[1][2] will retrieve the entry in the first row and
	// second column.
	class AURORA_LIBRARY Matrix4 : virtual public SSEAlloc
	{
	public:
		const static Matrix4 Identity;
		const static Matrix4 Zero;

	protected:
		AURORA_SSE_ALIGN Real mComponents[16];

#		if AURORA_CACHING_LEVEL >= 1
		mutable Real mDeterminant;
		mutable Real mInverse[16];
		mutable bool mDetDirty, mInvDirty;
#		endif

		inline bool inv4x4(Real *m, const Real *s = NULL) const;
		inline void trans4x4(Real *m, const Real *s = NULL) const;
		inline Real minorDet(std::size_t Column, std::size_t Row, const Real *m) const;
	public:

		inline Matrix4()
		{
#			if AURORA_CACHING_LEVEL >= 1
			mDetDirty = mInvDirty = true;
#			endif
		}

		inline Matrix4(const Matrix4& other)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mDetDirty = other.mDetDirty;
			mInvDirty = other.mInvDirty;
			if (!mDetDirty) mDeterminant = other.mDeterminant;
			if (!mInvDirty) memcpy(mInverse, other.mInverse, 16 * sizeof(Real));
		#	endif

			memcpy(mComponents, other.mComponents, 16 * sizeof(Real));
		}

		inline Matrix4(Real m00, Real m01, Real m02, Real m03,
				Real m10, Real m11, Real m12, Real m13,
				Real m20, Real m21, Real m22, Real m23,
				Real m30, Real m31, Real m32, Real m33)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mDetDirty = mInvDirty = true;
		#	endif

			mComponents[0] = m00;
			mComponents[1] = m01;
			mComponents[2] = m02;
			mComponents[3] = m03;
			mComponents[4] = m10;
			mComponents[5] = m11;
			mComponents[6] = m12;
			mComponents[7] = m13;
			mComponents[8] = m20;
			mComponents[9] = m21;
			mComponents[10] = m22;
			mComponents[11] = m23;
			mComponents[12] = m30;
			mComponents[13] = m31;
			mComponents[14] = m32;
			mComponents[15] = m33;

		}

		Matrix4(const Matrix3& other);

		explicit inline Matrix4(const Real* ptr)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mDetDirty = mInvDirty = true;
		#	endif

			memcpy(mComponents, ptr, 16 * sizeof(Real));
		}

		explicit inline Matrix4(const Real Scalar)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mDetDirty = mInvDirty = true;
		#	endif

			for (Real* ptr = mComponents; ptr < mComponents + 16; ++ptr)
				*ptr = Scalar;
		}

		inline Real getEntry(std::size_t Column, std::size_t Row)
		{
			AURORA_ASSERT(Column < 4 && Row < 4 );
			return mComponents[LEA2D(Column, Row, 4)];
		}

		inline void setEntry(std::size_t Column, std::size_t Row, Real Value)
		{
			AURORA_ASSERT(Column < 4 && Row < 4);
			mComponents[LEA2D(Column, Row, 4)] = Value;

		#	if AURORA_CACHING_LEVEL >= 1
			mDetDirty = mInvDirty = true;
		#	endif
		}


		Real getDeterminant() const;
		bool getInverse(Matrix4& Dest) const;
		void invert();
		Matrix4 getTransposed() const;
		void transpose();

		inline Matrix4 operator + (const Matrix4& other) const
		{
			Matrix4 res;
		#	if AURORA_SSE_ENABLED
			SSE::packedAddM4(mComponents, other.mComponents, res.mComponents);
		#	else

			for (std::size_t i = 0; i < 16; ++i)
				res.mComponents[i] = mComponents[i] + other.mComponents[i];
		#	endif
			return res;
		}

		inline Matrix4 operator - (const Matrix4& other) const
		{
			Matrix4 res;
		#	if AURORA_SSE_ENABLED
			SSE::packedSubtractM4(mComponents, other.mComponents, res.mComponents);
		#	else
			for (std::size_t i = 0; i < 16; ++i)
				res.mComponents[i] = mComponents[i] - other.mComponents[i];
		#	endif
			return res;
		}

		inline Matrix4 operator - () const
		{
			Matrix4 res;
			for (std::size_t i = 0; i < 16; ++i)
				res.mComponents[i] = -mComponents[i];

			return res;
		}

		inline Matrix4 operator * (const Matrix4& other) const
		{
			Matrix4 result;
		#	if AURORA_SSE_ENABLED
			SSE::packedMultiplyM4(mComponents, other.mComponents, result.mComponents);
		#	else
			for (std::size_t x = 0; x < 4; ++x)
				for (std::size_t y = 0; y < 4; ++y)
					result.mComponents[LEA2D( x, y, 4 )] =
						mComponents[LEA2D( 0, y, 4 )] * other.mComponents[LEA2D( x, 0, 4 )] +
						mComponents[LEA2D( 1, y, 4 )] * other.mComponents[LEA2D( x, 1, 4 )] +
						mComponents[LEA2D( 2, y, 4 )] * other.mComponents[LEA2D( x, 2, 4 )] +
						mComponents[LEA2D( 3, y, 4 )] * other.mComponents[LEA2D( x, 3, 4 )];

		#	endif
			return result;

		}

		inline Matrix4 operator * (Real Scalar) const
		{
			Matrix4 res;
			for (std::size_t i = 0; i < 16; ++i)
				res.mComponents[i] = mComponents[i] * Scalar;

			return res;
		}

		inline AURORA_LIBRARY friend Matrix4 operator * (Real Scalar, const Matrix4& m)
		{
			return m * Scalar;
		}

		inline Vector3D operator * (const Vector3D& other) const
		{
			Real x = other.getX(), y = other.getY(), z = other.getZ();
			Real wInv = Real( 1.0 ) / (mComponents[12] * x + mComponents[13] * y + mComponents[14] * z + mComponents[15]);

			Vector3D r(
				(mComponents[0] * x + mComponents[1] * y + mComponents[2] * z + mComponents[3]) * wInv,
				(mComponents[4] * x + mComponents[5] * y + mComponents[6] * z + mComponents[7]) * wInv,
				(mComponents[8] * x + mComponents[9] * y + mComponents[10] * z + mComponents[11]) * wInv);

			return r;
		}

		inline AURORA_LIBRARY friend Vector3D operator * (const Vector3D& v, const Matrix4& m)
		{
			return m * v;
		}

		inline const Real* operator[] (std::size_t Row) const
		{
			return mComponents + 4 * Row;
		}

		inline const Matrix4& operator = (const Matrix4& other)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mDetDirty = other.mDetDirty;
			mInvDirty = other.mInvDirty;
			if (!mDetDirty) mDeterminant = other.mDeterminant;
			if (!mInvDirty) memcpy(mInverse, other.mInverse, 16 * sizeof(Real));
		#	endif

			memcpy(mComponents, other.mComponents, 16 * sizeof(Real));

			return *this;
		}

		inline const Matrix4& operator +=(const Matrix4& other)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mInvDirty = mDetDirty = true;
		#	endif

		#	if AURORA_SSE_ENABLED
			SSE::packedAddM4(mComponents, other.mComponents, mComponents);
		#	else
			for (std::size_t x = 0; x < 4; ++x)
				for (std::size_t y = 0; y < 4; ++y)
					mComponents[LEA2D(x,y,4)] += other.mComponents[LEA2D(x,y,4)];
		#	endif

			return *this;
		}

		inline const Matrix4& operator -=(const Matrix4& other)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mInvDirty = mDetDirty = true;
		#	endif

		#	if AURORA_SSE_ENABLED
			SSE::packedSubtractM4(mComponents, other.mComponents, mComponents);
		#	else
			for (std::size_t x = 0; x < 4; ++x)
				for (std::size_t y = 0; y < 4; ++y)
					mComponents[LEA2D(x,y,4)] -= other.mComponents[LEA2D(x,y,4)];
		#	endif

			return *this;
		}

		inline const Matrix4& operator *=(const Matrix4& other)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mInvDirty = mDetDirty = true;
		#	endif

			AURORA_SSE_ALIGN Real tmp[16];

		#	if AURORA_SSE_ENABLED
			SSE::packedMultiplyM4(mComponents, other.mComponents, tmp);
		#	else
			for (std::size_t x = 0; x < 4; ++x)
			{
				for (std::size_t y = 0; y < 4; ++y)
				{
					Real mul = mComponents[LEA2D( 0, y, 4 )] * other.mComponents[LEA2D( x, 0, 4 )] +
						mComponents[LEA2D( 1, y, 4 )] * other.mComponents[LEA2D( x, 1, 4 )] +
						mComponents[LEA2D( 2, y, 4 )] * other.mComponents[LEA2D( x, 2, 4 )] +
						mComponents[LEA2D( 3, y, 4 )] * other.mComponents[LEA2D (x, 3, 4 )];

					tmp[LEA2D(x,y,4)] = mul;
				}
			}
		#	endif

			memcpy(mComponents, tmp, 16 * sizeof(Real));
			return *this;
		}

		inline const Matrix4& operator *=(Real Scalar)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mInvDirty = mDetDirty = true;
		#	endif

			for (std::size_t i = 0; i < 16; ++i)
			{
				mComponents[i] *= Scalar;
			}

			return *this;
		}

		inline bool operator ==(const Matrix4& other) const
		{
#			if AURORA_SSE_ENABLED
			return SSE::packedEqualM4(mComponents, other.mComponents);
#			else
			for (std::size_t i = 0; i < 16; ++i)
				if (mComponents[i] == other.mComponents[i]) return false;
			return true;
#			endif
		}

		// Static generators
		static Matrix4 fromXRotation(Angle Theta);
		static Matrix4 fromYRotation(Angle Theta);
		static Matrix4 fromZRotation(Angle Theta);
		static Matrix4 fromXYZRotation(Angle X, Angle Y, Angle Z);
		static Matrix4 fromAngleAxis(const Vector3D& Axis, Angle Theta);
		static Matrix4 fromScale(Real X, Real Y, Real Z);
		static Matrix4 fromTranslation(Real X, Real Y, Real Z);
		static Matrix4 fromTranslation(const Vector3D& Translation);
	};
}

#endif // __AURORA_MATRIX4_H__

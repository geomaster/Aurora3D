#ifndef __AURORA_MATRIX3_H__
#define __AURORA_MATRIX3_H__
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"
#include "AuroraVector3D.h"
#include "AuroraMath.h"

#include <cassert>

namespace Aurora
{
	// Caching behavior for Matrix3:
	// (1) normal caching: keeps track of the determinant and the inverse so they need not
	// be computed again.
	// (2) same as (1) but also precomputes the transpose of the matrix

	// Row-major matrix i.e. Mat[1][2] will retrieve the entry in the first row and
	// second column.
	class AURORA_LIBRARY Matrix3 : public virtual SSEAlloc
	{
	public:
		const static Matrix3 Identity;
		const static Matrix3 Zero;

	protected:
		Real mComponents[9];

#		if AURORA_CACHING_LEVEL >= 1
		mutable Real mDeterminant;
		mutable Real mInverse[9];
		mutable bool mDetDirty, mInvDirty;
#		endif

		inline bool inv3x3(Real *m, const Real *s = NULL) const;
		inline void trans3x3(Real *m, const Real *s = NULL) const;
		inline Real minorDet(std::size_t Column, std::size_t Row, const Real *m) const;

	public:
		inline Matrix3()
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mDetDirty = mInvDirty = true;
		#	endif
		}

		inline Matrix3(const Matrix3& other)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mDetDirty = other.mDetDirty;
			mInvDirty = other.mInvDirty;
			if (!mDetDirty) mDeterminant = other.mDeterminant;
			if (!mInvDirty) memcpy(mInverse, other.mInverse, 9 * sizeof(Real));
		#	endif


			memcpy(mComponents, other.mComponents, 9 * sizeof(Real));
		}

		inline Matrix3(Real m00, Real m01, Real m02,
				Real m10, Real m11, Real m12,
				Real m20, Real m21, Real m22)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mDetDirty = mInvDirty = true;
		#	endif

			mComponents[0] = m00;
			mComponents[1] = m01;
			mComponents[2] = m02;
			mComponents[3] = m10;
			mComponents[4] = m11;
			mComponents[5] = m12;
			mComponents[6] = m20;
			mComponents[7] = m21;
			mComponents[8] = m22;

		}

		explicit inline Matrix3(const Real* ptr)
		{
			Matrix3(ptr[0], ptr[1], ptr[2],
					ptr[3], ptr[4], ptr[5],
					ptr[6], ptr[7], ptr[8]);
		}

		explicit inline Matrix3(const Real Scalar)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mDetDirty = mInvDirty = true;
		#	endif

			for (Real* ptr = mComponents; ptr < mComponents + 9; ++ptr)
				*ptr = Scalar;
		}

		inline Real getEntry(std::size_t Column, std::size_t Row)
		{
			AURORA_ASSERT(Column <3 && Row <3 ); // Columns and rows need love too!
			return mComponents[LEA2D(Column, Row, 3)];
		}

		inline void setEntry(std::size_t Column, std::size_t Row, Real Value)
		{
			AURORA_ASSERT(Column < 3 && Row < 3);
			mComponents[LEA2D(Column, Row, 3)] = Value;

		#	if AURORA_CACHING_LEVEL >= 1
			mDetDirty = mInvDirty = true;
		#	endif
		}


		Real getDeterminant() const;
		bool getInverse(Matrix3& Dest) const;
		void invert();
		Matrix3 getTransposed() const;
		void transpose();
		inline bool isOrthogonal() const
		{
			Matrix3 inverted;
			if (!getInverse(inverted))
				return false;
			else return inverted == getTransposed();
		}

		inline Matrix3 operator + (const Matrix3& other) const
		{
			return Matrix3(
				mComponents[0] + other.mComponents[0],
				mComponents[1] + other.mComponents[1],
				mComponents[2] + other.mComponents[2],
				mComponents[3] + other.mComponents[3],
				mComponents[4] + other.mComponents[4],
				mComponents[5] + other.mComponents[5],
				mComponents[6] + other.mComponents[6],
				mComponents[7] + other.mComponents[7],
				mComponents[8] + other.mComponents[8]
			);
		}

		inline Matrix3 operator - (const Matrix3& other) const
		{
			return Matrix3(
				mComponents[0] - other.mComponents[0],
				mComponents[1] - other.mComponents[1],
				mComponents[2] - other.mComponents[2],
				mComponents[3] - other.mComponents[3],
				mComponents[4] - other.mComponents[4],
				mComponents[5] - other.mComponents[5],
				mComponents[6] - other.mComponents[6],
				mComponents[7] - other.mComponents[7],
				mComponents[8] - other.mComponents[8]
			);
		}

		inline Matrix3 operator - () const
		{
			return Matrix3(
				-mComponents[0],
				-mComponents[1],
				-mComponents[2],
				-mComponents[3],
				-mComponents[4],
				-mComponents[5],
				-mComponents[6],
				-mComponents[7],
				-mComponents[8]
			);
		}

		inline Matrix3 operator * (const Matrix3& other) const
		{
			Matrix3 result;
			for (std::size_t x = 0; x < 3; ++x)
				for (std::size_t y = 0; y < 3; ++y)
					result.setEntry(x, y,
						mComponents[LEA2D( 0, y, 3 )] * other.mComponents[LEA2D( x, 0, 3 )] +
						mComponents[LEA2D( 1, y, 3 )] * other.mComponents[LEA2D( x, 1, 3 )] +
						mComponents[LEA2D( 2, y, 3 )] * other.mComponents[LEA2D( x, 2, 3 )]);

			return result;
		}

		inline Matrix3 operator * (Real Scalar) const
		{
			return Matrix3(
				mComponents[0] * Scalar,
				mComponents[1] * Scalar,
				mComponents[2] * Scalar,
				mComponents[3] * Scalar,
				mComponents[4] * Scalar,
				mComponents[5] * Scalar,
				mComponents[6] * Scalar,
				mComponents[7] * Scalar,
				mComponents[8] * Scalar
			);
		}

		inline AURORA_LIBRARY friend Matrix3 operator * (Real Scalar, const Matrix3& m)
		{
			return m * Scalar;
		}

		inline Vector3D operator * (const Vector3D& other) const
		{
			Real x = other.getX(), y = other.getY(), z = other.getZ();
			return Vector3D(
				mComponents[0] * x + mComponents[1] * y + mComponents[2] * z,
				mComponents[3] * x + mComponents[4] * y + mComponents[5] * z,
				mComponents[6] * x + mComponents[7] * y + mComponents[8] * z);
		}

		inline AURORA_LIBRARY friend Vector3D operator * (const Vector3D& v, const Matrix3& m)
		{
			return m * v;
		}

		inline const Real* operator[] (std::size_t Row) const
		{
			return mComponents + 3 * Row;
		}

		inline const Matrix3& operator = (const Matrix3& other)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mDetDirty = other.mDetDirty;
			mInvDirty = other.mInvDirty;
			if (!mDetDirty) mDeterminant = other.mDeterminant;
			if (!mInvDirty) memcpy(mInverse, other.mInverse, 9 * sizeof(Real));
		#	endif

			memcpy(mComponents, other.mComponents, 9 * sizeof(Real));

			return *this;
		}

		inline const Matrix3& operator +=(const Matrix3& other)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mInvDirty = mDetDirty = true;
		#	endif

			for (std::size_t x = 0; x < 3; ++x)
				for (std::size_t y = 0; y < 3; ++y)
					mComponents[LEA2D(x,y,3)] += other.mComponents[LEA2D(x,y,3)];

			return *this;
		}

		inline const Matrix3& operator -=(const Matrix3& other)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mInvDirty = mDetDirty = true;
		#	endif

			for (std::size_t x = 0; x < 3; ++x)
				for (std::size_t y = 0; y < 3; ++y)
					mComponents[LEA2D(x,y,3)] -= other.mComponents[LEA2D(x,y,3)];

			return *this;
		}

		inline const Matrix3& operator *=(const Matrix3& other)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mInvDirty = mDetDirty = true;
		#	endif

			Real tmp[16];
			for (std::size_t x = 0; x < 3; ++x)
			{
				for (std::size_t y = 0; y < 3; ++y)
				{
					Real mul = mComponents[LEA2D( 0, y, 3 )] * other.mComponents[LEA2D( x, 0, 3 )] +
						mComponents[LEA2D( 1, y, 3 )] * other.mComponents[LEA2D( x, 1, 3 )] +
						mComponents[LEA2D( 2, y, 3 )] * other.mComponents[LEA2D( x, 2, 3 )];

					tmp[LEA2D(x,y,3)] = mul;
				}
			}
			memcpy(mComponents, tmp, 16 * sizeof(Real));

			return *this;
		}

		inline const Matrix3& operator *=(Real Scalar)
		{
		#	if AURORA_CACHING_LEVEL >= 1
			mInvDirty = mDetDirty = true;
		#	endif

			for (std::size_t i = 0; i < 9; ++i)
			{
				mComponents[i] *= Scalar;
			}

			return *this;
		}

		inline bool operator ==(const Matrix3& other) const
		{
			for (std::size_t i = 0; i < 9; ++i)
				if (mComponents[i] == other.mComponents[i]) return false;
			return true;
		}

		// Static generators
		static Matrix3 fromXRotation(Angle Theta);
		static Matrix3 fromYRotation(Angle Theta);
		static Matrix3 fromZRotation(Angle Theta);
		static Matrix3 fromXYZRotation(Angle X, Angle Y, Angle Z);
		static Matrix3 fromAngleAxis(const Vector3D& Axis, Angle Theta);
		static Matrix3 fromScale(Real X, Real Y, Real Z);
	};
}

#endif // __AURORA_MATRIX3_H__

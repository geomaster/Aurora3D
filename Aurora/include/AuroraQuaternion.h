#ifndef __AURORA_QUATERNION_H__
#define __AURORA_QUATERNION_H__
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"
#include "AuroraMatrix3.h"
#include "AuroraMatrix4.h"
#include "AuroraVector3D.h"
#include "AuroraMath.h"
#if AURORA_SSE_ENABLED
#include "AuroraSSE.h"
#endif

namespace Aurora
{
	class AURORA_LIBRARY Quaternion : virtual public SSEAlloc
	{
	public:
		static Quaternion Zero;
		static Quaternion Identity;

	protected:
        AURORA_SSE_ALIGN Real mComponents[4];

		Real *mW, *mX, *mY, *mZ;

		void initValues(bool dty = true)
		{
#			if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mNormDirty = dty;
#			endif

			mW = mComponents, mX = mW + 1, mY = mX + 1, mZ = mY + 1;
		}

#		if AURORA_CACHING_LEVEL >= 1
		mutable Real mLength, mNorm;
		mutable bool mLengthDirty, mNormDirty;
#		endif

	public:
		inline Quaternion()
		{
#			if AURORA_CACHING_LEVEL >= 1
			mLength = mNorm = Real( 0.0 );
#			endif

			initValues(false);
			*mW = *mX = *mY = *mZ = Real( 0.0 );
		}

		inline Quaternion(Real W, Real X, Real Y, Real Z)
		{
			initValues(true);
			*mW = W, *mX = X, *mY = Y, *mZ = Z;
		}

		inline Quaternion(const Vector3D& Axis, Angle Ang)
		{
			initValues(true);
			Vector3D nAxis = Axis.getNormalized();
			Real halfAngle = Real(Radian(Ang)) / Real( 2.0 );

			*mW = Math::cos(halfAngle);
			Real coeff = Math::sin(halfAngle);
			*mX = nAxis.getX() * coeff;
			*mY = nAxis.getY() * coeff;
			*mZ = nAxis.getZ() * coeff;
		}

		inline Quaternion(const Quaternion& other)
		{
#			if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = other.mLengthDirty;
			if (!mLengthDirty) mLength = other.mLength;
			mNormDirty = other.mNormDirty;
			if (!mNormDirty) mNorm = other.mNorm;
#			endif

			mW = mComponents, mX = mW + 1, mY = mX + 1, mZ = mY + 1;
			memcpy(mComponents, other.mComponents, 4 * sizeof(Real));
		}

		inline Real getW()
		{
			return *mW;
		}

		inline Real getX()
		{
			return *mX;
		}

		inline Real getY()
		{
			return *mY;
		}

		inline Real getZ()
		{
			return *mZ;
		}

		inline void setW(Real W)
		{
#			if AURORA_CACHING_LEVEL
			mLengthDirty = mNormDirty = true;
#			endif

			*mW = W;
		}

		inline void setX(Real X)
		{
#			if AURORA_CACHING_LEVEL
			mLengthDirty = mNormDirty = true;
#			endif

			*mX = X;
		}

		inline void setY(Real Y)
		{
#			if AURORA_CACHING_LEVEL
			mLengthDirty = mNormDirty = true;
#			endif

			*mY = Y;
		}

		inline void setZ(Real Z)
		{
#			if AURORA_CACHING_LEVEL
			mLengthDirty = mNormDirty = true;
#			endif

			*mZ = Z;
		}

        void normalize();
		void normalizeApproximate();
		Quaternion getNormalized() const;
		Quaternion getNormalizedApproximate() const;

		void invert();
		Quaternion getInverse() const;
		void invertUnit();
		Quaternion getInverseUnit() const;

        Real getLength() const;
		Real getNorm() const;

		Matrix3 toRotationMatrix3() const;
		Matrix3 toRotationMatrix3Unit() const;
		Matrix4 toRotationMatrix4() const;
		Matrix4 toRotationMatrix4Unit() const;

		inline Quaternion operator + (const Quaternion& other) const
		{
#			if AURORA_SSE_ENABLED
			Quaternion q;
			SSE::packedAdd(mComponents, other.mComponents, q.mComponents);
			return q;
#			else
            return Quaternion(*mW + *other.mW, *mX + *other.mX, *mY + *other.mY, *mZ + *other.mZ);
#			endif
		}

		inline Quaternion operator - (const Quaternion& other) const
		{
#			if AURORA_SSE_ENABLED
			Quaternion q;
			SSE::packedSubtract(mComponents, other.mComponents, q.mComponents);
			return q;
#			else
            return Quaternion(*mW - *other.mW, *mX - *other.mX, *mY - *other.mY, *mZ - *other.mZ);
#			endif
        }

		inline Quaternion operator - () const
		{
			return Quaternion(-*mW, -*mX, -*mY, -*mZ);
		}

		inline Quaternion operator * (const Quaternion& other) const
		{
            // SSE's inefficient for operations like this. Sign changes. Lots of sign changes.
            // That means lots of XORs since there's no such instruction in SSE. And yeah - shuffles.
            // Lots of shuffles... bitches love shuffles. Unfortunately SSE does not.

            return Quaternion(
                *mW * *other.mW - *mX * *other.mX - *mY * *other.mY - *mZ * *other.mZ,
                *mW * *other.mX + *mX * *other.mW + *mY * *other.mZ - *mZ * *other.mY,
                *mW * *other.mY + *mY * *other.mW + *mZ * *other.mX - *mX * *other.mZ,
                *mW * *other.mZ + *mZ * *other.mW + *mX * *other.mY - *mY * *other.mX
            );
		}

		inline Quaternion operator * (Real Scalar) const
		{
#           if AURORA_SSE_ENABLED
            Quaternion q;
            SSE::packedMultiplyScalar(mComponents, Scalar, q.mComponents);
            return q;
#           else
            return Quaternion(*mW * Scalar, *mX * Scalar, *mY * Scalar, *mZ * Scalar);
#           endif
		}

		inline Vector3D operator * (const Vector3D& other) const
		{
			// TODO: See if this can be optimized
			// p' = q p q^-1
			Quaternion qVec(0, other.getX(), other.getY(), other.getZ());
			Quaternion r(*this * qVec * getInverse());
			return Vector3D(r.getX(), r.getY(), r.getZ());
		}

		inline Quaternion operator / (const Quaternion& other) const
		{
			return *this * other.getInverse();
		}

		inline Quaternion operator / (Real Scalar) const
		{
#           if AURORA_SSE_ENABLED
            Quaternion q;
            SSE::packedMultiplyScalar(mComponents, Real( 1.0 ) / Scalar, q.mComponents);
            return q;
#           else
            return Quaternion(*mW / Scalar, *mX / Scalar, *mY / Scalar, *mZ / Scalar);
#           endif
		}

		inline Quaternion operator += (const Quaternion& other)
		{
#			if AURORA_SSE_ENABLED
			SSE::packedAdd(mComponents, other.mComponents, mComponents);
#			else
			*mW += *other.mW;
			*mX += *other.mX;
			*mY += *other.mY;
			*mZ += *other.mZ;
#			endif

#			if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mNormDirty = true;
#			endif

			return *this;
		}

		inline Quaternion operator -= (const Quaternion& other)
		{
#			if AURORA_SSE_ENABLED
			SSE::packedSubtract(mComponents, other.mComponents, mComponents);
#			else
			*mW -= *other.mW;
			*mX -= *other.mX;
			*mY -= *other.mY;
			*mZ -= *other.mZ;
#			endif

#			if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mNormDirty = true;
#			endif

			return *this;
		}

		inline Quaternion operator *= (const Quaternion& other)
		{
			Real tmp[16];
            tmp[0] = *mW * *other.mW - *mX * *other.mX - *mY * *other.mY - *mZ * *other.mZ;
            tmp[1] = *mW * *other.mX + *mX * *other.mW - *mY * *other.mZ + *mZ * *other.mY;
            tmp[2] = *mW * *other.mY + *mX * *other.mZ + *mY * *other.mW - *mZ * *other.mX;
            tmp[3] = *mW * *other.mZ - *mX * *other.mY + *mY * *other.mX + *mZ * *other.mW;
			memcpy(mComponents, tmp, 4 * sizeof(Real));

#			if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mNormDirty = true;
#			endif

			return *this;

		}

		inline Quaternion operator *= (Real Scalar)
		{
#			if AURORA_CACHING_LEVEL >= 1
			SSE::packedMultiplyScalar(mComponents, Scalar, mComponents);
#			else
			*mW *= Scalar;
			*mX *= Scalar;
			*mY *= Scalar;
			*mZ *= Scalar;
#			endif

#			if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mNormDirty = true;
#			endif

			return *this;
		}

		inline Quaternion operator /= (const Quaternion& other)
		{
			return operator*=(other.getInverse());
		}

		inline Quaternion operator /= (Real Scalar)
		{
#			if AURORA_CACHING_LEVEL >= 1
			SSE::packedMultiplyScalar(mComponents, Real( 1.0 ) / Scalar, mComponents);
#			else
			*mW /= Scalar;
			*mX /= Scalar;
			*mY /= Scalar;
			*mZ /= Scalar;
#			endif

#			if AURORA_CACHING_LEVEL >= 1
			mLengthDirty = mNormDirty = true;
#			endif

			return *this;

		}

		inline bool operator== (const Quaternion& other) const
		{
			return *mW == *other.mW && *mX == *other.mX && *mY == *other.mY && *mZ == *other.mZ;
		}

	};
}

#endif // __AURORA_QUATERNION_H__

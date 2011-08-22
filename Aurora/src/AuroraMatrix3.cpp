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
#include "AuroraMatrix3.h"
using namespace Aurora;

/* Constant static members */
const Matrix3 Matrix3::Identity(
	Real( 1.0 ), Real( 0.0 ), Real( 0.0 ),
	Real( 0.0 ), Real( 1.0 ), Real( 0.0 ),
	Real( 0.0 ), Real( 0.0 ), Real( 1.0 )
	);

const Matrix3 Matrix3::Zero(
	Real( 0.0 )
	);


/* Member functions */

Real Matrix3::getDeterminant() const
{
#	if AURORA_CACHING_LEVEL >= 1
	if (!mDetDirty) return mDeterminant;
#	endif

	Real det = mComponents[LEA2D( 0, 0, 3 )] *
			  (mComponents[LEA2D( 1, 1, 3 )] * mComponents[LEA2D( 2, 2, 3 )]
			 - mComponents[LEA2D( 2, 1, 3 )] * mComponents[LEA2D( 1, 2, 3 )])
			 - mComponents[LEA2D( 1, 0, 3 )] *
			  (mComponents[LEA2D( 0, 1, 3 )] * mComponents[LEA2D( 2, 2, 3 )]
			 - mComponents[LEA2D( 2, 1, 3 )] * mComponents[LEA2D( 0, 2, 3 )])
			 + mComponents[LEA2D( 2, 0, 3 )] *
			  (mComponents[LEA2D( 0, 1, 3 )] * mComponents[LEA2D( 1, 2, 3 )]
			 - mComponents[LEA2D( 1, 1, 3 )] * mComponents[LEA2D( 0, 2, 3 )]);

#	if AURORA_CACHING_LEVEL >= 1
	mDetDirty = false;
	mDeterminant = det;
#	endif

	return det;
}

bool Matrix3::getInverse(Matrix3& Dest) const
{
	Real newMat[9];
	if (!inv3x3(newMat, mComponents)) return false;

	memcpy(Dest.mComponents, newMat, 9 * sizeof(Real));

#	if AURORA_CACHING_LEVEL >= 1
	Dest.mInvDirty = false;
	Dest.mDetDirty = true;
	memcpy(Dest.mInverse, mComponents, 9 * sizeof(Real));
#	endif

	return true;
}

void Matrix3::invert()
{
#	if AURORA_CACHING_LEVEL >= 1
	if (!mInvDirty)
	{
		// Swap the current matrix and the inverse
		// Actually there's a sweet swapping method using XOR that requires
		// no temporary memory; however it causes a slight loss in speed
		Real tmp[9];
		memcpy(tmp, mComponents, 9 * sizeof(Real));
		memcpy(mComponents, mInverse, 9 * sizeof(Real));
		memcpy(mInverse, tmp, 9 * sizeof(Real));

		// Okay, so... three memory copies of 36 bytes, totaling 108 bytes. Is this really
		// faster than a bunch of FPU instructions? Let's hope so. Because if not, the whole
		// caching thing is pointless.

		if (!mDetDirty)
			mDeterminant = Real( 1.0 ) / mDeterminant;

		return;
	}
#	endif

#	if AURORA_CACHING_LEVEL >= 1
	Real tmp[9];
	memcpy(tmp, mComponents, 9 * sizeof(Real));
#	endif

	Real res[9];

	inv3x3(res, mComponents);

	memcpy(mComponents, res, 9 * sizeof(Real));
#	if AURORA_CACHING_LEVEL >= 1
	memcpy(mInverse, tmp, 9 * sizeof(Real));
	mInvDirty = false;
	if (!mDetDirty)
		mDeterminant = Real( 1.0 ) / mDeterminant;

#	endif
}


Real Matrix3::minorDet(std::size_t Column, std::size_t Row, const Real *m) const
{
	std::size_t x1 = (Column == 0? 1 : 0),
		  y1 = (Row == 0? 1 : 0),
		  x2 = (Column == 2? 1 : 2),
		  y2 = (Row == 2? 1 : 2);

	return (m[LEA2D( x1, y1, 3 )] * m[LEA2D( x2, y2, 3)]
		  - m[LEA2D( x2, y1, 3 )] * m[LEA2D( x1, y2, 3)]);
}


bool Matrix3::inv3x3(Real *m, const Real *s) const
{
	if (!s) s = m;

	Real det = getDeterminant();
	if (Math::equal(det, 0.f))
		return false;

	Real invDet = Real( 1.0 ) / det;

	for (std::size_t x = 0; x < 3; ++x)
		for (std::size_t y = 0; y < 3; ++y)
		{
			// Note the order (y,x) in the call of minorDet(). This is because the rule
			// includes a cofactor of the transposed matrix
			// This may seem a little tricky, but actually negative entries all have
			// an odd coordinate sum
			if ((x+y)&1)
				m[LEA2D(x,y,3)] = minorDet(y, x, s) * -invDet;
			else m[LEA2D(x,y,3)] = minorDet(y, x, s) * invDet;
		}

	return true;
}

void Matrix3::trans3x3(Real *m, const Real *s) const
{
	if (!s) s = m;

	for (std::size_t x = 0; x < 3; ++x)
		for (std::size_t y = 0; y < 3; ++y)
			m[LEA2D(y,x,3)] = s[LEA2D(x,y,3)];
}

void Matrix3::transpose()
{
#	if AURORA_CACHING_LEVEL >= 1
	mInvDirty = mDetDirty = true;
#	endif

	Real pdest[9];
	trans3x3(pdest, mComponents);
	memcpy(mComponents, pdest, 9 * sizeof(Real));
}

Matrix3 Matrix3::getTransposed() const
{
	// This doesn't hit performance whatsoever
	Matrix3 mat(*this);
	mat.transpose();
	return mat;
}

/* Static member functions */
Matrix3 Matrix3::fromXRotation(Angle Theta)
{
	Real rad = Radian(Theta);
	Real cosTh = Math::cos(rad), sinTh = Math::sin(rad);
	return Matrix3(
		1,		0,		0,
		0,		cosTh,	-sinTh,
		0,		sinTh,	cosTh);
}

Matrix3 Matrix3::fromYRotation(Angle Theta)
{
	Real rad = Radian(Theta);
	Real cosTh = Math::cos(rad), sinTh = Math::sin(rad);
	return Matrix3(
		cosTh,	0,		sinTh,
		0,		1,		0,
	   -sinTh,	0,		cosTh);
}

Matrix3 Matrix3::fromZRotation(Angle Theta)
{
	Real rad = Radian(Theta);
	Real cosTh = Math::cos(rad), sinTh = Math::sin(rad);
	return Matrix3(
		cosTh,	-sinTh, 0,
		sinTh,	cosTh,	0,
		0,		0,		1);
}

Matrix3 Matrix3::fromXYZRotation(Angle X, Angle Y, Angle Z)
{
	// Building this matrix directly is faster than multiplying three matrices for X, Y and Z
	Real phi = Radian(X), theta = Radian(Y), psi = Radian(Z);
	Real sinTh = Math::sin(theta), cosTh = Math::cos(theta),
		sinPh = Math::sin(phi), cosPh = Math::cos(phi),
		sinPs = Math::sin(psi), cosPs = Math::cos(psi);

	return Matrix3(
		cosTh * cosPs,
		-cosPh * sinPs + sinPh * sinTh * cosPs,
		sinPh * sinPs + cosPh * sinTh * cosPs,
		cosTh * sinPs,
		cosPh * cosPs + sinPh * sinTh * sinPs,
		-sinPh * cosPs + cosPh * sinTh * sinPs,
		-sinTh,
		sinPh * cosTh,
		cosPh * cosTh);
}

Matrix3 Matrix3::fromScale(Real X, Real Y, Real Z)
{
	return Matrix3(
		X, 0, 0,
		0, Y, 0,
		0, 0, Z);
}

Matrix3 Matrix3::fromAngleAxis(const Vector3D& Axis, Angle Theta)
{
	Real x = Axis.getX(), y = Axis.getY(), z = Axis.getZ();
	Real sinTh = Math::sin(Radian(Theta)), cosTh = Math::cos(Radian(Theta)), oneMCosTh = 1 - cosTh;
	Real xy = x*y*oneMCosTh, xz = x*z*oneMCosTh, yz = y*z*oneMCosTh;

	return Matrix3(
		cosTh + x * x * oneMCosTh, xy - z * sinTh, xz + y * sinTh,
		xy + z * sinTh,	cosTh + y * y * oneMCosTh, yz - x * sinTh,
		xz - y * sinTh, yz + x * sinTh, cosTh + z * z * oneMCosTh);
}

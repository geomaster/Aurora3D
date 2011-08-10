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
#include "AuroraMatrix4.h"
using namespace Aurora;

/* Constant static members */
const Matrix4 Matrix4::Identity(
	Real( 1.0 ), Real( 0.0 ), Real( 0.0 ), Real( 0.0 ),
	Real( 0.0 ), Real( 1.0 ), Real( 0.0 ), Real( 0.0 ),
	Real( 0.0 ), Real( 0.0 ), Real( 1.0 ), Real( 0.0 ),
	Real( 0.0 ), Real( 0.0 ), Real( 0.0 ), Real( 1.0 )
	);

const Matrix4 Matrix4::Zero(
	Real( 0.0 )
	);

/* Constructors */

Matrix4::Matrix4(const Matrix3& other)
{
#	if AURORA_CACHING_LEVEL >= 1
	mDetDirty = mInvDirty = true;
#	endif

	for (std::size_t x = 0; x < 4; ++x)
		for (std::size_t y = 0; y < 4; ++y)
		{
			Real r = ((x < 3 && y < 3)? other[y][x] : (x==y&&x==3? Real( 1.0 ) : Real( 0.0 )));
			mComponents[LEA2D( x, y, 4 )] = r;
		}
}

/* Member functions */
Real Matrix4::getDeterminant() const
{
#	if AURORA_CACHING_LEVEL >= 1
	if (!mDetDirty) return mDeterminant;
#	endif

	Real det = mComponents[0] * minorDet(0, 0, mComponents) -
		mComponents[1] * minorDet(1, 0, mComponents) +
		mComponents[2] * minorDet(2, 0, mComponents) -
		mComponents[3] * minorDet(3, 0, mComponents);

#	if AURORA_CACHING_LEVEL >= 1
	mDetDirty = false;
	mDeterminant = det;
#	endif

	return det;
}

bool Matrix4::getInverse(Matrix4& Dest) const
{
	Real newMat[16];
	if (!inv4x4(newMat, mComponents)) return false;

	memcpy(Dest.mComponents, newMat, 16 * sizeof(Real));

#	if AURORA_CACHING_LEVEL >= 1
	Dest.mInvDirty = false;
	Dest.mDetDirty = true;
	memcpy(Dest.mInverse, mComponents, 16 * sizeof(Real));
#	endif

	return true;
}

void Matrix4::invert()
{
#	if AURORA_CACHING_LEVEL >= 1
	if (!mInvDirty)
	{
		// Swap the current matrix and the inverse
		// Actually there's a sweet swapping method using XOR that requires
		// no temporary memory; however it causes a slight loss in speed
		Real tmp[16];
		memcpy(tmp, mComponents, 16 * sizeof(Real));
		memcpy(mComponents, mInverse, 16 * sizeof(Real));
		memcpy(mInverse, tmp, 16 * sizeof(Real));

		// Okay, so... three memory copies of 64 bytes, totaling 192 bytes. Is this really
		// faster than a bunch of FPU instructions? Let's hope so. Because if not, the whole
		// caching thing is pointless.

		if (!mDetDirty)
			mDeterminant = Real( 1.0 ) / mDeterminant;

		return;
	}
#	endif

#	if AURORA_CACHING_LEVEL >= 1
	Real tmp[16];
	memcpy(tmp, mComponents, 16 * sizeof(Real));
#	endif

	Real res[16];

	inv4x4(res, mComponents);

	memcpy(mComponents, res, 16 * sizeof(Real));
#	if AURORA_CACHING_LEVEL >= 1
	memcpy(mInverse, tmp, 16 * sizeof(Real));
	mInvDirty = false;
	if (!mDetDirty)
		mDeterminant = Real( 1.0 ) / mDeterminant;

#	endif
}

Real Matrix4::minorDet(std::size_t Column, std::size_t Row, const Real *m) const
{
	std::size_t x0 = (Column == 0? 1 : 0),
		x1 = (x0 == 0? (Column == 1? 2 : 1) : 2),
		x2 = (x1 == 1? (Column == 2? 3 : 2) : 3),
		y0 = (Row == 0? 1 : 0),
		y1 = (y0 == 0? (Row == 1? 2 : 1) : 2),
		y2 = (y1 == 1? (Row == 2? 3 : 2) : 3);

	return (m[LEA2D( x0, y0, 4 )] *
			  (m[LEA2D( x1, y1, 4 )] * m[LEA2D( x2, y2, 4 )]
			 - m[LEA2D( x2, y1, 4 )] * m[LEA2D( x1, y2, 4 )])
			 - m[LEA2D( x1, y0, 4 )] *
			  (m[LEA2D( x0, y1, 4 )] * m[LEA2D( x2, y2, 4 )]
			 - m[LEA2D( x2, y1, 4 )] * m[LEA2D( x0, y2, 4 )])
			 + m[LEA2D( x2, y0, 4 )] *
			  (m[LEA2D( x0, y1, 4 )] * m[LEA2D( x1, y2, 4 )]
			 - m[LEA2D( x1, y1, 4 )] * m[LEA2D( x0, y2, 4 )]));

}


bool Matrix4::inv4x4(Real *m, const Real *s) const
{
	if (!s) s = m;

	Real det = getDeterminant();
	if (Math::equal(det, 0.f))
		return false;

	Real invDet = Real( 1.0 ) / det;

	for (std::size_t x = 0; x < 4; ++x)
		for (std::size_t y = 0; y < 4; ++y)
		{
			// Note the order (y,x) in the call of minorDet(). This is because the rule
			// includes a cofactor of the transposed matrix
			// This may seem a little tricky, but actually negative entries all have
			// an odd coordinate sum
			if ((x+y)&1)
				m[LEA2D(x,y,4)] = minorDet(y, x, s) * -invDet;
			else m[LEA2D(x,y,4)] = minorDet(y, x, s) * invDet;
		}

	return true;
}

void Matrix4::trans4x4(Real *m, const Real *s) const
{
	if (!s) s = m;

	for (std::size_t x = 0; x < 4; ++x)
		for (std::size_t y = 0; y < 4; ++y)
			m[LEA2D(y,x,4)] = s[LEA2D(x,y,4)];
}

void Matrix4::transpose()
{
#	if AURORA_CACHING_LEVEL >= 1
	mInvDirty = mDetDirty = true;
#	endif

	Real pdest[16];
	trans4x4(pdest, mComponents);
	memcpy(mComponents, pdest, 16 * sizeof(Real));
}

Matrix4 Matrix4::getTransposed() const
{
	// This doesn't hit performance whatsoever
	Matrix4 mat(*this);
	mat.transpose();
	return mat;
}

/* Static member functions */
Matrix4 Matrix4::fromXRotation(Angle Theta)
{
	return Matrix4(Matrix3::fromXRotation(Theta));
}

Matrix4 Matrix4::fromYRotation(Angle Theta)
{
	return Matrix4(Matrix3::fromYRotation(Theta));
}

Matrix4 Matrix4::fromZRotation(Angle Theta)
{
	return Matrix4(Matrix3::fromZRotation(Theta));
}

Matrix4 Matrix4::fromXYZRotation(Angle X, Angle Y, Angle Z)
{
	return Matrix4(Matrix3::fromXYZRotation(X, Y, Z));
}

Matrix4 Matrix4::fromScale(Real X, Real Y, Real Z)
{
	return Matrix4(
		X, 0, 0, 0,
		0, Y, 0, 0,
		0, 0, Z, 0,
		0, 0, 0, 1);
}

Matrix4 Matrix4::fromAngleAxis(const Vector3D& Axis, Angle Theta)
{
	return Matrix4(Matrix3::fromAngleAxis(Axis, Theta));
}

Matrix4 Matrix4::fromTranslation(Real X, Real Y, Real Z)
{
	return Matrix4(
		1, 0, 0, X,
		0, 1, 0, Y,
		0, 0, 1, Z,
		0, 0, 0, 1);
}

Matrix4 Matrix4::fromTranslation(const Vector3D& Translation)
{
	return fromTranslation(Translation.getX(), Translation.getY(), Translation.getZ());
}

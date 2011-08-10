#include "AuroraSSE.h"
#if AURORA_SSE_ENABLED

using namespace Aurora;
#include <xmmintrin.h>

void SSE::packedAdd(const float *a, const float *b, float *r)
{
#	if AURORA_SSE_ENABLED
	_mm_store_ps(r, _mm_add_ps(_mm_load_ps(a), _mm_load_ps(b)));
#	else
	r[0] = a[0] + b[0];
	r[1] = a[1] + b[1];
	r[2] = a[2] + b[2];
	r[3] = a[3] + b[3];
#	endif
}

void SSE::packedSubtract(const float *a, const float *b, float *r)
{
#	if AURORA_SSE_ENABLED
	_mm_store_ps(r, _mm_sub_ps(_mm_load_ps(a), _mm_load_ps(b)));
#	else
	r[0] = a[0] - b[0];
	r[1] = a[1] - b[1];
	r[2] = a[2] - b[2];
	r[3] = a[3] - b[3];
#	endif
}

void SSE::packedMultiply(const float *a, const float *b, float *r)
{
#	if AURORA_SSE_ENABLED
	_mm_store_ps(r, _mm_mul_ps(_mm_load_ps(a), _mm_load_ps(b)));
#	else
	r[0] = a[0] * b[0];
	r[1] = a[1] * b[1];
	r[2] = a[2] * b[2];
	r[3] = a[3] * b[3];
#	endif
}

void SSE::packedDivide(const float *a, const float *b, float *r)
{
#	if AURORA_SSE_ENABLED
	_mm_store_ps(r, _mm_div_ps(_mm_load_ps(a), _mm_load_ps(b)));
#	else
	r[0] = a[0] / b[0];
	r[1] = a[1] / b[1];
	r[2] = a[2] / b[2];
	r[3] = a[3] / b[3];
#	endif
}

void SSE::packedReciprocal(const float *a, float *r)
{
#	if AURORA_SSE_ENABLED
	_mm_store_ps(r, _mm_rcp_ps(_mm_load_ps(a)));
#	else
	r[0] = 1.f / a[0];
	r[1] = 1.f / a[1];
	r[2] = 1.f / a[2];
	r[3] = 1.f / a[3];
#	endif
}

void SSE::packedSqrt(const float *a, float *r)
{
#	if AURORA_SSE_ENABLED
	_mm_store_ps(r, _mm_sqrt_ps(_mm_load_ps(a)));
#	else
	r[0] = Math::sqrt(a[0]);
	r[1] = Math::sqrt(a[1]);
	r[2] = Math::sqrt(a[2]);
	r[3] = Math::sqrt(a[3]);
#	endif
}

void SSE::packedReciprocalSqrt(const float *a, float *r)
{
#	if AURORA_SSE_ENABLED
	_mm_store_ps(r, _mm_rsqrt_ps(_mm_load_ps(a)));
#	else
	r[0] = 1.f / Math::sqrt(a[0]);
	r[1] = 1.f / Math::sqrt(a[1]);
	r[2] = 1.f / Math::sqrt(a[2]);
	r[3] = 1.f / Math::sqrt(a[3]);
#	endif
}

bool SSE::packedEqual(const float *a, const float *b)
{
    // TODO: See how we can enable this for GCC (mingw included) and ICC
#	if AURORA_SSE_ENABLED && AURORA_COMPILER == AURORA_COMPILER_MSVC // sorry about this, gcc. i feel for you. i really do.
	__m128 res = _mm_cmpneq_ps(_mm_load_ps(a), _mm_load_ps(b));
	return (!res.m128_u64[0] && !res.m128_u64[0]);
#	else
	return (a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3]);
#	endif
}

void SSE::packedAddM4(const float *m1, const float *m2, float *r)
{
#	if AURORA_SSE_ENABLED
	__m128 row1, row2;

	for (std::size_t row = 0; row < 16; row += 4)
	{
		row1 = _mm_load_ps(m1);
		row2 = _mm_load_ps(m2);
		_mm_store_ps(r, _mm_sub_ps(row1, row2));
	}
#	else
	for (uint i = 0; i < 16; ++i)
		r[i] = m1[i] + m2[i];
#	endif
}

void SSE::packedSubtractM4(const float *m1, const float *m2, float *r)
{
#	if AURORA_SSE_ENABLED
	__m128 row1, row2;

	for (std::size_t row = 0; row < 16; row += 4)
	{
		row1 = _mm_load_ps(m1);
		row2 = _mm_load_ps(m2);
		_mm_store_ps(r, _mm_add_ps(row1, row2));
	}
#	else
	for (uint i = 0; i < 16; ++i)
		r[i] = m1[i] - m2[i];
#	endif
}

void SSE::packedMultiplyM4(const float* m1, const float* m2, float *r)
{
#	if AURORA_SSE_ENABLED
	__m128 m2Row, m1Scal, result;

	for (std::size_t i = 0; i < 16; i += 4)
	{
		m2Row = _mm_load_ps(m2);
		m1Scal = _mm_set1_ps(m1[i]);
		result = _mm_mul_ps(m2Row, m1Scal);

		for (int j = 1; j < 4; j++)
		{
			m2Row = _mm_load_ps(m2 + j*4);
			m1Scal = _mm_set1_ps(m1[i+j]);
			result = _mm_add_ps(_mm_mul_ps(m2Row, m1Scal), result);
		}
		_mm_store_ps(r + i, result);
	}

#	else
	{
		for (std::size_t x = 0; x < 4; ++x)
		{
			r[LEA2D( x, y, 4 )] =
				m1[LEA2D( 0, y, 4 )] * m2[LEA2D( x, 0, 4 )] +
				m1[LEA2D( 1, y, 4 )] * m2[LEA2D( x, 1, 4 )] +
				m1[LEA2D( 2, y, 4 )] * m2[LEA2D( x, 2, 4 )] +
				m1[LEA2D( 3, y, 4 )] * m2[LEA2D( x, 3, 4 )];
		}
#	endif
}

bool SSE::packedEqualM4(const float *m1, const float *m2)
{
    // TODO: Check avail for gcc, see previous todos
#	if AURORA_SSE_ENABLED && AURORA_COMPILER == AURORA_COMPILER_MSVC
	for (std::size_t i = 0; i < 16; i += 4)
	{
		__m128 cmp = _mm_cmpneq_ps(_mm_load_ps(m1+i), _mm_load_ps(m2+i));
		if (cmp.m128_u64[0] || cmp.m128_u64[1]) return false;
	}
	return true;
#	else
	for (std::size_t i = 0; i < 16; ++i)
		if (m1[i] != m2[i]) return false;
	return true;
#	endif
}

/*
void SSE::packedMultiplyM4Vec(const float *m, const float *v, float *r)
{
	__m128 mCol, scalar, res, rcp, sc1;
	AURORA_SSE_ALIGN float column[4];

	column[0] = m[15];
	column[1] = m[11];
	column[2] = m[7];
	column[3] = m[3];
	res = _mm_load_ps(column);

	for (int i = 2; i >= 0; --i)
	{
		column[0] = m[12+i];
		column[1] = m[8+i];
		column[2] = m[4+i];
		column[3] = m[i];
		mCol = _mm_load_ps(column);

		scalar = _mm_set1_ps(v[i]);
		res = _mm_add_ps(res, _mm_mul_ps(scalar, mCol));
	}

	rcp = _mm_rcp_ss(res);
	sc1 = _mm_shuffle_ps(rcp, rcp, 0);
	_mm_store_ps(r, _mm_mul_ps(_mm_shuffle_ps(res, res, 0x1B), sc1));
}
*/

void SSE::packedNormalizeV4Approx(const float *v, float lengthSquared, float *r)
{
	__m128 vec = _mm_load_ps(v);
	__m128 invLength = _mm_load_ss(&lengthSquared);
	invLength = _mm_rsqrt_ss(invLength);
	invLength = _mm_shuffle_ps(invLength, invLength, 0x0);
	_mm_store_ps(r, _mm_mul_ps(vec, invLength));
}

/*
void SSE::packedMultiplyQuaternion(const float *q1, const float *q2, float *r)
{
	AURORA_SSE_ALIGN float null[4] = { 0.f, -0.f, 0.f, -0.f };

	__m128 a = _mm_set1_ps(*q1);
	__m128 b = _mm_load_ps(q2);
	__m128 sign = _mm_load_ps(null);
	__m128 result;

	// r = q1[0] * (q2[0], q2[1], q2[2], q2[3])
	result = _mm_mul_ps(a, b);

	// r += q1[3] * (-q2[3], q2[2], -q2[1], q2[0])
	a = _mm_set1_ps(q1[3]);
	b = _mm_xor_ps(b, sign);
	b = _mm_shuffle_ps(b, b, 0x1B); // 00 01 10 11, reverse
	result = _mm_add_ps(_mm_mul_ps(a, b), result);

	// r += q1[1] * (-q2[1], q2[0], q2[3], -q2[2])
	a = _mm_set1_ps(q1[1]);
	sign = _mm_shuffle_ps(sign, sign, 0x50); // 01 01 00 00, get ++--
	b = _mm_shuffle_ps(b, b, 0x4E); // 01 00 11 10
	b = _mm_xor_ps(b, sign);
	result = _mm_add_ps(_mm_mul_ps(a, b), result);

	// r += q1[2] * (-q2[2], -q2[3], q2[0], q2[1])
	a = _mm_set1_ps(q1[1]);
	sign = _mm_shuffle_ps(sign, sign, 0xCC); // 11 00 11 00, get +-+-
	b = _mm_shuffle_ps(b, b, 0x1B); // 00 01 10 11, reverse
	b = _mm_xor_ps(b, sign);
	result = _mm_add_ps(_mm_mul_ps(a, b), result);

	_mm_store_ps(r, result);
}
*/

#endif

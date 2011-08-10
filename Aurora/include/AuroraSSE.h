#ifndef __AURORA_SSE_H__
#define __AURORA_SSE_H__
#include "AuroraPrereqs.h"
#if AURORA_SSE_ENABLED
#include "AuroraMath.h"
#include <xmmintrin.h>

#define movaps_load(from)       _mm_load_ps(from)
#define movaps_set(to, from)    _mm_store_ps(to, from)
#define addps(a, b)             _mm_add_ps(a, b)
#define subps(a, b)             _mm_sub_ps(a, b)
#define mulps(a, b)             _mm_mul_ps(a, b)
#define divps(a, b)             _mm_div_ps(a, b)
#define rcpps(a)                _mm_rcp_ps(a)
#define sqrtps(a)               _mm_sqrt_ps(a)
#define rsqrtps(a)              _mm_rsqrt_ps(a)
#define movaps_shuffle(a)       _mm_set1_ps(a)
#define shufps(a, b, imm)       _mm_shuffle_ps(a, b, imm)
#define movss_load(from)        _mm_load_ss(from)
#define rsqrtss(a)              _mm_rsqrt_ss(a)

namespace Aurora
{
	class AURORA_LIBRARY SSE
	{
	public:
		static void packedAdd(const float *a, const float *b, float *r);
		static void packedSubtract(const float *a, const float *b, float *r);
		static void packedMultiply(const float *a, const float *b, float *r);
		static void packedDivide(const float *a, const float *b, float *r);
		static void packedReciprocal(const float *a, float *r);
		static void packedSqrt(const float *a, float *r);
		static void packedReciprocalSqrt(const float *a, float *r);
		static bool packedEqual(const float *a, const float *b);
		static void packedNormalizeV4Approx(const float *v, float lengthSquared, float *r);
        static void packedMultiplyScalar(const float *a, float S, float *r)
		{
			movaps_set(r, mulps(movaps_shuffle(S), movaps_load(a)));
		}

		static void packedAddM4(const float* m1, const float *m2, float *r);
		static void packedSubtractM4(const float* m1, const float *m2, float *r);
		static void packedMultiplyM4(const float* m1, const float *m2, float *r);
		static bool packedEqualM4(const float *m1, const float *m2);


		//static void packedMultiplyM4Vec(const float *m, const float *v, float *r);
	};
}

#endif // AURORA_SSE_ENABLED
#endif // __AURORA_SSE_H__

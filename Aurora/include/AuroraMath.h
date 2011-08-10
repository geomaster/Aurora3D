#ifndef __AURORA_MATH_H__
#define __AURORA_MATH_H__
#include "AuroraPrereqs.h"
#include "AuroraConfig.h"
#include <cmath>

#define DEFAULT_OP(cls, op, dataval) \
	cls operator op (const cls &other) \
	{ \
		return cls( dataval op other. dataval ); \
	} \
	cls operator op (Real Scalar) \
	{ \
		return cls ( dataval op Scalar ); \
	}

#define DEFAULT_ASSIGNOP(cls, op, dataval) \
	cls operator op (const cls &other) \
	{ \
		dataval op other. dataval; \
		return *this; \
	} \
	cls operator op (Real Scalar) \
	{ \
		dataval op Scalar; \
		return *this; \
	}

#define DEFAULT_COMPOP(cls, op, dataval) \
	bool operator op (const cls &other) \
	{ \
		return dataval op other. dataval; \
	} \
	bool operator op (Real Scalar) \
	{ \
		return dataval op Scalar; \
	}

#define DEFAULT_OPERATORS(cls, dataval) \
	DEFAULT_OP(cls, +, dataval) \
	DEFAULT_OP(cls, -, dataval) \
	DEFAULT_OP(cls, *, dataval) \
	DEFAULT_OP(cls, /, dataval) \
	\
	DEFAULT_ASSIGNOP(cls, =, dataval) \
	DEFAULT_ASSIGNOP(cls, +=, dataval) \
	DEFAULT_ASSIGNOP(cls, -=, dataval) \
	DEFAULT_ASSIGNOP(cls, /=, dataval) \
	DEFAULT_ASSIGNOP(cls, *=, dataval) \
	\
	cls operator-() \
	{ \
		dataval = -dataval; \
		return *this; \
	} \
	\
	DEFAULT_COMPOP(cls, <, dataval) \
	DEFAULT_COMPOP(cls, <=, dataval) \
	DEFAULT_COMPOP(cls, >, dataval) \
	DEFAULT_COMPOP(cls, >=, dataval) \
	DEFAULT_COMPOP(cls, ==, dataval) \
	DEFAULT_COMPOP(cls, !=, dataval)

namespace Aurora
{
	const Real RadianToDegree = static_cast<Real>(57.295779513082320876798154814105);
	const Real DegreeToRadian = static_cast<Real>(0.01745329251994329576923690768489);
	const Real Pi = static_cast<Real>(3.1415926535897932384626433832795);
	const Real HalfPi = static_cast<Real>(1.5707963267948966192313216916398);
	const Real TwoPi = static_cast<Real>(6.283185307179586476925286766559);
	const Real Log2 = static_cast<Real>(0.30102999566398119521373889472449);
	const Real Sqr2 = static_cast<Real>(1.4142135623730950488016887242097);
	const Real Sqr3 = static_cast<Real>(1.7320508075688772935274463415059);

	class AURORA_LIBRARY Radian
	{
	protected:
		Real mData;

	public:
		explicit Radian(Real r) : mData(r) { }
		Radian() : mData(0.f) { }
		Radian(const Radian& other) : mData(other.mData) { }

		operator Real()
		{
			return mData;
		}

		DEFAULT_OPERATORS(Radian, mData)
	};

	class AURORA_LIBRARY Degree
	{
	protected:
		Real mData;

	public:
		explicit Degree(Real r) : mData(r) { }
		Degree() : mData(0.f) { }
		Degree(const Degree& other) : mData(other.mData) { }

		operator Real()
		{
			return mData;
		}

		DEFAULT_OPERATORS(Degree, mData)
	};

	// TODO: Make this class able to use default angle unit
	class AURORA_LIBRARY Angle
	{
	protected:
		Real mAngle;

	public:
		Angle(Degree deg) : mAngle(deg * DegreeToRadian)
		{
		}

		Angle(Radian rad) : mAngle(rad)
		{
		}

		operator Degree() { return Degree(mAngle * RadianToDegree); }
		operator Radian() { return Radian(mAngle); }
	};

	class AURORA_LIBRARY Math
	{
	public:
		static inline Real cos (Real v)
		{
			return ::cos(v);
		}

		static inline Real sin (Real v)
		{
			return ::sin(v);
		}

		static inline Real tan (Real v)
		{
			return ::tan(v);
		}

		static inline Real acos(Real v)
		{
			return ::acos(v);
		}

		static inline Real asin(Real v)
		{
			return ::asin(v);
		}

		static inline Real atan(Real v)
		{
			return ::atan(v);
        }

		static inline Real sqrt(Real v)
		{
			return ::sqrt(v);
		}

		static inline int compare(Real a, Real b, Real tolerance = AURORA_FPTOLERANCE)
		{
			Real d = fabs(b - a);
			if (d <= tolerance)
				return 0;
			return (d<0.0 ? -1 : 1);
		}

		static inline bool equal(Real a, Real b, Real tolerance = AURORA_FPTOLERANCE)
		{
			return compare(a, b, tolerance) == 0;
		}

		static inline bool greater(Real a, Real b, Real tolerance = AURORA_FPTOLERANCE)
		{
			return compare(a, b, tolerance) >= 1;
		}

		static inline bool greaterOrEqual(Real a, Real b, Real tolerance = AURORA_FPTOLERANCE)
		{
			return compare(a, b, tolerance) >= 0;
		}

		static inline bool less(Real a, Real b, Real tolerance = AURORA_FPTOLERANCE)
		{
			return compare(a, b, tolerance) <= -1;
		}

		static inline bool lessOrEqual(Real a, Real b, Real tolerance = AURORA_FPTOLERANCE)
		{
			return compare(a, b, tolerance) <= 0;
		}

		static inline bool notEqual(Real a, Real b, Real tolerance = AURORA_FPTOLERANCE)
		{
			return compare(a, b, tolerance) != 0;
		}

		// Fast coarse approximation of 1 / sqrt(x)
		// note that PrecisionLevel should be either 0 or 1
		static Real rsqrt_e(Real v, int PrecisionLevel = 1);

		template<typename T>
		T lerp(const T& a, const T& b, Real t)
		{
			return T(t * a + (1 - t) * b);
		}
	};
}
#endif

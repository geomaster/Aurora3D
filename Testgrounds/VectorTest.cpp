#include <AuroraVector4D.h>
#include <cstdlib>
using namespace Aurora;

void dumpVec4(const Vector4D& v)
{
	printf("v = (%f, %f, %f, %f)\n", v.getX(), v.getY(), v.getZ(), v.getW());
}

int main()
{
	/*
	Vector2D v1(1.f, 2.f);
	Vector2D v2(5.f);
	Vector2D v3(0.f);
	Real allOnes[] = { 1.f, 1.f };
	Real randomness[] = { 22.f, 33.f };
	
	Vector2D v4(allOnes);
	Vector2D v5(randomness);
	Vector2D v6;

	Vector2D ang1(0.f, 10.f), ang2(1.f, 1.f);
	ang1.normalize();
	ang2.normalize();

	printf("len %f %f\n", ang1.getLength(), ang2.getLength());

	printf("|v1| = %f, |v2] = %f, |v3| = %f,\n |v4| = %f, |v5| = %f, |v6| = %f\n",
		v1.getLength(), v2.getLength(), v3.getLength(), v4.getLength(), v5.getLength(), v6.getLength());

	printf("|v1|2 = %f, |v2]2 = %f, |v3|2 = %f,\n |v4|2 = %f, |v5|2 = %f, |v6|2 = %f\n",
		v1.getLengthSquared(), v2.getLengthSquared(), v3.getLengthSquared(), v4.getLengthSquared(), v5.getLengthSquared(), v6.getLengthSquared());

	v5.normalize();
	printf("v5 = (%f, %f)\n", v5.getX(), v5.getY());

	printf("v1 to v2: %f\n", Real(Degree(ang1.getAngleTo(ang2))));
	
	Vector2D t1(4.f, 7.f);
	float l = t1.getLengthSquared();

	printf("t1 l**2=%f l=%f rl=%f\n", l, Math::sqrt(l), t1.getLength());

	t1.setX(20.f);
	l = t1.getLength();
	printf("t1 l**2=%f rl**2=%f l=%f\n", t1.getLengthSquared(), l*l, l);
	*/
	/*
	Vector3D v1(5.f, 0.f, 0.f);
	Vector3D v2(0.f, 0.f, 12.f);

	Vector3D result = v2.crossProduct(v1);
	printf("result = (%f, %f, %f)\n", result.getX(), result.getY(), result.getZ());

	Vector3D a1(0.f, 1.f, 0.f), a2(0.f, 1.f, 1.f), a3(1.f, 0.f, 0.f);
	printf("y to yz = %f, y to x = %f, x to yz = %f\n",
		Real(Degree(a2.getAngleTo(a1))),
		Real(Degree(a1.getAngleTo(a3))),
		Real(Degree(a2.getAngleTo(a3))));*/

	Vector4D op1(4.f, -5.f, 9.f, 2.f);
	Vector4D op2(-9.f, 7.f, 4.f, 3.f);
	
	Vector4D res(op1+op2);
	
	float *a, *b, *c;
	c = new float[4];
	a = new float[4];
	b = new float[4];

	a[0] = 1.f;
	a[1] = 2.f;
	a[2] = 3.f;
	a[3] = 4.f;

	b[0] = 4.f;
	b[1] = 5.f;
	b[2] = 6.f;
	b[3] = 7.f;

	__asm
	{
		mov eax, a;
		mov ebx, b;

		movups xmm0, [eax];
		movups xmm1, [ebx];
		addps xmm0, xmm1;
		mov eax, c;
		movups [eax], xmm0;
	}
	float fa = 1.f, fb = 3.f, fc;
	fc=fa+fb;
	printf("manual add %f %f %f %f\n", c[0],c[1],c[2],c[3]);
	SSE::packedDivide(a,b,c);
	printf("SSE::packedDivide %f %f %f %f\n", c[0],c[1],c[2],c[3]);
	dumpVec4(res);
	dumpVec4(op1+op2);
	dumpVec4(op1-op2);
	dumpVec4(op1*op2);
	dumpVec4(op1/op2);
	dumpVec4(op1*4);
	dumpVec4(op2*-3);

	system("pause >nul");
	return 0;
}
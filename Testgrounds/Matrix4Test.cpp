#include <AuroraMatrix4.h>
#include <AuroraVector3D.h>
#include <AuroraSSE.h>
#include <emmintrin.h>

#include <cstdlib>
#include "WinTimer.h"

using namespace Aurora;

void dmpMat4(const Matrix4& mat)
{
	for (std::size_t x = 0; x < 4; ++x)
	{
		for (std::size_t y = 0; y < 4; ++y)
			printf("%.3f ", mat[y][x]);
		printf("\n");
	}
}

void
mmul_sse(const float * a, const float * b, float * r)
{
  __m128 a_line, b_line, r_line;
  for (int i=0; i<16; i+=4) {
    // unroll the first step of the loop to avoid having to initialize r_line to zero
    a_line = _mm_load_ps(b);         // a_line = vec4(column(a, 0))
    b_line = _mm_set1_ps(a[i]);      // b_line = vec4(b[i][0])
    r_line = _mm_mul_ps(a_line, b_line); // r_line = a_line * b_line
    for (int j=1; j<4; j++) {
      a_line = _mm_load_ps(&b[j*4]); // a_line = vec4(column(a, j))
      b_line = _mm_set1_ps(a[i+j]);  // b_line = vec4(b[i][j])
                                     // r_line += a_line * b_line
      r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
    }
    _mm_store_ps(&r[i], r_line);     // r[i] = r_line
  }
}


void benchmarkSSE()
{
	Timer t;
	Matrix4 a (1.f, 2.f, 3.f, 4.f,
			   5.f, 6.f, 7.f, 8.f,
			   9.f, 0.1f, 0.2f, 0.3f,
			   0.4f, 0.4f, 0.4f, 0.4f);;

	Matrix4 b /*(0.7f, 0.f, 6.f, 0.f,
			   0.1f, 10.f, 8.f, 9.f,
			   5.f, 7.f, 9.f, 0.f,
			   1.f, 2.f, 9.f, 4.f);*/(Matrix4::Identity);

	Matrix4 r1=a*b;
	t.startCounting();

	for (std::size_t i = 0; i < 20000000; ++i)
	{
		Matrix4 l(a+b);
	}
	double t1 = t.stopCounting();

	t.startCounting();
	__declspec(align(16)) float r[16];
	for (std::size_t i = 0; i < 20000000; ++i)
	{
		SSE::packedAddM4(a.getTransposed()[0], b[0], r);
		//mmul_sse(a[0], b[0], r);
		//mmul_aurorasse(a[0], b[0], r);
		Matrix4 k(r);
	}
	double t2 = t.stopCounting();

	printf ("normal mul: %f, sse: %f\n", t1, t2);
}

int main()
{
	/*
	dmpMat4(Matrix4::Identity);
	printf("\n");

	Matrix4 idt = Matrix4::Identity;
	idt.invert();
	dmpMat4(idt);
	printf("\n");

	Matrix4 shouldBeIdt = idt * Matrix4::Identity;
	dmpMat4(shouldBeIdt);
	printf("\n");

	Real det = Matrix4::Identity.getDeterminant();
	printf("idt det: %f\n", det);

	Matrix4 yaw = Matrix4::fromYRotation(Degree(45));
	Matrix4 transl = Matrix4::Identity; //Matrix4::fromTranslation(0, 5, 0);
	Matrix4 combined = yaw * transl;
	dmpMat4(combined);
	printf("mat1 det: %f\n\n", combined.getDeterminant());

	Vector3D trans1 (0, 0, 1);
	trans1 = trans1 * combined;
	printf("(%f %f %f)\n\n", trans1.getX(), trans1.getY(), trans1.getZ());

	Matrix4 combinedInv (combined);
	combinedInv.invert();
	combinedInv.setEntry(0, 0, combinedInv[0][0]);
	dmpMat4(combinedInv);
	printf("matinv det: %f\n\n", combinedInv.getDeterminant());

	printf("\n");

	dmpMat4(combinedInv*combined);
	printf("\n");

	float myMat[16];
	Matrix4 x1(1, 2, 0, 3,
		0, 2, 2, 1,
		9, 5, 5, 0,
		4, 4, 9, 8);
	Matrix4 x2(6, 1, 5, 2,
		0, 2, 6, 0,
		9, 4, 5, 0,
		0, 0, 8, 8);

	Matrix4 realR = x1*x2;
	SSE::packedMultiplyM4(x1[0], x2[0], myMat);
	Matrix4 sseR(myMat);

	printf("\nREAL R:\n");
	dmpMat4(realR);

	printf("\nSSE R:\n");
	dmpMat4(sseR);*/

	Matrix4 mat = Matrix4::fromXYZRotation(Degree(45), Degree(30), Degree(15));
	mat *= Matrix4::fromTranslation(Vector3D(1.f, 2.f, 3.f));
	mat *= Matrix4::fromScale(2.f, 3.f, 2.f);

	Vector3D vec(0.f, 0.f, 0.f);

	Vector3D x1 = mat * vec;
	system("pause >nul");
	return 0;
}
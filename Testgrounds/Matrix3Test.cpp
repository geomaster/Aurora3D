#include <AuroraMatrix3.h>
#include <cstdlib>
#include <cassert>

using namespace Aurora;

void dumpMatrix(const Matrix3& mat)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
			printf("%f ", mat[i][j]);
		printf("\n");
	}
}
int test1()
{
	Matrix3 idt1 = Matrix3::Identity;
	Matrix3 idt2 = Matrix3::Identity;

	Matrix3 res1 = idt1 * idt2;
	dumpMatrix(res1);
	printf("det: %f\n", res1.getDeterminant());
	printf("\n");

	res1.invert();
	dumpMatrix(res1);
	printf("det: %f\n", res1.getDeterminant());

	printf("\n");

	Matrix3 a1 = Matrix3( 1.f, 0.f, 3.f, 
						  0.f, 5.f, 3.f,
						  0.f, 2.f, 3.f);
	a1 *= 5.f;
	dumpMatrix(a1);
	printf("\n");

	a1.transpose();
	dumpMatrix(a1);
	printf("\n");

	Matrix3 a1inv;
	a1.getInverse(a1inv);
	Matrix3 res2 = a1 * a1inv;
	dumpMatrix(res2);
	printf("det: %f\n", res2.getDeterminant());
	printf("\n");

	res2.setEntry(2, 2, 10.f);
	dumpMatrix(res2);
	printf("det: %f\n", res2.getDeterminant());
	printf("\n");

	res2.invert();
	dumpMatrix(res2);
	printf("det: %f\n", res2.getDeterminant());
	printf("\n");

	res2.invert();
	dumpMatrix(res2);
	printf("det: %f\n", res2.getDeterminant());
	printf("\n");

	// A rotation matrix
	Matrix3 yaw(Matrix3::fromYRotation(Degree(45)));
	Vector3D resVec(0.f, 0.f, 1.f);
	resVec = yaw * resVec;
	Vector3D yawEigen(0.f, 1.f, 0.f);
	yawEigen = yaw * yawEigen;
	assert(yaw.isOrthogonal());

	printf("z-axis rotation around y for 45 deg: (%f,%f,%f)\n", resVec.getX(), resVec.getY(), resVec.getZ());
	printf("norm: %f\n", resVec.getLength());

	printf("y-axis rotation around y for 45 deg (eigen): (%f,%f,%f)\n", yawEigen.getX(), yawEigen.getY(), yawEigen.getZ());
	printf("norm: %f\n", yawEigen.getLength());

	Matrix3 rot(Matrix3::fromXYZRotation(Degree(90), Degree(270), Degree(90)));
	Vector3D yUnit = Vector3D::UnitY;
	Vector3D resultant = yUnit * rot;
	printf("(%f %f %f), |a| = %f\n", resultant.getX(), resultant.getY(), resultant.getZ(), resultant.getLength());

	Matrix3 rot2(Matrix3::fromAngleAxis(Vector3D(1,1,1).getNormalized(), Degree(90)));
	Vector3D eigen1(1.f,1.f, 1.f);
	printf("|a|=%f\n", eigen1.getLength());

	eigen1 = rot2 * eigen1;
	assert(rot2.isOrthogonal());
	printf("(%f %f %f), |a| = %f\n", eigen1.getX(), eigen1.getY(), eigen1.getZ(), eigen1.getLength());

	return 0;
}

int main()
{
	test1();
	system("pause >nul");
	return 0;
}
#include <cstdlib>
#include <AuroraQuaternion.h>
#include <AuroraVector3D.h>
#include <AuroraSceneNode.h>

using namespace Aurora;

int main()
{
	Quaternion q(Vector3D(0.f, 1.f, 0.f), Degree(90));
	Matrix4 rot(Matrix4::fromAngleAxis(Vector3D(0.f, 1.f, 0.f), Degree(90))), rot2(q.toRotationMatrix3()), rot3(q.toRotationMatrix3Unit());

	Vector3D v(0.f, 0.f, 1.f);
	Vector3D mat(rot * v), quat(q * v), mat2(rot2 * v), mat3(rot3 * v);

	printf("by matrix: %f %f %f\n", mat.getX(), mat.getY(), mat.getZ());
	printf("by quaternion: %f %f %f\n", quat.getX(), quat.getY(), quat.getZ());
	printf("by matrix from q: %f %f %f\n", mat2.getX(), mat2.getY(), mat2.getZ());
	printf("by matrix from norm q: %f %f %f\n", mat3.getX(), mat3.getY(), mat3.getZ());

	system("pause >nul");

	//SceneNode d;
	return 0;
}
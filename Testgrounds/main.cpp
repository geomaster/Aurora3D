#include <iostream>
#include <AuroraVector3D.h>
#include <AuroraMatrix4.h>
#include <cstdio>

using namespace std;
using namespace Aurora;

int main()
{
    Vector3D x(2.f, 3.f, 4.f);
    x.normalize();

    Matrix4 a(Matrix4::Identity);
    Matrix4 b(Matrix4::fromYRotation(Degree(90)));

    a*=b;
    printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n", a[0][0], a[0][1],
           a[0][2], a[0][3], a[1][0], a[1][1], a[1][2], a[1][3],
           a[2][0], a[2][1], a[2][2], a[2][3],
           a[3][0], a[3][1], a[3][2], a[3][3]);

    Vector3D v(0.f, 0.f, 1.f);
    v = v * a;

    printf("(%f %f %f)", v.getX(), v.getY(), v.getZ());
    char pk;
    cin >> pk;
    return 0;
}

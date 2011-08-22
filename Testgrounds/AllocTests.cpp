#include <AuroraMatrix4.h>
#include <cstdlib>
using namespace Aurora;

class Align16 : virtual public SSEAlloc
{
public:
	virtual void check() { printf(reinterpret_cast<size_t>(this) % 16 ? "nope\n" : "yeah\n"); }
};

class Align32 : virtual public Align16, virtual public AllocatedObject<DefaultAllocators, 32>
{
public:
	virtual void check() { printf(reinterpret_cast<size_t>(this) % 32 ? "nope\n" : "yeah\n"); }
};

class Align64 : virtual public Align16, virtual public AllocatedObject<DefaultAllocators, 64>
{
public:
	virtual void check() { printf(reinterpret_cast<size_t>(this) % 64 ? "nope\n" : "yeah\n"); }
};

class DontCare : virtual public Align16, virtual public Align32
{
public:
	virtual void check() { printf(reinterpret_cast<size_t>(this) % 32 ? "nope\n" : "yeah\n"); }
};

int main()
{
	Matrix4 *d = new Matrix4(Matrix4::fromTranslation(1.f, 2.f, 3.f));

	printf("matrix4 is 16-aligned? %s\n", (reinterpret_cast<int>(d)%16?"nope":"yeah"));

	Matrix4 *d2 = new Matrix4(Matrix4::fromXYZRotation(Degree(90), Degree(45), Degree(15)));

	d->operator+=(*d2);

	delete d;

	Align16 *p16 = new Align16();
	p16->check();

	Align32 *p32 = new Align32();
	p32->check();

	Align64 *p64 = new Align64();
	p64->check();

	DontCare* pDC = new DontCare();
	pDC->check();

	{ char c; scanf("%c",&c); }

	return 0;
}

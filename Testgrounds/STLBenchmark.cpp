#include <AuroraPrereqs.h>
#include <STL/Vector.h>
#include <cstdlib>
#include "WinTimer.h"

using namespace Aurora;

int subj1(STL::Vector<int>::iterator* it)
{
	int numEvens = 0;
	while (it->hasMoreElements())
	{
		int t = it->get();
		if (!t&1) ++numEvens;

		it->increment();
	}

	return numEvens;
}

int subj2(STL::Vector<int>::type::iterator it, const STL::Vector<int>::type& cont)
{
	int numEvens = 0;
	for (it; it != cont.end(); ++it)
	{
		int t = *it;
		if (!t&1) ++numEvens;

		++it;
	}

	return numEvens;
}
int main()
{
	const int num = 100;
	const int max = 1234567;
	STL::Vector<int>::type one;
	for (int i = 0; i < num; ++i)
	{
		one.push_back(rand() % max);
	}

	const int numExecs = 40000, modulo = 127;

	int accumul = 0;

	Timer t;
	t.startCounting();
	for (int i = 0; i < numExecs; ++i)
	{
		STL::Vector<int>::iterator it(one.begin(), one.begin(), one.end());
		accumul += subj1(&it) % modulo;
	}
	double tOne = t.stopCounting();

	printf("%d time = %f\n", accumul, tOne);

	t.startCounting();
	for (int i = 0; i < numExecs; ++i)
	{
		STL::Vector<int>::type::iterator it(one.begin());
		accumul += subj2(it, one) % modulo;
	}
	double tTwo = t.stopCounting();

	printf("%d time = %f\n", accumul, tTwo);

	system("pause >nul");
}
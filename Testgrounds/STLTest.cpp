#include <AuroraPrereqs.h>
#include <AuroraIterator.h>
#include <STL/Vector.h>
#include <STL/List.h>
#include <cstdio>
#include <iostream>
#include <AuroraMemoryTracker.h>

using namespace Aurora;

void iterate(ContainerForwardIterator<String> *it)
{
	printf("I'm iterating and don't know the type...\n");
	while(it->hasMoreElements())
	{
		printf("\t%s\n", it->get().c_str());
		it->increment();
	}
}

void test()
{
	STL::Vector<String>::type vec;
	STL::List<String>::type list;
	vec.push_back("one");
	vec.push_back("two");
	vec.push_back("three");
	vec.push_back("this is gettin' troblesome");
	vec.push_back("hey ya");
	vec.push_back("i'm the last one");

	list.push_back("i'm the list guy");
	list.push_back("me too!");

	STL::Vector<String>::iterator it(vec.begin(), vec.begin(), vec.end());
	iterate(&it);

	STL::List<String>::iterator it2(list.begin(), list.begin(), list.end());
	iterate(&it2);

	MemoryTracker::getSingletonPtr()->report(std::cout);
}

int main()
{
	test();
	MemoryTracker::getSingletonPtr()->report(std::cout);

	{ char c; scanf("%c", &c); }
	return 0;
}

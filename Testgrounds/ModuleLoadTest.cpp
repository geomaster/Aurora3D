#include <Aurora.h>
#include "../../Testmodule/TestModule.h"
#include <iostream>
using namespace Aurora;
using namespace std;

int main()
{
	cout << "hey.\n";
	Engine* e = new Engine();
#	if AURORA_DYNLIB_API == 1
	Module* t = e->addModule("TestModule.dll");
#	else
	Module* t = e->addModule("../Testmodule/bin/Debug/libtestmodule.so");
#	endif

	assert(t);
	TestModule *m = reinterpret_cast<TestModule*>(t);
	cout << "afaik...";

	e->installModules();
	assert(m->isInstalled());
	e->activateModule("TestModule");
	assert(m->activateFlagSet() && m->isActive());

	cin.get();
	return 0;
}

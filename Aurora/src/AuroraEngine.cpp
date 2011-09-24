/*
	This file is part of Aurora Game Engine.

	Aurora Game Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Aurora Game Engine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with Aurora Game Engine.  If not, see <http://www.gnu.org/licenses/>.

	Copyright (C) David Davidovic (Geomaster) 2011.

*/
#include "AuroraEngine.h"
#include "AuroraException.h"
#include "AuroraFrameListener.h"
#include "AuroraSceneManager.h"

using namespace Aurora;

Engine::Engine()
{

}

Engine::Engine(const String& PluginConfigurationFile)
{
	loadModulesFromFile(PluginConfigurationFile);
	installModules();
}

void Engine::loadModulesFromFile(const String& Filename)
{
	// TODO: Implement this (eww)
}

void Engine::initialize()
{
	// Notify all modules
	for (ModuleCategoryMapIterator mapIt = mModules.begin(); mapIt != mModules.end(); ++mapIt)
		for (ModuleListIterator modIt = mapIt->second.begin(); modIt != mapIt->second.end(); ++modIt)
			if (modIt->Installed)
				modIt->Mod->onInitialize();

	// TODO: Resource managers and stuff

	mInitialized = true;
}


Engine::ModuleSpec Engine::createModule(const String& Filename)
{
	try
	{
		DynamicLibrary* lib = AURORA_NEW DynamicLibrary(Filename);
		Module* (*moduleCreateFunc)();
		moduleCreateFunc = reinterpret_cast<Module*(*)()>(lib->getSymbol("createInstance"));

		if (moduleCreateFunc)
		{
			Module* m = moduleCreateFunc();
			if (!m)
				throw BadModuleException();
			else
			{
				ModuleSpec spec;
				spec.Category = m->getCategory();
				spec.Name = m->getName();
				spec.Lib = lib;
				spec.Mod = m;
				spec.SourceFilename = Filename;

				return spec;
			}
		}
		else throw BadModuleException();
	}
	catch (LibraryCallException&)
	{
		// Something's bad in the module, don't rethrow an internal error to the caller
		throw BadModuleException();
	}
}

void Engine::destroyModule(const ModuleSpec& Mod)
{
	try
	{
		if (Mod.Mod && Mod.Lib)
		{
			void (*moduleDestroyFunc)(Module*) = reinterpret_cast<void(*)(Module*)>(Mod.Lib->getSymbol("destroyInstance"));
			if (moduleDestroyFunc)
				moduleDestroyFunc(Mod.Mod), delete Mod.Lib;
			else throw BadModuleException();
		}
	}
	catch (LibraryCallException&)
	{
		// Something failed, rethrow a more descriptive exception
		throw BadModuleException();
	}
}

void Engine::shutdown()
{
	// Notify all modules
	for (ModuleCategoryMapIterator mapIt = mModules.begin(); mapIt != mModules.end(); ++mapIt)
		for (ModuleListIterator modIt = mapIt->second.begin(); modIt != mapIt->second.end(); ++modIt)
		{
			if (modIt->Installed)
				modIt->Mod->uninstall(this);

			modIt->Mod->onShutdown();
		}

	// TODO: Resource managers and stuff

	mInitialized = false;
}

void Engine::addModule(String Filename)
{
	ModuleSpec spec = createModule(Filename);

	if (!hasModule(spec.Name))
		mModules[spec.Category].push_back(spec);
	else
		// Already have the module with that name? Sorry pal
		destroyModule(spec);
}

bool Engine::hasModule(String ModuleName)
{
	// O(n). Fuck. Well, it's not likely that this function will be called often...
	for (ModuleCategoryMapIterator catIt = mModules.begin(); catIt != mModules.end(); ++catIt)
		for (ModuleListIterator modIt = catIt->second.begin(); modIt != catIt->second.end(); ++modIt)
			if (modIt->Name == ModuleName)
				return true;

	return false;
}

void Engine::removeModule(String Filename)
{
	for (ModuleCategoryMapIterator catIt = mModules.begin(); catIt != mModules.end(); ++catIt)
	{
		for (ModuleListIterator modIt = catIt->second.begin(); modIt != catIt->second.end(); ++modIt)
			if (modIt->SourceFilename == Filename)
			{
				destroyModule(*modIt);
				catIt->second.erase(modIt);
				break;
			}
	}
}

void Engine::installModules()
{
	for (ModuleCategoryMapIterator catIt = mModules.begin(); catIt != mModules.end(); ++catIt)
	{
		for (ModuleListIterator it = catIt->second.begin(); it != catIt->second.end(); ++it)
			if (!it->Installed)
			{
				it->Mod->install(this);
				it->Installed = true;
			}
	}
}

void Engine::uninstallModules()
{
	for (ModuleCategoryMapIterator catIt = mModules.begin(); catIt != mModules.end(); ++catIt)
	{
		for (ModuleListIterator it = catIt->second.begin(); it != catIt->second.end(); ++it)
			if (it->Installed)
			{
				it->Mod->uninstall(this);
				it->Installed = false;
			}
	}
}

void Engine::activateModule(String Name)
{
	for (ModuleCategoryMapIterator catIt = mModules.begin(); catIt != mModules.end(); ++catIt)
	{
		for (ModuleListIterator it = catIt->second.begin(); it != catIt->second.end(); ++it)
			if (it->Name == Name)
				it->Mod->activate();
	}
}

void Engine::deactivateModule(String Name)
{
	for (ModuleCategoryMapIterator catIt = mModules.begin(); catIt != mModules.end(); ++catIt)
	{
		for (ModuleListIterator it = catIt->second.begin(); it != catIt->second.end(); ++it)
			if (it->Name == Name)
				it->Mod->deactivate();
	}
}

void Engine::deactivateAllModules()
{
	for (ModuleCategoryMapIterator catIt = mModules.begin(); catIt != mModules.end(); ++catIt)
	{
		for (ModuleListIterator it = catIt->second.begin(); it != catIt->second.end(); ++it)
			it->Mod->deactivate();
	}
}

void Engine::switchModuleInCategory(String Category, String Name)
{
	ModuleCategoryMapIterator it = mModules.find(Category);
	if (it != mModules.end())
		for (ModuleListIterator modIt = it->second.begin(); modIt != it->second.end(); ++modIt)
			if (modIt->Name == Name)
				modIt->Mod->activate();
			else
				modIt->Mod->deactivate();
}

void Engine::deactivateModulesInCategory(String CategoryName)
{
	ModuleCategoryMapIterator it = mModules.find(CategoryName);
	if (it != mModules.end())
		for (ModuleListIterator modIt = it->second.begin(); modIt != it->second.end(); ++modIt)
			modIt->Mod->deactivate();	
}

void Engine::registerSceneFactory(Factory<Scene>* SceneFactory)
{
	String type = SceneFactory->getType();
	SceneFactoryMap::iterator it = mSceneFactories.find(type);
	if (it != mSceneFactories.end())
		throw DuplicateNameException();
	else
		mSceneFactories.insert(SceneFactoryMap::value_type(type, SceneFactory));
}

void Engine::unregisterSceneFactory(String Type)
{
	SceneFactoryMap::iterator it = mSceneFactories.find(Type);
	if (it == mSceneFactories.end())
		throw NonExistentNameException();
	else
		mSceneFactories.erase(it);
}

void Engine::registerSceneManagerFactory(Factory<SceneManager>* SceneManagerFactory)
{
	String type = SceneManagerFactory->getType();
	SceneManagerFactoryMap::iterator it = mSceneManagerFactories.find(type);
	if (it != mSceneManagerFactories.end())
		throw DuplicateNameException();
	else
		mSceneManagerFactories.insert(SceneManagerFactoryMap::value_type(type, SceneManagerFactory));
}

void Engine::unregisterSceneManagerFactory(String Type)
{
	SceneManagerFactoryMapIterator it = mSceneManagerFactories.find(Type);
	if (it == mSceneManagerFactories.end())
		throw NonExistentNameException();
	else
		mSceneManagerFactories.erase(it);
}

Scene* Engine::createScene(String Type)
{
	SceneFactoryMapIterator it = mSceneFactories.find(Type);
	if (it == mSceneFactories.end())
		throw NonExistentNameException();
	else
	{
		Scene *s = it->second->create();
		mScenes[s] = it->second;
		return s;
	}
}

void Engine::destroyScene(Scene* OldScene)
{
	CreatedScenesMapIterator it = mScenes.find(OldScene);
	if (it == mScenes.end())
		throw NonExistentNameException();
	else
	{
		it->second->destroy(it->first);
		mScenes.erase(it);
	}
}

SceneManager* Engine::createSceneManager(String Type)
{
	SceneManagerFactoryMapIterator it = mSceneManagerFactories.find(Type);
	if (it == mSceneManagerFactories.end())
		throw NonExistentNameException();
	else
	{
		SceneManager *s = it->second->create();
		mSceneManagers[s] = it->second;
		return s;
	}
}

void Engine::destroySceneManager(SceneManager* OldSceneManager)
{
	CreatedSceneManagersMapIterator it = mSceneManagers.find(OldSceneManager);
	if (it == mSceneManagers.end())
		throw NonExistentNameException();
	else
	{
		it->second->destroy(it->first);
		mSceneManagers.erase(it);
	}
}

void Engine::addFrameListener(FrameListener* Listener, ListenerPriority Priority)
{
	FrameListenerSpec spec;
	spec.Listener = Listener;
	spec.Priority = Priority;
	
	if (mListeners.find(spec) == mListeners.end())
		throw DuplicateObjectException();
	else
		mListeners.insert(spec);
}

void Engine::removeFrameListener(FrameListener* Listener)
{
	// Perform a linear search because we don't know the priority to construct
	// an equivalent FrameListenerSpec;
	bool found = false;
	for (ListenerListIterator it = mListeners.begin(); it != mListeners.end(); ++it)
		if (it->Listener == Listener)
		{
			found = true;
			mListeners.erase(it);
			break;
		}

	// Throwing away a cycle or two and 4 bytes in favor of good practice - close enough.
	if (!found) throw NonExistentObjectException();
}

// </Boilerplate>
// <JuicyCode>, muthafucka!

bool Engine::doOneFrame()
{
	FrameEvent evt; 
	// [first-order priority] TODO: Implement timing
	evt.TimeSinceLastEvent = Real(0.0);
	evt.TimeSinceLastFrame = Real(0.0);

	// Notify the listeners that a frame is starting (i.e. we're about to update the scene graph)
	for (ListenerListIterator it = mListeners.begin(); it != mListeners.end(); ++it)
		if (!it->Listener->preSceneUpdate(evt))
			return false;

	// Update all of the scene managers
	for (CreatedSceneManagersMapIterator it = mSceneManagers.begin(); it != mSceneManagers.end(); ++it)
		it->first->update();

	// Tell everyone that the frame is ending (i.e. we have just updated the scene graph
	for (ListenerListIterator it = mListeners.begin(); it != mListeners.end(); ++it)
		if (!it->Listener->postSceneUpdate(evt))
			return false;

	return true;
}

void Engine::startMainLoop()
{
	// This looks pretty disappointing for the main function of the whole engine core. Duh.
	while (doOneFrame());
}

Engine::~Engine()
{
	shutdown();
	for (ModuleCategoryMapIterator mapIt = mModules.begin(); mapIt != mModules.end(); ++mapIt)
		for (ModuleListIterator modIt = mapIt->second.begin(); modIt != mapIt->second.end(); ++modIt)
			destroyModule(*modIt);

	for (CreatedSceneManagersMapIterator it = mSceneManagers.begin(); it != mSceneManagers.end(); ++it)
		it->second->destroy(it->first);

	for (CreatedScenesMapIterator it = mScenes.begin(); it != mScenes.end(); ++it)
		it->second->destroy(it->first);
}
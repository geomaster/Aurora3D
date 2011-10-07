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
#ifndef __AURORA_ENGINE_H__
#define __AURORA_ENGINE_H__
#include "AuroraPrereqs.h"
#include "AuroraAllocatedObject.h"
//#include "AuroraScene.h"
//#include "AuroraSceneManager.h"
#include "AuroraFactory.h"
#include "AuroraModule.h"
#include "AuroraDynamicLibrary.h"
#include "STL/HashMap.h"
#include "STL/Set.h"
#include "STL/Vector.h"

namespace Aurora
{
	// The root class of the system, doing things like message pump, frame stepping etc.
	// Instantiates modules and similar business. Scene Managers should be created from
	// here, not using the operator new or any other memory allocation.
	class AURORA_LIBRARY Engine : virtual public Alloc
	{
	public:
		typedef int ListenerPriority;

	protected:
		struct FrameListenerSpec
		{
			FrameListener *Listener;
			ListenerPriority Priority;

			inline bool operator<(const FrameListenerSpec& other) const
			{
				// Make sure there is an ordering first by priorities, then by addresses
				if (Priority == other.Priority)
					return Listener < other.Listener;
				else return Priority < other.Priority;
			}

			inline bool operator>(const FrameListenerSpec& other) const
			{
				// Make sure there is an ordering first by priorities, then by addresses
				if (Priority == other.Priority)
					return Listener > other.Listener;
				else return Priority > other.Priority;
			}

			inline bool operator==(const FrameListenerSpec& other) const
			{
				return (Priority == other.Priority && Listener == other.Listener);
			}
		};

		struct ModuleSpec
		{
			String Category, Name, SourceFilename;
			bool Installed;
			Module *Mod;
			DynamicLibrary *Lib;
		};


		typedef STL::HashMap<Scene*, Factory<Scene>* >::type CreatedScenesMap;
		typedef CreatedScenesMap::iterator CreatedScenesMapIterator;
		typedef CreatedScenesMap::const_iterator CreatedScenesMapConstIterator;

		typedef STL::HashMap<SceneManager*, Factory<SceneManager>* >::type CreatedSceneManagersMap;
		typedef CreatedSceneManagersMap::iterator CreatedSceneManagersMapIterator;
		typedef CreatedSceneManagersMap::const_iterator CreatedSceneManagersMapConstIterator;

		typedef STL::HashMap<String, Factory<Scene>* >::type SceneFactoryMap;
		typedef SceneFactoryMap::iterator SceneFactoryMapIterator;
		typedef SceneFactoryMap::const_iterator SceneFactoryMapConstIterator;

		typedef STL::HashMap<String, Factory<SceneManager>* >::type SceneManagerFactoryMap;
		typedef SceneManagerFactoryMap::iterator SceneManagerFactoryMapIterator;
		typedef SceneManagerFactoryMap::const_iterator SceneManagerFactoryMapConstIterator;

		// Due to the usage of std::greater and FrameListenerSpec's overloaded operator >, these
		// listeners will be following a strict higher-priority-first ordering.
		typedef STL::Set<FrameListenerSpec, std::greater<FrameListenerSpec> >::type ListenerList;
		typedef ListenerList::iterator ListenerListIterator;
		typedef ListenerList::const_iterator ListenerListConstIterator;

		typedef STL::Vector<ModuleSpec>::type ModuleList;
		typedef ModuleList::iterator ModuleListIterator;
		typedef ModuleList::const_iterator ModuleListConstIterator;

		typedef STL::HashMap<String, ModuleList>::type ModuleCategoryMap;
		typedef ModuleCategoryMap::iterator ModuleCategoryMapIterator;
		typedef ModuleCategoryMap::const_iterator ModuleCategoryMapConstIterator;

		SceneFactoryMap mSceneFactories;
		CreatedScenesMap mScenes;
		CreatedSceneManagersMap mSceneManagers;
		SceneManagerFactoryMap mSceneManagerFactories;
		ListenerList mListeners;
		ModuleCategoryMap mModules;

		bool mInitialized;

		void loadModulesFromFile(const String& Filename);
		// TODO: Resource managers

		//void create
			//TODO: createModule() and destroyModule(); use them in addModule and removeModule resp., decentralize
		ModuleSpec createModule(const String& Filename);
		void destroyModule(const ModuleSpec& Mod);

		// control
	public:
		Engine();
		Engine(const String& PluginConfigurationFile);

		void initialize();
		void shutdown();

		Module* addModule(String Filename);
		void removeModule(String ModuleName);
		bool hasModule(String ModuleName);

		void installModules();
		void uninstallModules();

		void activateModule(String Name);
		void deactivateModule(String Name);
		void deactivateAllModules();
		// Activates a module and deactivates any other active in the same category
		void switchModuleInCategory(String Category, String Name);
		// Deactivates all modules in a specific category
		void deactivateModulesInCategory(String CategoryName);

		void registerSceneFactory(Factory<Scene>* SceneFactory);
		inline void unregisterSceneFactory(Factory<Scene>* OldFactory)
		{
			unregisterSceneFactory(OldFactory->getType());
		}
		void unregisterSceneFactory(String Type);
		void registerSceneManagerFactory(Factory<SceneManager>* SceneManagerFactory);
		inline void unregisterSceneManagerFactory(Factory<SceneManager>* OldFactory)
		{
			unregisterSceneManagerFactory(OldFactory->getType());
		}
		void unregisterSceneManagerFactory(String Type);

		Scene* createScene(String Type);
		SceneManager* createSceneManager(String Type);
		void destroyScene(Scene* OldScene);
		void destroySceneManager(SceneManager* OldSceneManager);

		void addFrameListener(FrameListener* Listener, ListenerPriority Priority = 1);
		void removeFrameListener(FrameListener* Listener);

		bool doOneFrame();
		void startMainLoop();

		~Engine();
	};
}

#endif

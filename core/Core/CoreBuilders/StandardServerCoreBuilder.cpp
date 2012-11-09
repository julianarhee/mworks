/**
 * StandardServerCoreBuilder.cpp
 *
 * History:
 * paul on 1/20/06 - Created.
 *
 * Copyright 2006 MIT. All rights reserved.
 */

#include "StandardServerCoreBuilder.h"

#include "ComponentRegistry.h"

#import "Utilities.h"
#import "Experiment.h"
#import "Scheduler.h"
#import "StateSystem.h"
#import "Event.h"
#import "PluginServices.h"
#import "TrialBuildingBlocks.h"
#import "VariableRegistry.h"
#import "StandardVariables.h"
#import "EventBuffer.h"
#import "OpenALContextManager.h"
#import "OpenGLContextManager.h"
#include "PlatformDependentServices.h"
#include "MWPythonEnvironment.h"


BEGIN_NAMESPACE_MW


StandardServerCoreBuilder::StandardServerCoreBuilder() { }
StandardServerCoreBuilder::~StandardServerCoreBuilder() { }

/****************************************************************
 *             AbstractCoreBuilder  Methods
 ***************************************************************/
bool StandardServerCoreBuilder::buildProcessWillStart() { return true; }

bool StandardServerCoreBuilder::initializeEventBuffers() {
    initEventBuffers();
    return true;
}

bool StandardServerCoreBuilder::initializeRegistries() {
	initializeServiceRegistries();
	
	return true;
}

bool StandardServerCoreBuilder::initializeGlobalParameters() {
    initializeStandardVariables(global_variable_registry);
	try {
		loadSetupVariables();
	} catch (std::exception& e){
		mwarning(M_PARSER_MESSAGE_DOMAIN, "Unable to load setup variables.  Error was: %s", e.what());
	}
	GlobalMessageOrigin = M_SERVER_MESSAGE_ORIGIN;
   
    return true;
}

bool StandardServerCoreBuilder::loadPlugins() {
	
	shared_ptr<ComponentRegistry> component_registry = 
									ComponentRegistry::getSharedRegistry();
	
	readPlugins(pluginPath());
    return true;
}

bool StandardServerCoreBuilder::chooseRealtimeComponents() {
    
	shared_ptr<ComponentRegistry> component_registry = 
									ComponentRegistry::getSharedRegistry();

	shared_ptr<mw::Component> clock = component_registry->createNewObject("MachClock", map<string, string>());
	Clock::registerInstance(clock);	
	
	shared_ptr<mw::Component> scheduler = component_registry->createNewObject("ZenScheduler", map<string, string>());
	Scheduler::registerInstance(scheduler);
	
	shared_ptr<mw::Component> state_system = component_registry->createNewObject("ZenStateSystem", map<string, string>());
	StateSystem::registerInstance(state_system);

    return true;
}

bool StandardServerCoreBuilder::startInterpreters() {
    shared_ptr<MWPythonEnvironment> python_env(new MWPythonEnvironment());
    
    MWPythonEnvironment::registerInstance(python_env);
    python_env->initialize();
    
    return true;
}

bool StandardServerCoreBuilder::startRealtimeServices() {
	shared_ptr <Clock> clock = Clock::instance();
	clock->startClock();
    return true;
}

bool StandardServerCoreBuilder::customInitialization() { 
    // ignore sigpipes
    signal(SIGPIPE, SIG_IGN);
	
    return true; 
}

bool StandardServerCoreBuilder::buildProcessWillEnd() { return true; }


END_NAMESPACE_MW

//
//  MWPythonEnvironment.cpp
//  MWorksCore
//
//  Created by David Cox on 10/30/12.
//
//

#include "MWPythonEnvironment.h"
#include "MWorksCore/PlatformDependentServices.h"
#include <python/python.h>
#include <cstdio>

using namespace mw;

SINGLETON_INSTANCE_STATIC_DECLARATION(MWPythonEnvironment);

    
MWPythonEnvironment::MWPythonEnvironment(){ }
    
void MWPythonEnvironment::initialize(){
    Py_Initialize();
    thread_state = Py_NewInterpreter();
    
    // build the path to the interpreter prefix script
    // that makes the MWorks experiment environment (variables etc.
    // available to python
    std::string prefix_script = prependScriptingPath("python_environment_prefix.py").string().c_str();
    runFile(prefix_script);
    
}
    
void MWPythonEnvironment::finalize(){
    Py_EndInterpreter(thread_state);
}

void MWPythonEnvironment::runFile(std::string filepath){
    FILE *f = std::fopen(filepath.c_str(), "r");
    PyRun_SimpleFile(f, filepath.c_str());
    fclose(f);
}

void MWPythonEnvironment::runString(std::string code){
    
    PyRun_SimpleString(code.c_str());
}

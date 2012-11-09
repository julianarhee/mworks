//
//  PythonAction.cpp
//  MWorksCore
//
//  Created by David Cox on 10/31/12.
//
//

#include "PythonAction.h"

using namespace mw;

PythonAction::PythonAction(std::string _code){
    code = _code;
    interpreter = MWPythonEnvironment::instance();
}

bool PythonAction::execute(){

    mprintf("running: {\n %s \n}", code.c_str());
    interpreter->runString(code);
    return true;
}

shared_ptr<mw::Component> PythonActionFactory::createObject(std::map<std::string, std::string> parameters,
                                                            ComponentRegistry *reg){
    REQUIRE_ATTRIBUTES(parameters, "xml_cdata");
    
    return shared_ptr<mw::Component>(new PythonAction(parameters["xml_cdata"]));
}

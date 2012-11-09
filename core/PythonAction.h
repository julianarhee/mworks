//
//  PythonAction.h
//  MWorksCore
//
//  Created by David Cox on 10/31/12.
//
//

#ifndef __MWorksCore__PythonAction__
#define __MWorksCore__PythonAction__

#include "TrialBuildingBlocks.h"
#include "MWPythonEnvironment.h"

BEGIN_NAMESPACE_MW


class PythonAction : public Action {
protected:
    std::string code;
    shared_ptr<MWPythonEnvironment> interpreter;
    
public:
	PythonAction(std::string code);
	virtual ~PythonAction(){ }
	virtual bool execute();
};

class PythonActionFactory : public ComponentFactory{
	virtual shared_ptr<mw::Component> createObject(std::map<std::string, std::string> parameters,
                                                   ComponentRegistry *reg);
	
};


END_NAMESPACE_MW

#endif /* defined(__MWorksCore__PythonAction__) */

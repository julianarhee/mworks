//
//  MWPythonEnvironment.h
//  MWorksCore
//
//  Created by David Cox on 10/30/12.
//
//

#ifndef __MWorksCore__MWPythonEnvironment__
#define __MWorksCore__MWPythonEnvironment__


#include "MWPythonEnvironment.h"
#include "RegisteredSingleton.h"
#include <python2.7/python.h>
#include <string>

namespace mw {

class MWPythonEnvironment{
    
    protected:
        
        PyThreadState *thread_state;
        
        
    public:
        
        MWPythonEnvironment();
        
        void initialize();
        void finalize();
        void runFile(std::string filepath);        
        void runString(std::string code);
    
        REGISTERED_SINGLETON_CODE_INJECTION(MWPythonEnvironment);
    
    };
    
}

#endif /* defined(__MWorksCore__MWPythonEnvironment__) */

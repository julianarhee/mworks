//
//  PythonBindings.h
//  MWorksCore
//
//  Created by David Cox on 10/31/12.
//
//

#ifndef __MWorksCore__PythonBindings__
#define __MWorksCore__PythonBindings__

#include <MWorksCore/VariableRegistry.h>
#include <MWorksCore/EventBuffer.h>
#include <MWorksCore/GenericVariable.h>
#include <boost/python.hpp>
using namespace boost::python;


BEGIN_NAMESPACE_MW


// A wrapper/shim to smooth over polymorphism in the variable interface
class WrappedVariable {
    
protected:
    
    shared_ptr<Variable> variable;
    
public:
    
    WrappedVariable(){
        // do nothing
    }
    
    WrappedVariable(shared_ptr<Variable> var){
        variable = var;
    }
    
    // TODO: take / give python types
    Datum getValue(){
        return variable->getValue();
    }
    
    void setValue(Datum new_val){
        variable->setValue(new_val);
    }

    void setValueWithTime(Datum new_val, MWTime time){
        variable->setValue(new_val, time);
    }
};


// standalone access functions for the variable registry
// (since wrapping the VariableRegistry turns out to be messy)
shared_ptr<WrappedVariable> getVariableByName(const std::string& tagname) {
    shared_ptr<WrappedVariable> var(new WrappedVariable(global_variable_registry->getVariable(tagname)));
    return var;
}

shared_ptr<WrappedVariable> getVariableByCode(int codec_code) {
    shared_ptr<WrappedVariable> var(new WrappedVariable(global_variable_registry->getVariable(codec_code)));
    return var;
}



BOOST_PYTHON_MODULE(variables)
{
    
    PyEval_InitThreads();

    
    
    class_<Datum>("Datum")
    .add_property("float", &Datum::getFloat)
    .add_property("integer", &Datum::getInteger);
    ;
    
    class_<WrappedVariable>("Variable")
    .add_property("value", &WrappedVariable::getValue, &WrappedVariable::setValue)
    ;
    
    def("get_variable_by_name", &getVariableByName);
    def("get_variable_by_code", &getVariableByCode);
    
    register_ptr_to_python< shared_ptr<WrappedVariable> >();

        
}
    
    
END_NAMESPACE_MW

#endif /* defined(__MWorksCore__PythonBindings__) */

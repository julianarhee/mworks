/*
 *  ExpressionVariable.h
 *  MWorksCore
 *
 *  Created by David Cox on 2/20/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 *
 *	The expression variable object hierarchically represents arbitrary
 *  expressions made up of other variable objects (which might be "regular"
 *  variables, constants, or other expressions).  The base Variable
 *  object overrides arithmetic operators to generate ExpressionVariable
 *  objects, and one can create arbitrarily complex trees of variables
 *  and operation by simply performing arithmetic on the variables themselves
 *  (e.g. "a + 4 / b" would evaluate to two layer hierarchy of variable
 *  objects contained within an expression variable, and this composite
 *  variable would evaluate to the current value of the expression when 
 *  cast to a built-in type or evaluated with the "get" method.  Since this
 *  operator overloading can be exposed in scripting targets as well, very 
 *  expression creation can be done within the XML parser in an almost 
 *  entirely seemless way, with no explicit "expression parsing" code required
 */
 
#ifndef	EXPRESSION_VARIABLE_H_
#define	EXPRESSION_VARIABLE_H_
 
#include "GenericVariable.h"
#include "VariableRegistry.h"

#include <iostream>
#include <boost/algorithm/string/replace.hpp>
#include <boost/regex.hpp>
namespace mw {
class	ExpressionVariable : public Variable {

	protected:
	
		Variable *v1;
		Variable *v2;
	
		Operator op;
	
	public:
	
		ExpressionVariable(Variable *_v1 = NULL, Variable *_v2 = NULL, Operator _op = M_PLUS);
		
		Operator getOperator() { return op; };

		virtual Datum getValue(){
			return getExpressionValue();
		}
		
		virtual void setValue(Datum val){ return; }
		virtual void setValue(Datum val, MWTime time){ return; }
		virtual void setSilentValue(Datum _value){ return; }
		
		// TODO: remove
	 Datum getExpressionValue();
		
		Variable *getFirstOperand(){ return v1; }
		Variable *getSecondOperand(){ return v2; }
		
		
		/**
		 *  A polymorphic copy constructor (inherited from Clonable)
		 */
		virtual Variable *clone(){
			ExpressionVariable *returned = new ExpressionVariable((const ExpressionVariable&)(*this));
			return (Variable *)returned;
		}

};


#include "ExpressionParser.h"

class	ParsedExpressionVariable : public Variable {

	protected:
	
		stx::ParseTree expression_tree;
		std::string original_expression;
	public:
	
		ParsedExpressionVariable(string _expression_string);		
		virtual Datum getValue(){
			
			stx::AnyScalar val;
			
			if(expression_tree.isEmpty()){
				// TODO: throw
				throw  SimpleException("Invalid expression", original_expression);
			}
			
			
			try{
				val = expression_tree.evaluate((stx::SymbolTable&)(*(global_variable_registry.get())));

				//std::cout << "evaluated: [" << original_expression << "] = " << val << "\n";
			
			} catch (stx::UnknownSymbolException &e){
				
				FatalParserException f("Unknown symbol while parsing expression");
				
                
			} catch (stx::ExpressionParserException &e){
				merror(M_PARADIGM_MESSAGE_DOMAIN, "expression parser exception");
				//std::cout << "ExpressionParserException: " << e.what() << "\n";
				//return Datum(0L);
                throw(e);
			}
		
		 Datum returnval = val;
			
			return returnval;
		
		}
		
		
		virtual void setValue(Datum val){ return; }
		virtual void setValue(Datum val, MWTime time){ return; }
		virtual void setSilentValue(Datum _value){ return; }
		
		/**
		 *  A polymorphic copy constructor (inherited from Clonable)
		 */
		virtual Variable *clone(){
			ParsedExpressionVariable *returned = 
					new ParsedExpressionVariable((const ParsedExpressionVariable&)(*this));
			return (Variable *)returned;
		}

};
}
#endif


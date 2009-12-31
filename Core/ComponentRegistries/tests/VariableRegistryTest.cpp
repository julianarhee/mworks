/*
 *  VariableRegistryTest.cpp
 *  MonkeyWorksCore
 *
 *  Created by David Cox on 4/11/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "VariableRegistryTest.h"
#include "EventBuffer.h"
using namespace mw;

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( VariableRegistryTestFixture, "Unit Test" );


void VariableRegistryTestFixture::setUp(){
}

void VariableRegistryTestFixture::tearDown(){
}

void VariableRegistryTestFixture::testCodec(){
	fprintf(stderr, "mVariableRegistryTestFixture::testCodec()\n");			
	
	VariableRegistry *r = new VariableRegistry(global_outgoing_event_buffer);

    Datum generated_codec(r->generateCodecDatum());	
	// #codec, #componentCodec, #termination (all generated by default)
	checkCodec(generated_codec.getScarabDatum(), 0);
	CPPUNIT_ASSERT(r->getNVariables() == 0);
	
    Datum test1((float)5);
	VariableProperties props(&test1, 
							  "test",
							  "Test test",
							  "Testy test test test",
							  M_NEVER, 
							  M_WHEN_CHANGED,
							  true, 
							  false,
							  M_CONTINUOUS_INFINITE,
							  "");
	
	shared_ptr<Variable> testvar =
                r->createGlobalVariable(&props);
	int n_variables = 1;
    
	CPPUNIT_ASSERT(r->lookupVariable("test").getInteger() == 5);
	CPPUNIT_ASSERT(r->getNVariables() == n_variables);
	
	
	//r->generateCodec();
    Datum generated_codec2(r->generateCodecDatum());
	
	checkCodec(generated_codec2.getScarabDatum(), n_variables);
	
	shared_ptr<Variable> _testvar = r->getVariable("test");
	shared_ptr<Variable> __testvar = r->getVariable(N_RESERVED_CODEC_CODES);
    CPPUNIT_ASSERT(__testvar != NULL);
	CPPUNIT_ASSERT(_testvar->getValue() == testvar->getValue());
	CPPUNIT_ASSERT(__testvar->getValue() == testvar->getValue());

	CPPUNIT_ASSERT(r->hasVariable("test"));
	CPPUNIT_ASSERT(!r->hasVariable("test2"));
	CPPUNIT_ASSERT(r->getNVariables() == 1);
	
	// regenerate a new registry from the codec
	VariableRegistry *r2 = new VariableRegistry(global_outgoing_event_buffer);
	r2->updateFromCodecDatum(generated_codec2);
    Datum generated_codec3(r2->generateCodecDatum());
	checkCodec(generated_codec3.getScarabDatum(), 1);

	CPPUNIT_ASSERT(r->lookupVariable("test").getInteger() == 5);
	CPPUNIT_ASSERT(r2->lookupVariable("test").getInteger() == 5);
	CPPUNIT_ASSERT(r->getNVariables() == n_variables);
	CPPUNIT_ASSERT(r2->getNVariables() == n_variables);
	
    Datum test2((long)15);
	VariableProperties props2(&test2, 
							   "test2",
							   "Test2 test2",
							   "Testy test2 test2 test2",
							   M_NEVER, 
							   M_WHEN_CHANGED,
							   true, 
							   false,
							   M_CONTINUOUS_INFINITE,
							   "");
	shared_ptr<Variable> testvar2 =
            r2->createGlobalVariable(&props2);
    
    n_variables++;
	
	_testvar = r2->getVariable("test");
	__testvar = r2->getVariable(N_RESERVED_CODEC_CODES);
	CPPUNIT_ASSERT(_testvar->getValue() == testvar->getValue());
	CPPUNIT_ASSERT(__testvar->getValue() == testvar->getValue());
	CPPUNIT_ASSERT(__testvar->getValue() == _testvar->getValue());
	shared_ptr<Variable> _testvar2 = r2->getVariable("test2");
	shared_ptr<Variable> __testvar2 = r2->getVariable(N_RESERVED_CODEC_CODES + 1);
	CPPUNIT_ASSERT(_testvar2->getValue() == testvar2->getValue());
	CPPUNIT_ASSERT(__testvar2->getValue() == testvar2->getValue());
	CPPUNIT_ASSERT(__testvar2->getValue() == _testvar2->getValue());

	CPPUNIT_ASSERT(r2->hasVariable("test"));
	CPPUNIT_ASSERT(r2->hasVariable("test2"));
	CPPUNIT_ASSERT(r2->getNVariables() == n_variables);

	CPPUNIT_ASSERT(r->lookupVariable("test").getInteger() == 5);
	CPPUNIT_ASSERT(r2->lookupVariable("test").getInteger() == 5);
	CPPUNIT_ASSERT(r2->lookupVariable("test2").getInteger() == 15);
	
    Datum generated_codec4(r2->generateCodecDatum());
	checkCodec(generated_codec4.getScarabDatum(), 2);
	
	delete r;
	delete r2;
}


void VariableRegistryTestFixture::checkCodec(ScarabDatum *generated_codec,
											  const int number_of_entries) const {
	
	CPPUNIT_ASSERT( generated_codec != NULL );
	
	CPPUNIT_ASSERT( generated_codec->type == SCARAB_DICT );
	ScarabDatum **keys = scarab_dict_keys(generated_codec);
	CPPUNIT_ASSERT( keys != NULL );
	ScarabDatum **values = scarab_dict_values(generated_codec);
	CPPUNIT_ASSERT( values != NULL );
	
	int dict_length = generated_codec->data.dict->tablesize;
	//		CPPUNIT_ASSERT( dict_length == 1 + 3 );
	
	int num_entries=0;
	for(int i = 0; i < dict_length; i++){
		ScarabDatum *key = keys[i];
		if(key) {
			num_entries++;
			ScarabDatum *value = values[i];
			CPPUNIT_ASSERT( value != NULL );
			CPPUNIT_ASSERT( value->type == SCARAB_DICT );					
		}
	}
	
	CPPUNIT_ASSERT(num_entries == number_of_entries);
}

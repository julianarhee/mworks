/*
 *  HIDFactory.cpp
 *  HIDPlugin
 *
 *  Created by labuser on 8/18/08.
 *  Copyright 2008 MIT. All rights reserved.
 *
 */

#include "USBHID.h"
#include "HIDFactory.h"


boost::shared_ptr<mw::Component> mHIDFactory::createObject(std::map<std::string, std::string> parameters,
														   mw::ComponentRegistry *reg) {
	
	boost::shared_ptr <mw::Component> new_hid = boost::shared_ptr<mw::Component>(new mUSBHID());
	return new_hid;
}
#ifndef _RFID_INTERFACE_HPP
#define _RFID_INTERFACE_HPP

#include <rc522.h>
#include "WebInterface.hpp"

class RFIDInterface {
	rc522_start_args_t startConfig;

	public:
		RFIDInterface();
		void start();
		void registerWebResources(WebInterface* interface);
};

#endif
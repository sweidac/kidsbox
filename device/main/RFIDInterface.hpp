#ifndef _RFID_INTERFACE_HPP
#define _RFID_INTERFACE_HPP

#include <rc522.h>

class RFIDInterface {
	rc522_start_args_t startConfig;

	public:
		RFIDInterface();
		void start();
};

#endif
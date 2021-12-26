#ifndef _WEB_INTERFACE_HPP
#define _WEB_INTERFACE_HPP

#include <ESPAsyncWebServer.h>

class WebInterface {
	AsyncWebServer* server;

	public:
		WebInterface();
		void start();
};

#endif
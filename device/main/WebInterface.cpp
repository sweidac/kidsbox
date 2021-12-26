#include "WebInterface.hpp"

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

WebInterface::WebInterface() {
	server = new AsyncWebServer(80);
}

void WebInterface::start() {
	server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		Serial.println("Received a request");
		request->send(200, "text/plain", "Hello World");
	});

	server->begin();
}
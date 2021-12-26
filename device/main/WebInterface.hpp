#ifndef _WEB_INTERFACE_HPP
#define _WEB_INTERFACE_HPP

#include <esp_http_server.h>

class WebInterface {
	httpd_handle_t server = NULL;

	public:
		WebInterface();
		void start();

	private:
		void initSD();
		httpd_handle_t start_webserver();
		//esp_err_t hello_get_handler(httpd_req_t *req); 
};

#endif
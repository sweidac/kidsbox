#include "RFIDInterface.hpp"

#include <esp_log.h>
#include <esp_err.h>
#include <esp_http_server.h>

#include "WebInterface.hpp"

static const char* TAG = "rfid";

void tag_handler(uint8_t* sn) { // serial number is always 5 bytes long
    ESP_LOGI(TAG, "Tag: %#x %#x %#x %#x %#x",
        sn[0], sn[1], sn[2], sn[3], sn[4]
    );
}

RFIDInterface::RFIDInterface() {
	startConfig = (rc522_start_args_t){
        .miso_io     = 26,
        .mosi_io     = 25,
		.sck_io      = 23,
        .sda_io      = 5,
        .callback    = &tag_handler
	};
}

void RFIDInterface::start() {
	rc522_start(startConfig);
}

void RFIDInterface::registerWebResources(WebInterface* interface) {
    httpd_uri_t config = {
        .uri = "/rfid",
        .method = HTTP_GET,
        .handler = [](httpd_req_t *req) {
            httpd_resp_send(req, "it's all fine!", HTTPD_RESP_USE_STRLEN);
            return ESP_OK;
        }
    };

    interface->registerResource(&config);
}


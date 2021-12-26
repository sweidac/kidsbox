#include "RFIDInterface.hpp"

#include <esp_log.h>
#include <esp_err.h>
#include <esp_http_server.h>
#include <stdio.h>
#include <functional>

#include "WebInterface.hpp"

static const char* TAG = "rfid";

static char* lastTagId = (char*)malloc(26 * sizeof(char));
static std::function<void(char*)> callback;

static char* getLastTagId() {
    return lastTagId;
}

RFIDInterface::RFIDInterface() {
	startConfig = (rc522_start_args_t){
        .miso_io     = 12,
        .mosi_io     = 13,
		.sck_io      = 23,
        .sda_io      = 18,  
        .callback    = [](uint8_t* sn) { // serial number is always 5 bytes long
                ESP_LOGI(TAG, "Tag: %#x %#x %#x %#x %#x",
                    sn[0], sn[1], sn[2], sn[3], sn[4]
                );

                sprintf(lastTagId, "%#x_%#x_%#x_%#x_%#x", sn[0], sn[1], sn[2], sn[3], sn[4]);

                ESP_LOGI(TAG, "sprintf tag id: %s", lastTagId);

                if (callback) {
                    callback(getLastTagId());
                }
            }
	};
}

void RFIDInterface::start() {
	rc522_start(startConfig);
}

void RFIDInterface::registerWebResources(WebInterface* interface) {
    httpd_uri_t config = {
        .uri = "/rfid",
        .method = HTTP_GET,
        .handler = [](httpd_req_t *req)  {
            httpd_resp_send(req, getLastTagId(), HTTPD_RESP_USE_STRLEN);
            return ESP_OK;
        }
    };

    interface->registerResource(&config);
}

void RFIDInterface::registerTagChangeCallback(std::function<void(char*)> cb) {
    callback = cb;
}
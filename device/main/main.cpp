#include "Arduino.h"

#include <WiFi.h>

#include "WebInterface.hpp"

extern "C" {
    void app_main();
}

void connectWifi() {
    WiFi.begin(CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Waiting for connection...");
    }

    Serial.println("Connected.");
    Serial.println(WiFi.localIP());
}

WebInterface webInterface;

void app_main(void) {
    initArduino();

    Serial.begin(115200);

    connectWifi();

    webInterface.start();
}

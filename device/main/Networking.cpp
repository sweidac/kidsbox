#include "Networking.hpp"

#include "WiFi.h"

void Networking::connectWifi() {
	WiFi.begin(CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Waiting for connection...");
    }

    Serial.println("Connected.");
    Serial.println(WiFi.localIP());
}
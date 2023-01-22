#include "controlawifi.h"

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

ControlaWIFI::ControlaWIFI() {
    this->wifiManager.autoConnect("StarON");
    WiFi.hostname("staron");
}
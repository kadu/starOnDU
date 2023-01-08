#pragma once

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>


class ControlaWIFI {
  private:
    WiFiManager wifiManager;

  public:
    ControlaWIFI();
};
#pragma once
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecureBearSSL.h>
#include <string>

#define TOKENURL "https://id.twitch.tv/oauth2/token"
#define PORT 443

class ControlaTwitch {
    private:
        std::string token;
        uint32_t lasTimeGetToken = 0;
        uint32_t latTimeGetStreamerOn = 0;
        bool isStreamerOn = false;
        bool awaitTimeOut(WiFiClientSecure* client);
        bool handGetTwitchToken(std::string ass_, std::string clientId, std::string clientSecret);
        bool getTwitchToken(std::string clientId, std::string clientSecret);
        bool checkToken();
        bool handStreamerIsOn(std::string streamer, std::string clientId);

    public:
        ControlaTwitch(std::string token);
        bool streamerIsOn(std::string streamerName, std::string clientId, std::string clientSecret);
};
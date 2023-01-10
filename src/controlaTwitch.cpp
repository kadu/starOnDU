#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>

#include "controlaTwitch.h"

ControlaTwitch::ControlaTwitch(std::string token) {
    this->token = token;
}

bool ControlaTwitch::awaitTimeOut(WiFiClientSecure *client) {
    delay(1000);
    unsigned long timeout = millis();
    while (client->available() < 64)
    {
        if ((millis() - timeout) > 5000)
        {
            Serial.println(">>> Client Timeout !");
            client->stop();
            return true;
        }
    }
    return false;
}

bool ControlaTwitch::handGetTwitchToken(std::string ass_, std::string clientId, std::string clientSecret) {
    BearSSL::WiFiClientSecure myClient;
    HTTPClient https;

    myClient.setInsecure();

    if (!https.begin(myClient, TOKENURL)) {
        Serial.println("Unable to connect");
        return false;
    }

    https.addHeader("Content-Type", "application/x-www-form-urlencoded");

    char data[180];

    strcpy(data, "client_id=");
    strcat(data, clientId.c_str());
    strcat(data, "&client_secret=");
    strcat(data, clientSecret.c_str());
    strcat(data, "&grant_type=client_credentials");

    Serial.print("clientId: ");
    Serial.println(clientId.c_str());
    Serial.println("clientSecret: ");
    Serial.print(clientSecret.c_str());

    int httpCode = https.POST(data);

    if (httpCode == 0) {
        Serial.printf("[HTTPS] POST... failed, error: %s\r", https.errorToString(httpCode).c_str());
        Serial.println("Error on HTTP request");
        return false;
    }

    Serial.printf("[HTTPS] POST... code: %d len: %d \r", httpCode, https.getSize());
    Serial.println(https.getString());

    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();

        StaticJsonDocument<2048> doc;
        auto error = deserializeJson(doc, payload);
        if (error) {
            Serial.println("deserializeJson() failed: " + String(error.c_str()));
            return false;
        }

        return true;
    }

    return false;
}

bool ControlaTwitch::checkToken()
{
    return strlen(this->token.c_str()) > 0;
}

bool ControlaTwitch::getTwitchToken(std::string clientId, std::string clientSecret)
{
    if (!WiFi.isConnected()) {
        return false;
    }
    if (lasTimeGetToken && (millis() - lasTimeGetToken) < (checkToken() ? 60000 : 3000)) {
        return this->checkToken();
    }

    if (!this->handGetTwitchToken(this->token, clientId, clientSecret)) {
        char tempToken[64];
        memset(tempToken, 0, 64);
        this->token = tempToken;
    }

    lasTimeGetToken = millis();
    return this->checkToken();
}

bool ControlaTwitch::handStreamerIsOn(std::string streamer, std::string clientId)
{
    BearSSL::WiFiClientSecure myClient;
    HTTPClient https;

    myClient.setInsecure();
    char url[256];

    sprintf(url, "https://api.twitch.tv/helix/streams?user_login=%s", streamer.c_str());

    if (!https.begin(myClient, url)) {
        Serial.println("2 connection failed");
        Serial.println("wait 5 sec...");
        return false;
    }

    char bearer[200];
    sprintf(bearer, "Bearer %s", this->token.c_str());
    https.addHeader("Authorization", bearer);
    https.addHeader("Client-Id", clientId.c_str());

    int httpCode = https.GET();

    if (httpCode == 0) {
        Serial.printf("1[HTTPS] GET... failed, error: %s\r", https.errorToString(httpCode).c_str());
        Serial.println("Error on HTTP request");
        return false;
    }

    Serial.printf("2[HTTPS] GET... code: %d len: %d \r", httpCode, https.getSize());
    Serial.println(https.getString());
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();

        StaticJsonDocument<2048> doc;
        auto error = deserializeJson(doc, payload);
        if (error) {
            Serial.println("deserializeJson() failed: " + String(error.c_str()));
            return false;
        }

        const char *user_name = doc["data"][0]["user_name"];
        Serial.println(user_name);

        //TODO: Verificar porque precisa fazer essa checagem
        //String nome = String(user_name);

        // if (nome.equalsIgnoreCase(streamer.c_str())) {
        //     if (streamerName != user_name) {
        //         Serial.print("Corrigindoo nome inserido de: [" + streamerName + "] para [" + user_name + "]");
        //         Serial.print("");
        //         saveData("STREAMER", user_name);
        //         // corrigirnome();

        //         streamerName = user_name;
        //     }
        // }
        return user_name != NULL;
    }

    return false;
}

bool ControlaTwitch::streamerIsOn(std::string streamerName, std::string clientId, std::string clientSecret) {
    if (this->getTwitchToken(clientId, clientSecret)) {
        if ((millis() - latTimeGetStreamerOn) < 3000) {
            return isStreamerOn;
        }

        isStreamerOn = handStreamerIsOn(streamerName, clientId);
        latTimeGetStreamerOn = millis();
        return isStreamerOn;
    }
    return false;
}
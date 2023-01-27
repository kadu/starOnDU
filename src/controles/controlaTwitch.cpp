#include "controlaTwitch.h"
#include <Arduino.h>
#include <string.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>

StaticJsonDocument<2048> doc;

ControlaTwitch::ControlaTwitch()
{

}

void ControlaTwitch::adicionaChaves(char *clientId, char *clientSecret)
{
  Serial.println("adicionachave");
  this->identification.clientId = clientId;
  this->identification.clientSecret = clientSecret;
}

void ControlaTwitch::getAuth()
{
  Serial.println("getauth");
  BearSSL::WiFiClientSecure myClient;
  HTTPClient https;
  myClient.setInsecure();

#ifdef DEBUG
  Serial.print("Trying to get oauth2 token...: ");
#endif

  if (!https.begin(myClient, "https://id.twitch.tv/oauth2/token"))
  {
    Serial.println(F("Unable to connect"));
  }

  https.addHeader("Content-Type", "application/x-www-form-urlencoded");

  char data[180];

  strcpy(data, "client_id=");
  strcat(data, this->getClientId());
  strcat(data, "&client_secret=");
  strcat(data, this->getClientSecret());
  strcat(data, "&grant_type=client_credentials");
  int httpCode = https.POST(data);

  if (httpCode == 0 || httpCode >= 400)
  {
    Serial.println("[HTTPS] POST... failed, error: " + https.errorToString(httpCode));
    Serial.println(F("Error on HTTP request"));
  }


#ifdef DEBUG
  Serial.println("Token received.");
  Serial.println("[HTTPS] POST... code: " + String(httpCode) + " len: " + https.getSize());
  Serial.println("Access token received.");
  Serial.println(https.getString());
#endif

  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
  {
    const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(9) + 1000;

    // Allocate JsonBuffer
    DynamicJsonDocument root(bufferSize);

    // Parse JSON object
    DeserializationError error = deserializeJson(root, https.getString());
    if (error)
    {
      Serial.print(F("getUserData deserializeJson() failed: "));
      Serial.println(error.c_str());
    }
    else
    {
      this->identification.token = (char *)root["access_token"].as<const char *>();
    }
  }
}

  int ControlaTwitch::streamerIsOn(char *streamerName)
  {
    Serial.println("streamerison");
    getAuth();

    BearSSL::WiFiClientSecure myClient;
    HTTPClient https;

    myClient.setInsecure();
    char url[256];

    sprintf(url, "https://api.twitch.tv/helix/streams?user_login=%s", streamerName);
    Serial.println("Getting info from streamer (url): " + String(url));

    if (!https.begin(myClient, url))
    {
        Serial.println(F("connection failed\n wait 5 sec..."));
        return false;
    }

    https.addHeader("Authorization", "Bearer " + String(this->identification.token.c_str()));
    https.addHeader("Client-Id", this->identification.clientId.c_str());

    int httpCode = https.GET();

    if (httpCode == 0)
    {
        Serial.println("[HTTPS] GET... failed, error: " + https.errorToString(httpCode));
        Serial.println(F("Error on HTTP request"));
        return false;
    }

    Serial.println();
    Serial.println("[HTTPS] GET... code: " + String(httpCode) +" len: "+ https.getSize());
    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
    {
        String payload = https.getString();

        auto error = deserializeJson(doc, payload);
        if (error)
        {
            Serial.println("deserializeJson() failed: " + String(error.c_str()));
            return false;
        }

        Serial.println("############# Returned payload: ");
        serializeJsonPretty(doc, Serial);
        Serial.println("");
        Serial.println("############# Payload ended");

        const char *user_name = doc["data"][0]["user_name"];
        bool isOn = user_name != NULL;

        return isOn;
    }

    return false;
  }

  char *ControlaTwitch::getClientId()
  {
    return (char *)this->identification.clientId.c_str();
  }

  char *ControlaTwitch::getClientSecret()
  {
    return (char *)this->identification.clientSecret.c_str();
  }

  void ControlaTwitch::setToken(char *token)
  {
    this->identification.token = token;
  }

  char *ControlaTwitch::getToken()
  {
    return (char *)this->identification.token.c_str();
  }
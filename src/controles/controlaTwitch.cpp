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
  this->identification.clientId = clientId;
  this->identification.clientSecret = clientSecret;
}

void ControlaTwitch::getAuth()
{
  BearSSL::WiFiClientSecure myClient;
  HTTPClient https;
  myClient.setInsecure();

  log("Trying to get oauth2 token...: ");


  if (!https.begin(myClient, "https://id.twitch.tv/oauth2/token"))
  {
    log("Unable to connect\n");
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
    auto logmsg = "[HTTPS] POST... failed, error: " + https.errorToString(httpCode);
    log((char *)logmsg.c_str()); log("\n");
    log("Error on HTTP request");
  }


  log("Token received.\n");
  auto logmsg = "[HTTPS] POST... code: " + String(httpCode) + " len: " + https.getSize();
  log((char *)logmsg.c_str()); log("\n");
  log("Access token received.\n");
  log((char *)https.getString().c_str()); log("\n");

  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
  {
    const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(9) + 1000;

    // Allocate JsonBuffer
    DynamicJsonDocument root(bufferSize);

    // Parse JSON object
    DeserializationError error = deserializeJson(root, https.getString());
    if (error)
    {
      log("getUserData deserializeJson() failed: ");
      log((char *)error.c_str());
      log("\n");
    }
    else
    {
      this->identification.token = (char *)root["access_token"].as<const char *>();
    }
  }
}

int ControlaTwitch::streamerIsOn(char *streamerName)
{
  getAuth();

  BearSSL::WiFiClientSecure myClient;
  HTTPClient https;

  myClient.setInsecure();
  char url[256];

  sprintf(url, "https://api.twitch.tv/helix/streams?user_login=%s", streamerName);
  auto logmsg = "Getting info from streamer (url): " + String(url);
  log((char *)logmsg.c_str()); log("\n");


  if (!https.begin(myClient, url))
  {
    log("connection failed\n wait 5 sec...\n");
    return false;
  }

  https.addHeader("Authorization", "Bearer " + String(this->identification.token.c_str()));
  https.addHeader("Client-Id", this->identification.clientId.c_str());

  int httpCode = https.GET();

  if (httpCode == 0)
  {
    auto logmsg = "[HTTPS] GET... failed, error: " + https.errorToString(httpCode);
    log((char *)logmsg.c_str());
    log("Error on HTTP request");
    return false;
  }

  log("\n");
  auto mensagem = "[HTTPS] GET... code: " + String(httpCode) + " len: " + https.getSize();
  log((char*)mensagem.c_str());
  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
  {
    String payload = https.getString();

    auto error = deserializeJson(doc, payload);
    if (error)
    {
      auto logmsg = "deserializeJson() failed: " + String(error.c_str());
      log((char *)logmsg.c_str()); log("\n");
      return false;
    }

    log("############# Returned payload: \n");
    if(this->isInDebugState)
      serializeJsonPretty(doc, Serial);
    log("############# Payload ended");

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

void ControlaTwitch::setDebugState(bool debugState)
{
  this->isInDebugState = debugState;
}

void ControlaTwitch::log(char *message)
{
  if(this->isInDebugState)
    Serial.print(message);
}
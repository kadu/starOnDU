#pragma once

// #define DEBUG ON  // Descomentar para ver mensagens de debug

struct TWITCH_VARS
{
  char *clientId;
  char *clientSecret;
  char *token;
};

class ControlaTwitch
{
  private:
    TWITCH_VARS identification;

  public:
    ControlaTwitch(char *clientId, char *clientSecret);
    int streamerIsOn(char *streamerName);
    char *getClientId();
    char *getClientSecret();
    char *getToken();
    void setToken(char *token);
    void getAuth();
};
#pragma once
#include <string>
// #define DEBUG ON  // Descomentar para ver mensagens de debug

struct TWITCH_VARS
{
  std::string clientId;
  std::string clientSecret;
  std::string token;
};

class ControlaTwitch
{
  private:
    TWITCH_VARS identification;

  public:
    ControlaTwitch();
    int streamerIsOn(char *streamerName);
    void adicionaChaves(char *clientId, char *clientSecret);
    char *getClientId();
    char *getClientSecret();
    char *getToken();
    void setToken(char *token);
    void getAuth();
    void debuga();
};
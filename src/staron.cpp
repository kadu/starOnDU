#include "staron.h"

#include <string>
#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

StaticJsonDocument<2048> docconfig;
StaticJsonDocument<2048> docarray;

void StarON::adicionaStreamer(int index, std::string nome, int r, int g, int b)
{
    if (index < 4)
    {
        this->streamer[index].nome = nome;
        this->streamer[index].corR = r;
        this->streamer[index].corG = g;
        this->streamer[index].corB = b;
        this->quantidadeStreamers++;
    }
};

void StarON::removeStreamer(int index)
{
    this->adicionaStreamer(index, "", 0, 0, 0);
    this->quantidadeStreamers--;
}

void StarON::adicionaSecretEID(std::string secret, std::string streamerid)
{
    this->secret = secret;
    this->streamerid = streamerid;
}

void StarON::recuperaStreamer(int index)
{
    Serial.print("Nome.: ");
    Serial.println(this->streamer[index].nome.c_str());
    Serial.print("R.: ");
    Serial.println(this->streamer[index].corR);
    Serial.print("G.: ");
    Serial.println(this->streamer[index].corG);
    Serial.print("B.: ");
    Serial.println(this->streamer[index].corB);
}

std::string StarON::recuperaStreamerName(int index)
{
    return this->streamer[index].nome.c_str();
}

int StarON::recuperaStreamerR(int index)
{
    return this->streamer[index].corR;
}

int StarON::recuperaStreamerG(int index)
{
    return this->streamer[index].corG;
}

int StarON::recuperaStreamerB(int index)
{
    return this->streamer[index].corB;
}


void StarON::configura()
{
    if (LittleFS.exists("/streamers.json"))
    {
        File file = LittleFS.open("/streamers.json", "r");

        auto error = deserializeJson(docconfig, file);
        if (error)
        {
            Serial.println("deserializeJson() failed: " + String(error.c_str()));
        }

        JsonArray infos = docconfig.as<JsonArray>();
        int contador = 0;
        for (JsonVariant v : infos)
        {
            auto error = deserializeJson(docarray, v.as<String>());
            if (error)
            {
                Serial.println("deserializeJson() failed: " + String(error.c_str()));
            }
            String strAux = docarray["streamer"].as<String>();
            if(strlen(strAux.c_str()) != 0)
            {
                adicionaStreamer(
                    contador,
                    strAux.c_str(),
                    docarray["R"].as<int>(),
                    docarray["G"].as<int>(),
                    docarray["B"].as<int>()
                );
            }
            contador++;
        }

        file.close();
    }

    if (LittleFS.exists("/secrets.json"))
    {
        File file = LittleFS.open("/secrets.json", "r");

        auto error = deserializeJson(docconfig, file);
        if (error)
        {
            Serial.println("deserializeJson() failed: " + String(error.c_str()));
        }

        if(docconfig["clientId"].as<String>().length() > 0) {
            String clientAux = docconfig["clientId"].as<String>();
            String secretAux = docconfig["secret"].as<String>();
            adicionaSecretEID(secretAux.c_str(), clientAux.c_str());
        }


        file.close();
    }
}

std::string StarON::recuperarClientId()
{
    return this->streamerid;
}
std::string StarON::recuperarSecret()
{
    return this->secret;
}

StarON::StarON()
{
};

void StarON::debug()
{
    Serial.println(this->secret.c_str());
    Serial.println(this->streamerid.c_str());
}
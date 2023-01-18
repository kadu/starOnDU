#include "staron.h"

#include <string>
#include <Arduino.h>

void StarON::adicionaStreamer(int index, std::string nome, int r, int g, int b) {
    if(index < 3) {
        this->streamer[index].nome = nome;
        this->streamer[index].corR = r;
        this->streamer[index].corG = g;
        this->streamer[index].corB = b;
        this->quantidadeStreamers++;
    }

};

void StarON::removeStreamer(int index) {
    this->adicionaStreamer(index, "", 0,0,0);
    this->quantidadeStreamers--;
}

void StarON::adicionaSecretEID(std::string secret, std::string streamerid) {
    this->secret = secret;
    this->streamerid = streamerid;
}

void StarON::recuperaStreamer(int index) {
    Serial.print("Nome.: ");
    Serial.println(this->streamer[index].nome.c_str());

}

StarON::StarON() {
    Serial.println("StarON");
};

void StarON::debug() {
    // Serial.println(this->secret.c_str());
    // Serial.println(this->streamerid.c_str());
}
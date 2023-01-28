#include <Arduino.h>
#include <SimpleTimer.h>
#include <Adafruit_NeoPixel.h>
// #include "controles/controleled.h"
#include "controles/controlawifi.h"
#include "controles/controlaWebserver.h"
#include "controles/controlaTwitch.h"
#include "staron.h"

#define PINODOLED D1
#define NUMERODELEDS 9
#define DEBUG 1

Adafruit_NeoPixel pixels(NUMERODELEDS, PINODOLED, NEO_GRB + NEO_KHZ800);
ControlaWebserver controlaWebserver;
ControlaTwitch controlaTwitch;
StarON starON;
SimpleTimer timer;

void verificaStreamers() {
    Serial.println("verificaStreamer");
    if(!starON.configura()) {
      Serial.print("Sem configurações (Streamers ou API)");
      return;
    }

    for (size_t i = 0; i < 4; i++)
    {
      if((char *)starON.recuperaStreamerName(i).length() > 0) {
        Serial.printf("O Streamer %s está....:", starON.recuperaStreamerName(i).c_str());
        if (controlaTwitch.streamerIsOn((char *)starON.recuperaStreamerName(i).c_str()))
        {
          pixels.setPixelColor(i, starON.recuperaStreamerR(i), starON.recuperaStreamerG(i), starON.recuperaStreamerB(i));
          pixels.show();
          Serial.println("ON");
        }
        else
        {
          pixels.setPixelColor(i, 0,0,0);
          pixels.show();
          Serial.println("OFF");
        }
      }
    }
    // leds.mostra();

  Serial.println("VerificaSctreamers OFF");
}

void setup() {
  // leds.inicializa();
  // leds.limpa();
  // leds.mostra();
  pixels.begin();
  Serial.begin(115200);
  Serial.println(F("\n\nInicializando!\n"));


  ControlaWIFI controlaWIFI;
  controlaWebserver.configura();
  starON.configura();
  controlaTwitch.adicionaChaves(
    (char *)starON.recuperarClientId().c_str(),
    (char *)starON.recuperarSecret().c_str()
  );

  controlaWebserver.forcaAtualizacao(true);
  timer.setInterval(5*60000, verificaStreamers);

  Serial.println("\nFim do Setup");
}


int i = millis() + 5000;
void loop() {
  controlaWebserver.loop();
  if(controlaWebserver.forcaAtualizacao()) {
    Serial.println("Atualizando...");
    verificaStreamers();
    controlaWebserver.forcaAtualizacao(false);
  }
  timer.run();
  if(millis() > i) {
    Serial.print(".");
    i = millis() + 5000;
  }
}
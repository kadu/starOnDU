#include <Arduino.h>
#include "controles/controleled.h"
#include "controles/controlawifi.h"
#include "controles/controlaWebserver.h"
#include "controles/controlaTwitch.h"
#include "staron.h"
#include "segredos/segredos.h"

#define PINODOLED D1
#define NUMERODELEDS 16

ControlaLed leds(PINODOLED, NUMERODELEDS, NEO_BGR + NEO_KHZ800);
ControlaWebserver controlaWebserver;
ControlaTwitch controlaTwitch(STREAMERID, SEGREDO);
StarON starON;

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("Inicializando!");
  leds.inicializa();

  ControlaWIFI controlaWIFI;

  controlaWebserver.configura();

  starON.adicionaSecretEID(SEGREDO, STREAMERID);
  starON.adicionaStreamer(0, "kadu", 10,20,30);
  starON.recuperaStreamer(0);

  int ret = controlaTwitch.streamerIsOn("kaduzius");
  Serial.print("\n\nDebug controlaTwitch.streamerIsOn ");
  Serial.println(ret);

  ret = controlaTwitch.streamerIsOn("em1dio");
  Serial.print("\n\nDebug controlaTwitch.streamerIsOn");
  Serial.println(ret);

  Serial.println("Fim Setup");
}

void loop() {
  controlaWebserver.loop();
}
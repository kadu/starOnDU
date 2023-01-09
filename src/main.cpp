#include <Arduino.h>
#include "controleled.h"
#include "controlawifi.h"
#include "controlaWebserver.h"
#include "staron.h"

#define PINODOLED D1
#define NUMERODELEDS 16

ControlaLed leds(PINODOLED, NUMERODELEDS, NEO_BGR + NEO_KHZ800);
ControlaWebserver controlaWebserver;
StarON starON;

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("Inicializando!");
  leds.inicializa();

  ControlaWIFI controlaWIFI;

  controlaWebserver.configura();

  starON.adicionaSecretEID("teste", "outroteste");
  starON.adicionaStreamer(0, "kadu", 10,20,30);
  starON.recuperaStreamer(0);


  Serial.println("Fim Setup");
}

void loop() {
  controlaWebserver.loop();
  // for (size_t i = 0; i < NUMERODELEDS; i++)
  // {
  //   leds.limpa();
  //   leds.configuraCorDoPixel(i, RED);
  //   leds.mostra();
  //   delay(1200);
  //   leds.configuraCorDoPixel(i, GREEN);
  //   leds.mostra();
  //   delay(1200);
  //   leds.configuraCorDoPixel(i, BLUE);
  //   leds.mostra();
  //   delay(1200);
  // }
}
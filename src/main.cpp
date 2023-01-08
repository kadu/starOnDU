#include <Arduino.h>
#include "controleled.h"

#define PINODOLED D1
#define NUMERODELEDS 16

ControlaLed leds(PINODOLED, NUMERODELEDS, NEO_BGR + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("Inicializando!");
  leds.inicializa();

  Serial.println("Fim Setup");
}

void loop() {
  for (size_t i = 0; i < NUMERODELEDS; i++)
  {
    leds.limpa();
    leds.configuraCorDoPixel(i, RED);
    leds.mostra();
    delay(1200);
    leds.configuraCorDoPixel(i, GREEN);
    leds.mostra();
    delay(1200);
    leds.configuraCorDoPixel(i, BLUE);
    leds.mostra();
    delay(1200);
  }
}
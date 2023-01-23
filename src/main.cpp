#include <Arduino.h>
#include <SimpleTimer.h>
#include "controles/controleled.h"
#include "controles/controlawifi.h"
#include "controles/controlaWebserver.h"
#include "controles/controlaTwitch.h"
#include "staron.h"
#include "segredos/segredos.h"

#define PINODOLED D1
#define NUMERODELEDS 16

ControlaLed leds(PINODOLED, NUMERODELEDS, NEO_GRB + NEO_KHZ800);
ControlaWebserver controlaWebserver;
ControlaTwitch controlaTwitch;
StarON starON;
SimpleTimer timer;

void verificaStreamers() {
    starON.configura();
    Serial.print("Uptime (s): ");
    Serial.println(millis() / 1000);
    leds.limpa();

    int contador = 1;
    for (size_t i = 0; i < 4; i++)
    {
      if((char *)starON.recuperaStreamerName(i).length() > 0) {
        if (controlaTwitch.streamerIsOn((char *)starON.recuperaStreamerName(i).c_str()))
        {
          leds.configuraCorDoPixel(contador, starON.recuperaStreamerR(i), starON.recuperaStreamerG(i), starON.recuperaStreamerB(i));
          Serial.printf("O Streamer %s está ON\n", starON.recuperaStreamerName(i).c_str());
          leds.mostra();
          contador++;
        }
        else {
          leds.configuraCorDoPixel(contador, BLACK);
          Serial.printf("O Streamer %s está OFF\n", starON.recuperaStreamerName(i).c_str());
        }
      }
    }
    leds.mostra();
}


void setup() {
  leds.inicializa();
  leds.limpa();
  leds.mostra();
  Serial.begin(115200);
  delay(3000);
  Serial.println("Inicializando!");

  ControlaWIFI controlaWIFI;
  controlaWebserver.configura();
  starON.configura();
  controlaTwitch.adicionaChaves(
    (char *)starON.recuperarClientId().c_str(),
    (char *)starON.recuperarSecret().c_str()
  );

  verificaStreamers();
  timer.setInterval(5*60000, verificaStreamers);
}

void loop() {
  controlaWebserver.loop();
  timer.run();
}
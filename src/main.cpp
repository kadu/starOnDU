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

ControlaLed leds(PINODOLED, NUMERODELEDS, NEO_BGR + NEO_KHZ800);
ControlaWebserver controlaWebserver;
ControlaTwitch controlaTwitch;
StarON starON;
SimpleTimer timer;

void verificaStreamers() {
    starON.configura();
    Serial.print("Uptime (s): ");
    Serial.println(millis() / 1000);

    for (size_t i = 0; i < 4; i++)
    {
      if((char *)starON.recuperaStreamerName(i).length() > 0) {
        if (controlaTwitch.streamerIsOn((char *)starON.recuperaStreamerName(i).c_str()))
        {
          Serial.printf("O Streamer %s está ON\n\n", starON.recuperaStreamerName(i).c_str());
          Serial.println(starON.recuperaStreamerR(i));
          Serial.println(starON.recuperaStreamerG(i));
          Serial.println(starON.recuperaStreamerB(i));
        }
      }
    }
}


void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando!");
  leds.inicializa();

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
#include <Arduino.h>
#include <SimpleTimer.h>
#include "controles/controleled.h"
#include "controles/controlawifi.h"
#include "controles/controlaWebserver.h"
#include "controles/controlaTwitch.h"
#include "staron.h"

#define PINODOLED D1
#define NUMERODELEDS 9
#define DEBUG 1

ControlaLed leds(PINODOLED, NUMERODELEDS, NEO_GRB + NEO_KHZ800);
ControlaWebserver controlaWebserver;
ControlaTwitch controlaTwitch;
StarON starON;
//SimpleTimer timer;

void verificaStreamers() {
    Serial.println("verificaStreamer");
    if(!starON.configura()) {
      Serial.print("Sem configurações (Streamers ou API)");
      return;
    }

    leds.limpa();

    for (size_t i = 0; i < 4; i++)
    {
      if((char *)starON.recuperaStreamerName(i).length() > 0) {
        Serial.printf("O Streamer %s está....:", starON.recuperaStreamerName(i).c_str());
        if (controlaTwitch.streamerIsOn((char *)starON.recuperaStreamerName(i).c_str()))
        {
          leds.configuraCorDoPixel(i, starON.recuperaStreamerR(i), starON.recuperaStreamerG(i), starON.recuperaStreamerB(i));
          leds.mostra();
          Serial.println("ON");
        }
        else
        {
          leds.configuraCorDoPixel(i, BLACK);
          leds.mostra();
          Serial.println("OFF");
        }
      }
    }
    leds.mostra();

  Serial.println("VerificaSctreamers OFF");
}

void setup() {
  leds.inicializa();
  leds.limpa();
  leds.mostra();
  Serial.begin(115200);
  delay(3000);
  Serial.println(F("\n\nInicializando!\n"));


  ControlaWIFI controlaWIFI;
  controlaWebserver.configura();
  starON.configura();
  controlaTwitch.adicionaChaves(
    (char *)starON.recuperarClientId().c_str(),
    (char *)starON.recuperarSecret().c_str()
  );

  // controlaWebserver.forcaAtualizacao(true);
  //timer.setInterval(5*60000, verificaStreamers);
  //verificaStreamers();

  //controlaTwitch.streamerIsOn("kaduzius");
  // controlaTwitch.streamerIsOn("id_akira");
  // controlaTwitch.streamerIsOn("chartosgameplay");
  // controlaTwitch.streamerIsOn("ladonerd");
  // controlaTwitch.streamerIsOn("jp_amis");
  // controlaTwitch.streamerIsOn("Gaules");
  // controlaTwitch.streamerIsOn("SydHeresy");





  for (size_t i = 0; i < 4; i++)
  {
    char *streamer = (char *)starON.recuperaStreamerName(i).c_str();
    Serial.println(streamer);
    if(strlen(streamer) > 0) {
      Serial.printf("\nO Streamer %s está....:", streamer);
      if (controlaTwitch.streamerIsOn(streamer))
      {
        leds.configuraCorDoPixel(i, starON.recuperaStreamerR(i), starON.recuperaStreamerG(i), starON.recuperaStreamerB(i));
        leds.mostra();
        Serial.println("ON");
      }
      else
      {
        leds.configuraCorDoPixel(i, BLACK);
        leds.mostra();
        Serial.println("OFF");
      }
    }
  }

  Serial.println("Fim do Setup");
}


int i = millis() + 5000;
void loop() {
  controlaWebserver.loop();
//   if(controlaWebserver.forcaAtualizacao()) {
//     Serial.println("Atualizando...");
//     verificaStreamers();
//     controlaWebserver.forcaAtualizacao(false);
//   }
//  // timer.run();
//   if(millis() > i) {
//     Serial.print(".");
//     i = millis() + 5000;
//   }
}
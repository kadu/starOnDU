#include "controleled.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

ControlaLed::ControlaLed(int pin, int numpixels, neoPixelType t)
{
  this->pin = pin;
  this->numpixels = numpixels;

  this->pixels = Adafruit_NeoPixel(numpixels, pin, t);
}

void ControlaLed::inicializa()
{
  this->pixels.begin();
}
void ControlaLed::limpa()
{
  this->pixels.clear();
  this->pixels.show();
}

void ControlaLed::mostra() {
  this->pixels.show();
}

void ControlaLed::configuraCorDoPixel(uint16_t n, uint32_t c) {
  this->pixels.setPixelColor(n, c);  }

void ControlaLed::configuraCorDoPixel(uint16_t n, uint8_t r, uint8_t g,uint8_t b) {
  this->pixels.setPixelColor(n, r, g, b);
}
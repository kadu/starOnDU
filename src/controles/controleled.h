#pragma once
#include <Adafruit_NeoPixel.h>

#define RED        (uint32_t)0xFF0000
#define GREEN      (uint32_t)0x00FF00
#define BLUE       (uint32_t)0x0000FF
#define WHITE      (uint32_t)0xFFFFFF
#define BLACK      (uint32_t)0x000000
#define YELLOW     (uint32_t)0xFFFF00
#define CYAN       (uint32_t)0x00FFFF
#define MAGENTA    (uint32_t)0xFF00FF
#define PURPLE     (uint32_t)0x400080
#define ORANGE     (uint32_t)0xFF3000
#define PINK       (uint32_t)0xFF1493
#define GRAY       (uint32_t)0x101010
#define ULTRAWHITE (uint32_t)0xFFFFFFFF

class ControlaLed {

    private:
        int pin, numpixels;
        Adafruit_NeoPixel pixels;

    public:
        ControlaLed(int pin = 6, int numpixels = 4, neoPixelType t = NEO_BGR + NEO_KHZ800);
        void inicializa();
        void limpa();
        void configuraCorDoPixel(uint16_t n, uint32_t c);
        void configuraCorDoPixel(uint16_t n, uint8_t r, uint8_t g,uint8_t b);
        void mostra();

};
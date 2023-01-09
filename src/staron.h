#pragma once

#include <string>

struct Streamer {
    std::string nome;
    int corR;
    int corG;
    int corB;
};

class StarON {
    private:
        std::string secret;
        std::string streamerid;
        Streamer streamer[4];
        int quantidadeStreamers;

    public:
        StarON();
        void adicionaStreamer(int index, std::string nome, int r, int g, int b);
        void adicionaSecretEID(std::string secret, std::string streamerid);
        void removeStreamer(int index);
        void recuperaStreamer(int index);

};
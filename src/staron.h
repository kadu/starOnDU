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
        Streamer streamer[9];
        int quantidadeStreamers;

    public:
        StarON();
        bool configura();
        void adicionaStreamer(int index, std::string nome, int r, int g, int b);
        void adicionaSecretEID(std::string secret, std::string streamerid);
        void removeStreamer(int index);
        void recuperaStreamer(int index);
        int recuperaStreamerR(int index);
        int recuperaStreamerG(int index);
        int recuperaStreamerB(int index);
        std::string recuperaStreamerName(int index);
        std::string recuperarClientId();
        std::string recuperarSecret();

        void debug();
};
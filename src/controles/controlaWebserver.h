#pragma once

#include <ESP8266WebServer.h>

class ControlaWebserver {
    private:
        ESP8266WebServer server = {80};
        void notFound();
        void index();
        bool handleFileRead(String path);

    public:
        ControlaWebserver() = default;
        ControlaWebserver(int port): server{port} {};
        void configura();
        void loop();


};
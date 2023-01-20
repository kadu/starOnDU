#include "controlaWebserver.h"

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

void ControlaWebserver::configura()
{
    LittleFS.begin();
    this->server.onNotFound(std::bind(&ControlaWebserver::notFound, this));
    this->server.on("/setstreamer", std::bind(&ControlaWebserver::setstreamers, this));
    this->server.on("/setsecrets", std::bind(&ControlaWebserver::setsecrets, this));
    this->server.begin();

    MDNS.begin("StarON");
    MDNS.addService("http", "tcp", 80);
};

void ControlaWebserver::notFound()
{
    Serial.println("NotFound");

    if (!handleFileRead(server.uri()))
    {
        Serial.println("NotFound sem arquivo");
        String message = "File Not Found\n\n";
        message += "URI: ";
        message += server.uri();
        message += "\nMethod: ";
        message += (server.method() == HTTP_GET) ? "GET" : "POST";
        message += "\nArguments: ";
        message += server.args();
        message += "\n";

        for (uint8_t i = 0; i < server.args(); i++)
        {
            message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
        }

        this->server.send(404, "text/plain", message);
    }
};

void ControlaWebserver::loop()
{
    this->server.handleClient();

    MDNS.update();
}

bool ControlaWebserver::handleFileRead(String path)
{
    if (path.endsWith("/"))
    {
        path += "index.html";
    }
    String contentType = "text/html";
    if (path.endsWith(".css"))
    {
        contentType = "text/css";
    }
    else if (path.endsWith(".js"))
    {
        contentType = "application/javascript";
    }
    else if (path.endsWith(".ico"))
    {
        contentType = "image/x-icon";
    }
    else if (path.endsWith(".json")) {
        contentType = "application/json";
    }
    File file = LittleFS.open(path, "r");
    if (file)
    {
        server.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;
}

void ControlaWebserver::setstreamers()
{
    File file = LittleFS.open("/streamers.json", "w");
    String json = this->server.arg("plain");
    Serial.println(json);
    file.print(json);
    file.close();
    server.send(200, "application/json", "{\"status\":\"success\"}");
}

void ControlaWebserver::setsecrets()
{
    File file = LittleFS.open("/secrets.json", "w");
    String json = this->server.arg("plain");
    Serial.println(json);
    file.print(json);
    file.close();
    server.send(200, "application/json", "{\"status\":\"success\"}");
}
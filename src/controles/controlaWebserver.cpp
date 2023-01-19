#include "controlaWebserver.h"

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>

void ControlaWebserver::configura()
{
    LittleFS.begin();
    this->server.onNotFound(std::bind(&ControlaWebserver::notFound, this));
    //this->server.on("/", std::bind(&ControlaWebserver::index, this));
    this->server.begin();

    MDNS.begin("StarON");
    MDNS.addService("http", "tcp", 80);
};

void ControlaWebserver::notFound()
{
    if (!handleFileRead(server.uri()))
    {
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

void ControlaWebserver::index()
{
    StreamString pagina;

    pagina.reserve(500);
    pagina.printf("\
<!DOCTYPE html>\
<html lang='en'>\
<head>\
    <meta charset='UTF-8'>\
    <meta http-equiv='X-UA-Compatible' content='IE=edge'>\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
    <title>Document</title>\
</head>\
<body>\
    <h2>Deu bom</h2>\
</body>\
</html>\
    ");

    this->server.send(200, "text/html", pagina.c_str());
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
    File file = LittleFS.open(path, "r");
    if (file)
    {
        server.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;
}
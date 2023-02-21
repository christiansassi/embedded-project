#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include "esp32_async_server.h"

//*****************************************************************************
//
//! Starts Access Point with specified characteristics
//
//*****************************************************************************
void ServerClass::startServer()
{
    this->mode = "-";
    this->lastMeasure = "-";
    this->liveMeasure = "-";

    stopServer();

    WiFi.softAP(SERVER_SSID, SERVER_PASSWORD, SERVER_CHANNEL, SERVER_SSID_HIDDEN, SERVER_MAX_CONNECTION);
    
    this->server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    this->server.on("/updateLiveMeasure", HTTP_GET, [&](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", this->liveMeasure.c_str());
    });

    this->server.on("/updateLastMeasure", HTTP_GET, [&](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", this->lastMeasure.c_str());
    });

    this->server.on("/updateMode", HTTP_GET, [&](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", this->mode.c_str());
    });

    this->server.begin();
}

//*****************************************************************************
//
//! Stops Access Point
//
//*****************************************************************************
void ServerClass::stopServer()
{
    WiFi.softAPdisconnect(true);
}

//*****************************************************************************
//
//! Updates mode
//! `mode` specifies the desired new mode.
//
//*****************************************************************************
void ServerClass::setMode(String mode)
{
    if(mode != "-")
        this->mode = mode + " mode";
    else
        this->mode = "-";
}

//*****************************************************************************
//
//! Updates last measure for the current mode
//! `lastMeasure` last detected measure for the current mode
//! `unit` specifies the unit for the provided measure (e.g. cm)
//
//*****************************************************************************
void ServerClass::setLastMeasure(String lastMeasure, String unit)
{
    if(lastMeasure != "-")
        this->lastMeasure = "Last measure: " + lastMeasure + " " + unit;
    else
        this->lastMeasure = "-";
}

//*****************************************************************************
//
//! Updates live measure for the current mode
//! `liveMeasure` current detected measure for the current mode
//! `unit` specifies the unit for the provided measure (e.g. cm)
//
//*****************************************************************************
void ServerClass::setLiveMeasure(String liveMeasure, String unit)
{
    if(liveMeasure != "-")
        this->liveMeasure = liveMeasure + " " + unit;
    else    
        this->liveMeasure = "-";
}

ServerClass Server;

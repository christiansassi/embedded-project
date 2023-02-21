#ifndef ESP32_ASYNC_SERVER_H
#define ESP32_ASYNC_SERVER_H

#define SERVER_SSID "ESP32"
#define SERVER_PASSWORD "12345678"
#define SERVER_CHANNEL 1
#define SERVER_SSID_HIDDEN false
#define SERVER_MAX_CONNECTION 4
#define SERVER_DEFAULT_PORT 80

#include <WiFi.h>
#include <ESPAsyncWebServer.h>

//*****************************************************************************
//
//! This is the html, css and javascript code of the webpage hosted by the ESP32
//
//*****************************************************************************
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1" />
        <style>
            body {
                font-family: Arial, sans-serif;
                color: rgb(190, 0, 0);
                background-color: black;
                margin: 0;
                overflow: hidden;
            }

            header {
                align-items: center;
                padding: 10px;
                position: fixed;
                width: 100%;
            }

            header h1 {
                text-align: center;
                font-size: 8vw;
                width: 100%;
                margin-top: 20px;
            }

            .container {
                display: flex;
                flex-direction: column;
                align-items: center;
                height: 100vh;
                justify-content: center;
            }

            .live-measure {
                font-size: 18vw;
                color: rgb(255, 0, 0);
            }

            .last-measure {
                font-size: 5vw;
            }
        </style>
    </head>
    <body onload="setInterval(function() {updateLiveMeasure(); updateLastMeasure(); updateMode();}, 100)">
        <header>
            <h1 id="mode"></h1>
        </header>

        <div class="container">
            <div class="live-measure" id="livem">bLong</div>
            <div class="last-measure" id="lastm"></div>
        </div>

        <script type="text/javascript">
            function updateLiveMeasure() {
                let xhttp = new XMLHttpRequest();
                xhttp.open("GET", "updateLiveMeasure");

                xhttp.onreadystatechange = function () {
                    if (this.readyState == 4 && this.status == 200) 
                    {
                        if(this.responseText == "-")
                        {
                            document.getElementById("livem").innerText = "bLong";
                        }
                        else
                        {
                            document.getElementById("livem").innerText =
                                this.responseText;
                        }
                    }
                };

                xhttp.send();
            }

            function updateLastMeasure() {
                let xhttp = new XMLHttpRequest();
                xhttp.open("GET", "updateLastMeasure");

                xhttp.onreadystatechange = function () {
                    if (this.readyState == 4 && this.status == 200) {

                        if(this.responseText == "-")
                        {
                            document.getElementById("lastm").innerText = "";
                        }
                        else
                        {
                            document.getElementById("lastm").innerText =
                                this.responseText;
                        }
                    }
                };

                xhttp.send();
            }

            function updateMode() {
                let xhttp = new XMLHttpRequest();
                xhttp.open("GET", "updateMode");

                xhttp.onreadystatechange = function () {
                    if (this.readyState == 4 && this.status == 200) {

                        if(this.responseText == "-")
                        {
                            document.getElementById("mode").innerText = "";
                        }
                        else
                        {
                            document.getElementById("mode").innerText =
                                this.responseText;
                        }
                    }
                };

                xhttp.send();
            }
        </script>
    </body>
</html>
)rawliteral";

class ServerClass
{
    private:

        AsyncWebServer server{SERVER_DEFAULT_PORT};

        String mode;
        String lastMeasure;
        String liveMeasure;

    public:

        void startServer();
        void stopServer();

        void setMode(String mode);
        void setLastMeasure(String lastMeasure, String unit);
        void setLiveMeasure(String liveMeasure, String unit);

        String getMode();
        String getLastMeasure();
        String getLiveMeasure();
};

String getMode();

extern ServerClass Server;

#endif
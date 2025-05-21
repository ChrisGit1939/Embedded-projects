#include "main.h"

const char *ssid = "ESP32-Captive-Portal";
const char *password = "123456789";

bool scanInProgress = false;

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

AsyncWebServer server(80);
DNSServer dns;

String current_ssid = "";
String current_pass = "";

void wifi_transmit_init(void)
{
    WiFi.mode(WIFI_AP_STA);
    delay(100);

    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(ssid, password);

    // Nie łączymy się od razu z żadnym SSID jako STA

    // DNS przekierowujący
    dns.start(53, "*", local_IP);



    // Endpoint: skanowanie dostępnych sieci Wi-Fi
    server.on("/scan_wifi", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        if (!scanInProgress) 
        {
            Serial.println("-> Rozpoczynam asynchroniczne skanowanie Wi-Fi...");
            scanInProgress = true;
            WiFi.scanNetworks(true); // true = asynchroniczne
            request->send(200, "application/json", "[]"); // tymczasowo pusta odpowiedź
        } 
        else 
        {
            int status = WiFi.scanComplete();
            if (status == WIFI_SCAN_RUNNING) 
            {
                request->send(200, "application/json", "[]"); // nadal skanuje
            } 
            else if (status >= 0) 
            {
                Serial.printf("-> Zakończono skanowanie. Znaleziono %d sieci.\n", status);
                String json = "[";
                for (int i = 0; i < status; i++) 
                {
                    if (i > 0) json += ",";
                    json += "{\"ssid\":\"" + WiFi.SSID(i) + "\",\"rssi\":" + String(WiFi.RSSI(i)) + "}";
                }
                json += "]";
                WiFi.scanDelete(); // czyść wyniki
                scanInProgress = false;
                request->send(200, "application/json", json);
            } 
            else 
            {
                Serial.println("-> Błąd skanowania!");
                scanInProgress = false;
                request->send(500, "application/json", "[]");
            }
        }
    });

    // Obsługa zapisu SSID/hasła przez POST (opcjonalne)
    server.on("/save_wifi", HTTP_POST, [](AsyncWebServerRequest *request)
    {
        if (request->hasParam("ssid", true) && request->hasParam("password", true)) 
        {
            current_ssid = request->getParam("ssid", true)->value();
            current_pass = request->getParam("password", true)->value();

            Serial.printf("Zapisano dane WiFi (RAM): SSID = %s, HASLO = %s\n", current_ssid.c_str(), current_pass.c_str());

            // Próbujemy się połączyć z nową siecią
            WiFi.disconnect();
            WiFi.mode(WIFI_AP_STA);
            WiFi.begin(current_ssid.c_str(), current_pass.c_str());

            int tries = 0;
            while (WiFi.status() != WL_CONNECTED && tries < 20) 
            {
                delay(500);
                Serial.print(".");
                tries++;
                if (WiFi.status() == WL_CONNECT_FAILED) 
                {
                    Serial.println("\nNieudane logowanie (connect failed)!");
                    break;
                }
            }

            if (WiFi.status() == WL_CONNECTED) 
            {
                Serial.println("\nPołączenie udane!");
                request->send(LittleFS, "/index.html", "text/html");
            } 
            else 
            {
                Serial.println("\nBłąd logowania do WiFi!");
                WiFi.disconnect(true);   // upewniamy się czy odłączono
                WiFi.mode(WIFI_AP_STA);  // wracamy do AP+STA
           
                request->send(LittleFS, "/index.html", "text/html");
            }

        } 
        else 
        {
            request->send(400, "text/plain", "Brak ssid lub hasla");
        }
    });

    server.on("/internet_status", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        bool connected = WiFi.status() == WL_CONNECTED;
        String json = "{";
        json += "\"connected\":" + String(connected ? "true" : "false");
        json += ",\"ssid\":\"" + WiFi.SSID() + "\"";
        json += ",\"rssi\":" + String(WiFi.RSSI());
        json += ",\"ip\":\"" + WiFi.localIP().toString() + "\"";
        json += "}";
        request->send(200, "application/json", json);
    });

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/index.html", "text/html");
    });

    server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request){
        request->redirect("http://192.168.4.1/");
    });

    server.on("/gen_204", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/index.html", "text/html");
    });

    server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/index.html", "text/html");
    });

    server.on("/ncsi.txt", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/index.html", "text/html");
    });

    server.on("/connecttest.txt", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/index.html", "text/html");
    });

    server.on("/redirect", HTTP_GET, [](AsyncWebServerRequest *request){
        request->redirect("http://192.168.4.1/");
    });

    server.onNotFound([](AsyncWebServerRequest *request){
        request->redirect("http://192.168.4.1");
    });

    server.serveStatic("/", LittleFS, "/");
    server.begin();

    Serial.println("Access Point Init zakończony");
}
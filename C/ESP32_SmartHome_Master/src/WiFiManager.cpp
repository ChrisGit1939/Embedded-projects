#include "main.h"

 // Definiowanie SSID i hasła do Wi-Fi (dla trybu AP)
 const char *ssid = "ESP32-Captive-Portal";
 const char *password = "123456789";




  // Adres IP dla ESP32
  IPAddress local_IP(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  // Ustawienie ESP32 jako Access Point


  // Tworzenie obiektu serwera i DNS
  AsyncWebServer server(80);
  DNSServer dns;

void acces_point_init(void)
{
    WiFi.mode(WIFI_AP_STA); // <<<<<< WAŻNE! AP + STA jednocześnie
    delay(100);


    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(ssid, password);

    // --- Połączenie z routerem Wi-Fi jako STA ---
    const char* sta_ssid = "HUAWEI_B818_A495";
    const char* sta_password = "12345678";



    WiFi.begin(sta_ssid, sta_password);

    Serial.print("Łączenie z routerem WiFi");

    // Czekanie na połączenie (max 10 sekund)
    uint8_t retries = 20;
    while (WiFi.status() != WL_CONNECTED && retries > 0) 
    {
        delay(500);
        Serial.print(".");
        retries--;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("Połączono z routerem Wi-Fi!");
        Serial.print("Adres IP z routera: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("");
        Serial.println("Nie udało się połączyć z routerem!");
    }
  
     // Uruchomienie serwera DNS, który przekierowuje na ESP32 w każdej lokalizacji
     dns.start(53, "*", local_IP);
  
     // Obsługa głównej strony HTML
     server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
     {
     // Przekierowanie użytkownika do strony powitalnej
         
         request->send(LittleFS, "/index.html", "text/html");
     });
  
     server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request)
     {
         request->redirect("http://192.168.4.1/");
     });
  
     server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request)
     {
         request->send(LittleFS, "/index.html", "text/html");  // Android czeka na "204 No Content", więc to go zmyli
     });
  
     server.on("/gen_204", HTTP_GET, [](AsyncWebServerRequest *request)
     {
         request->send(LittleFS, "/index.html", "text/html");  // alternatywny endpoint
     });
  
     server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request)
     {
         request->send(LittleFS, "/index.html", "text/html");  // iOS
     });
  
     server.on("/ncsi.txt", HTTP_GET, [](AsyncWebServerRequest *request)
     {
         request->send(LittleFS, "/index.html", "text/html");  // Windows
     });
  
     server.on("/connecttest.txt", HTTP_GET, [](AsyncWebServerRequest *request)
     {
         request->send(LittleFS, "/index.html", "text/html");
     });
  
     server.on("/redirect", HTTP_GET, [](AsyncWebServerRequest *request)
     {
         request->redirect("http://192.168.4.1/");
     });
  
     server.on("/ncsi.txt", HTTP_GET, [](AsyncWebServerRequest *request)
     {
         request->redirect("http://192.168.4.1/");
     });
  
     server.on("/connecttest.txt", HTTP_GET, [](AsyncWebServerRequest *request)
     {
         request->redirect("http://192.168.4.1/");
     });
  
     // Obsługa wszystkich zapytań do nieistniejących zasobów
     server.onNotFound([](AsyncWebServerRequest *request)
     {
         request->redirect("http://192.168.4.1");
     });
  
     // Uruchomienie serwera
     server.serveStatic("/", LittleFS, "/");
     server.begin();
  
     Serial.println("Acces Point Init");
 
}
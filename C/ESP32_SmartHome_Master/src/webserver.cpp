#include "main.h"

void webserver_init(void)
{
    // Serwowanie formularza konfiguracji Wi-Fi
    server.on("/wifi_form", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(LittleFS, "/wifi_form.html", "text/html");
    });
    
    // Odbiór danych z formularza
    server.on("/save_wifi", HTTP_POST, [](AsyncWebServerRequest *request)
    {
            String ssid = "";
            String password = "";
    
            // Pobieranie danych z POST
            if (request->hasParam("ssid", true)) 
            {
                ssid = request->getParam("ssid", true)->value();
            }
            if (request->hasParam("password", true)) 
            {
                password = request->getParam("password", true)->value();
            }
    
            Serial.println("== Odebrano dane Wi-Fi ==");
            Serial.println("SSID: " + ssid);
            Serial.println("Hasło: " + password);

    
            // Odpowiadamy użytkownikowi
            request->send(200, "text/html", "<h1>Połączenie w toku...</h1><p>SSID: " + ssid + "</p>");
    
            // Próba połączenia się z siecią Wi-Fi
            WiFi.begin(ssid.c_str(), password.c_str());
    
            // (Możesz tu potem dodać sprawdzanie, czy udało się połączyć)

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
        


    });
    
        
    // Dla głównej strony (index)
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(LittleFS, "/index.html", "text/html");
    });

}

#include "main.h"

String Welcome() 
{
  String html = "<html><head><title>Captive Portal</title></head><body>";
  html += "<h1>Witaj w Captive Portal!</h1>";
  html += "<p>Połączono z siecią Wi-Fi.</p>";
  html += "</body></html>";
  return html;
}
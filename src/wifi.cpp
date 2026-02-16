#include <Arduino.h>
#include <WiFi.h>
#include "globals.h"
#include "wifi.h"

void HTTPClient(void)
{
  WiFiClient client = server.available();   // Listen for incoming clients
  if(client)
  {
    while(client.connected())
    {
      if(client.available())
      {

      }
    }
  }

}

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BluetoothSerial.h>
#include <SD.h>
#include <FS.h>
#include "globals.h"
#include "defines.h"
#include "enumerations.h"
#include "init_variables.h"
#include "oled.h"
#include "error.h"
#include "setup.h"

void SerialSetUp(void)
{
  Serial.begin(115200);
  Serial.println(F("\nESP32 Serial Viewer"));
  Serial.println(firmware_version);
}

void WiFiSetup(void)
{
  const char* ssid     = "ESP32-Serial-Viewer";
  const char* password = "123456789";
  if(WiFi.softAP(ssid, password))
  {
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    server.begin();
    display_ok();
  }
  else
  {
    display_fail();
  }
}

void OLEDSetUp(void)
{
  Wire.begin(OLED_SDA, OLED_SCL); // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS, false, false)) // Address 0x3C for 128x32
  {
    SetErrorID(OLED_ERROR);
    display_fail();
  }
  else
  {
    ClearErrorID(OLED_ERROR);
    display.clearDisplay(); // Clear display
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);  // Set the cursor to the top-left corner of the display
    display.println("ESP32 SERIAL VIEWER");
    display.println(firmware_version);
    display.println("---------------------");
    display_ok(); // print OK message
  }
}

void BluetoothSetUP(void)
{
  if(SerialBT.begin(bluetooth_name)) //Bluetooth device name
  {
    ClearErrorID(BLUETOOTH_ERROR);
    display_ok();
  }
  else
  {
    SetErrorID(BLUETOOTH_ERROR);
    display_fail();
  }
}

void LoRaSetUp(void)
{
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS); // Initialize SPI for LoRa tranceiver
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
  if (!LoRa.begin(LORA_BAND))
  {
    SetErrorID(LORA_ERROR);
    display_fail();
  }
  else
  {
    ClearErrorID(LORA_ERROR);
    display_ok();
  }
}

void SDSetup(void)
{
  sd_spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, sd_spi))
  {
    SetErrorID(SD_CARD_ERROR);
    display_fail();
  }
  else
  {
    ClearErrorID(SD_CARD_ERROR);
    display_ok();
  }
}

static const uint32_t baud_options[]   = {115200, 57600, 38400, 19200, 9600};
static const uint32_t parity_options[] = {SERIAL_8N1, SERIAL_8E1};
static const char*    baud_labels[]    = {"115200", "57600", "38400", "19200", "9600"};
static const char*    parity_labels[]  = {"None", "Even"};

void ConfigureUART(uint8_t baud_choice, uint8_t parity_choice)
{
  if (baud_choice < 1 || baud_choice > NUM_BAUD_OPTIONS)
  {
    SerialBT.println("Error: invalid baud rate selection.");
    return;
  }
  if (parity_choice < 1 || parity_choice > NUM_PARITY_OPTIONS)
  {
    SerialBT.println("Error: invalid parity selection.");
    return;
  }

  uint32_t baud   = baud_options[baud_choice - 1];
  uint32_t config = parity_options[parity_choice - 1];

  Serial.end();
  Serial.begin(baud, config);

  char msg[64];
  snprintf(msg, sizeof(msg), "UART configured: %s baud, %s parity",
           baud_labels[baud_choice - 1], parity_labels[parity_choice - 1]);
  SerialBT.println(msg);
}

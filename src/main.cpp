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
#include "main.h"
#include "globals.h"
#include "defines.h"
#include "enumerations.h"
#include "init_variables.h"
#include "dialogs.h"
#include "setup.h"
#include "oled.h"
#include "error.h"
#include "commands.h"
#include "sd_card.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST); // Initialize SSD1306 display
BluetoothSerial SerialBT;                                               // Initialize bluetooth
SPIClass sd_spi(HSPI);                                                  // Initialize SPI for SD card
WiFiServer server(80);                                                  // Initialize web server to port 80

SYS_STATUS 		SYSTEM_STATUS = SYSTEM_OK;
SYS_STATE  		SYSTEM_STATE  = SYSTEM_RESET;

void setup(void)
{
  // Initialize LED
  pinMode(LED_BUILTIN, OUTPUT);
  // Initialize serial
  SerialSetUp();
  // Initialize OLED
  OLEDSetUp();
  // Initialize WiFi
  WiFiSetup();
  // Initialize LoRa
  LoRaSetUp();
  // Initialize SD Card
  SDSetup();
   // Initialize Bluetooth
  BluetoothSetUP();

  PrintSystemStatus();
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW); // while OLED is running, must set GPIO25 in high
  display.clearDisplay();
  display.setCursor(0, 0);  // Set the cursor to the top-left corner of the display
  display.display();
}

void loop(void)
{
  while(1)
  {
    ErrorHandler();
    StateExecution();
  }
}

/* MAIN FLOW FUNCTIONS */
void ErrorHandler(void)
{
  uint8_t error_index = 0;
  while(error_index < 16) // Check for all possible errors and handle them
  {
    switch(CheckSystemError(error_index))
    {
      case SYSTEM_OK:
        break;
      case SD_CARD_ERROR:
        break;
      default:
        break;
    }
    error_index++;
  }
}

void StateExecution(void)
{
  static char* sys_control;
  switch(SYSTEM_STATE)
  {
    case SYSTEM_RESET:
      SystemPowerUp();
      break;
    case AWAIT_BLE_CLIENT:
      WaitBLEClientConnection();
      break;
    case BLE_CLIENT_CONNECTED:
      sys_control = BLEClientConnected();
      break;
    case COMMAND:
      ExecuteCommand(sys_control);
      break;
    default:
      break;
  }
}

void SystemPowerUp(void)
{
  OLEDDisplayStatus("SYSTEM BOOT");
  SYSTEM_STATE = AWAIT_BLE_CLIENT;
}

void WaitBLEClientConnection(void)
{
  OLEDDisplayStatus("WAIT FOR BLE CLIENT");
  while(!SerialBT.connected())
  {
    // Waiting for BLE client
  }
  SYSTEM_STATE = BLE_CLIENT_CONNECTED;
}

char* BLEClientConnected(void)
{
  char* command;
  static uint8_t greeting_displayed = FALSE;
  OLEDDisplayStatus("BLE CLIENT CONNECTED");
  if(greeting_displayed == FALSE)
  {
    SerialBT.write(bluetooth_greeting, sizeof(bluetooth_greeting)/sizeof(uint8_t) - 1);
    greeting_displayed = TRUE;
  }
  while(SerialBT.connected())
  {
    command = CheckForCommand();
    if(command != NULL)
    {
      SYSTEM_STATE = COMMAND;
      return command;
    }
  }
  if(!SerialBT.connected())
  {
    greeting_displayed = FALSE;
  }
  SYSTEM_STATE = AWAIT_BLE_CLIENT;
  return NULL;
}

void ReadUART(boolean save, char * command)
{
  int data_len;
  char name[32] = "";
  const uint8_t end[1] = {10};
  OLEDDisplayStatus("UART READ MODE");
  if(save == false)
  {
    SerialBT.write((uint8_t*)"ESP READ MODE INITIATED\n", 24);
  }
  else
  {
    if(CheckErrorID(SD_CARD_ERROR))
    {
      SDPrintNoCardError();
      return;
    }
    else
    {
      SerialBT.write((uint8_t*)"ESP LOG MODE INITIATED\n", 23);
      strcpy(name, &command[9]);
    }
  }
  while(SerialBT.connected())
  {
    if(Serial.available())
    {
      digitalWrite(LED_BUILTIN, HIGH); // while OLED is running, must set GPIO25 in high
      String data = Serial.readStringUntil('\n');
      data_len = data.length();
      OLEDScrollTextUp(data.c_str());
      SerialBT.write((uint8_t*)data.c_str(), data_len);
      SerialBT.write(end, 1);
      digitalWrite(LED_BUILTIN, LOW); // while OLED is running, must set GPIO25 in high
      if(save)
      {
        appendFile(SD, name, data.c_str());
      }
    }
    command = CheckForCommand();
    if(command != NULL)
    {
      if(command[0] == STOP_READ)
      {
        SerialBT.write((uint8_t*)"ESP EXIT READ MODE\n", 19);
        SYSTEM_STATE = BLE_CLIENT_CONNECTED;
        return;
      }
    }
  }
  SYSTEM_STATE = AWAIT_BLE_CLIENT;
}

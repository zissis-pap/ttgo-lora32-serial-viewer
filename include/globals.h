/**
  ******************************************************************************
  * @file           : globals.h
  * @brief          : Extern declarations for all shared global objects.
  ******************************************************************************
  */

#ifndef __GLOBALS_H
#define __GLOBALS_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BluetoothSerial.h>
#include <SPI.h>
#include <WiFi.h>
#include "main.h"

extern Adafruit_SSD1306 display;
extern BluetoothSerial SerialBT;
extern SPIClass sd_spi;
extern WiFiServer server;
extern SYS_STATUS SYSTEM_STATUS;
extern SYS_STATE  SYSTEM_STATE;

#endif /* __GLOBALS_H */

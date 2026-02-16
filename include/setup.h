/**
  ******************************************************************************
  * @file           : setup.h
  * @brief          : Declarations for setup functions.
  ******************************************************************************
  */

#ifndef __SETUP_H
#define __SETUP_H

void SerialSetUp(void);
void WiFiSetup(void);
void OLEDSetUp(void);
void BluetoothSetUP(void);
void LoRaSetUp(void);
void SDSetup(void);
void ConfigureUART(uint8_t baud_choice, uint8_t parity_choice);

#endif /* __SETUP_H */

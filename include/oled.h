/**
  ******************************************************************************
  * @file           : oled.h
  * @brief          : Declarations for OLED and display utility functions.
  ******************************************************************************
  */

#ifndef __OLED_H
#define __OLED_H

#include <Arduino.h>

void OLEDDisplayStatus(String data);
void OLEDScrollTextUp(const char* data);
void display_ok(void);
void display_fail(void);

#endif /* __OLED_H */

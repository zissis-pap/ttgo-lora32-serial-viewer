/**
  ******************************************************************************
  * @file           : error.h
  * @brief          : Declarations for error functions.
  ******************************************************************************
  */

#ifndef __ERROR_H
#define __ERROR_H

void PrintSystemStatus(void);
uint16_t CheckSystemError(uint8_t index);
uint8_t CheckErrorID(uint16_t error);
void SetErrorID(uint16_t error);
void ClearErrorID(uint16_t error);

#endif /* __ERROR_H */

/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  * */

#ifndef __MAIN_H
#define __MAIN_H

/* Variables */
typedef uint16_t SYS_STATUS;	/* Stores the current system error
				 	 	 	 	 * System errors are of greater priority than system state */
typedef uint8_t SYS_STATE;		/* Stores the current system state - What routine should the device run */

/* Main flow functions */
void ErrorHandler(void);
void StateExecution(void);
void SystemPowerUp(void);
void WaitBLEClientConnection(void);
char* BLEClientConnected(void);
void ReadUART(boolean save, char * command);

#endif /* __MAIN_H */

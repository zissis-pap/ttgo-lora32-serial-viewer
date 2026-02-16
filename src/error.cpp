#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "globals.h"
#include "enumerations.h"
#include "error.h"

/**
  * @brief  Prints the SYSTEM_STATUS variable
  * @retval none
  */
void PrintSystemStatus(void)
{
  char data[32] = "";
  sprintf(data, "ERROR STATUS: %d", SYSTEM_STATUS);
  Serial.println(data);
  display.println(data);
  display.display();
}

/**
  * @brief  Reads the SYSTEM_STATUS variable
  * @retval the error ID
  */
uint16_t CheckSystemError(uint8_t index)
{
  if(SYSTEM_STATUS&(1<<index))
  {
    return(1 << index);
  }
  return 0;
}

/**
  * @brief  Checks the SYSTEM_STATUS for a specific ID
  * @retval TRUE if the error is found or FALSE if not found
  */
uint8_t CheckErrorID(uint16_t error)
{
  if((SYSTEM_STATUS&error) == error)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/**
  * @brief  Adds a specific Error ID to the SYSTEM_STATUS variable
  * @retval none
  */
void SetErrorID(uint16_t error)
{
  SYSTEM_STATUS = SYSTEM_STATUS | error;
}

/**
  * @brief  Removes a specific Error ID to the SYSTEM_STATUS variable
  * @retval none
  */
void ClearErrorID(uint16_t error)
{
  SYSTEM_STATUS &= ~error;
}

#include <Arduino.h>
#include <BluetoothSerial.h>
#include "globals.h"
#include "enumerations.h"
#include "dialogs.h"
#include "bluetooth.h"
#include "commands.h"
#include "sd_card.h"
#include "oled.h"
#include "main.h"
#include "setup.h"
#include "defines.h"

static uint8_t WaitForUARTChoice(uint8_t max_choice)
{
  while (SerialBT.connected())
  {
    char* input = BluetoothReceive();
    if (input != NULL)
    {
      int choice = atoi(input);
      if (choice >= 1 && choice <= max_choice)
        return (uint8_t)choice;
      SerialBT.println("Invalid selection. Try again.");
    }
  }
  return 0;
}

static void ConfigureUARTWizard(void)
{
  SerialBT.println("\nSelect baud rate:\n  1: 115200\n  2: 57600\n  3: 38400\n  4: 19200\n  5: 9600");
  uint8_t baud_choice = WaitForUARTChoice(NUM_BAUD_OPTIONS);
  if (baud_choice == 0) return;

  SerialBT.println("\nSelect parity:\n  1: None\n  2: Even");
  uint8_t parity_choice = WaitForUARTChoice(NUM_PARITY_OPTIONS);
  if (parity_choice == 0) return;

  ConfigureUART(baud_choice, parity_choice);
}

char* CheckForCommand(void)
{
  char* data = BluetoothReceive();
  if(data != NULL)
  {
    if(strncmp(data, "esp", 3) == 0 && strlen(data) >= 8)
    {
      if      (strncmp(&data[4], "set", 3) == 0)
      {
        if(strncmp(&data[8], "baud", 4) == 0)     data[0] = SET_BAUD_RATE;
      }
      else if (strncmp(&data[4], "read", 4) == 0) data[0] = READ_UART;
      else if (strncmp(&data[4], "stop", 4) == 0) data[0] = STOP_READ;
      else if (strncmp(&data[4], "list", 4) == 0) data[0] = LIST_SD;
      else if (strncmp(&data[4], "save", 4) == 0) data[0] = SAVE_TO_SD;
      else if (strncmp(&data[4], "open", 4) == 0) data[0] = OPEN_FILE;
      else if (strncmp(&data[4], "delt", 4) == 0) data[0] = DELETE_FILE;
      else if (strncmp(&data[4], "help", 4) == 0)  // OK
      {
        SerialBT.write(help_message, sizeof(help_message)/sizeof(uint8_t) - 1);
        return NULL;
      }
      else return NULL;
    return data;
    }
  }
  return NULL;
}

void ExecuteCommand(char* command)
{
  uint8_t command_id;
  if(command != NULL)
  {
    command_id = command[0];
    switch(command_id)
    {
      case READ_UART:
        ReadUART(false, NULL);
        break;
      case LIST_SD:
        SDListDir();
        break;
      case SAVE_TO_SD:
        ReadUART(true, command);
        break;
      case OPEN_FILE:
        SDOpenFile(command);
        break;
      case DELETE_FILE:
        SDDeleteFile(command);
        break;
      case SET_BAUD_RATE:
        ConfigureUARTWizard();
        break;
      default:
        Serial.println("Default");
        break;
    }
  }
  if(SerialBT.connected())
  {
    SYSTEM_STATE = BLE_CLIENT_CONNECTED;
  }
  else
  {
    SYSTEM_STATE = AWAIT_BLE_CLIENT;
  }
}

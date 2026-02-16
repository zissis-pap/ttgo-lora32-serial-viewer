#include <Arduino.h>
#include <BluetoothSerial.h>
#include "globals.h"
#include "bluetooth.h"

char* BluetoothReceive(void)
{
  uint8_t index = 0;
  char c;
  static char buffer[64];
  while(SerialBT.available())
  {
    c = SerialBT.read();
    if(c == '\n')
    {
      buffer[index]     = 10;
      buffer[index + 1] = '\0';
      return buffer;
    }
    if(index >= sizeof(buffer) - 2)
    {
      // Buffer full without newline — discard and signal no command
      return NULL;
    }
    buffer[index] = c;
    index++;
    delay(1); // wait for whatever reason
  }
  return NULL;
}

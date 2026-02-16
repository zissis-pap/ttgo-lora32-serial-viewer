#include <Arduino.h>
#include <SD.h>
#include <FS.h>
#include <BluetoothSerial.h>
#include "globals.h"
#include "enumerations.h"
#include "sd_card.h"
#include "error.h"

void SDListDir(void)
{
  if(CheckErrorID(SD_CARD_ERROR))
  {
    SDPrintNoCardError();
  }
  else
  {
    listDir(SD, "/", 0);
  }
}

void SDOpenFile(const char* file_name)
{
  char buffer[64] = "";
  uint8_t i = 0;
  if(CheckErrorID(SD_CARD_ERROR))
  {
    SDPrintNoCardError();
  }
  else
  {
    while(file_name[9+i] != 10 && file_name[9+i] != '\0' && i < sizeof(buffer) - 1)
    {
      buffer[i] = file_name[9+i];
      i++;
    }
    buffer[i] = '\0';
    readFile(SD, buffer);
  }
}

void SDDeleteFile(const char* file_name)
{
  char buffer[64] = "";
  uint8_t i = 0;
  if(CheckErrorID(SD_CARD_ERROR))
  {
    SDPrintNoCardError();
  }
  else
  {
    while(file_name[9+i] != 10 && file_name[9+i] != '\0' && i < sizeof(buffer) - 1)
    {
      buffer[i] = file_name[9+i];
      i++;
    }
    buffer[i] = '\0';
    deleteFile(SD, buffer);
  }
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels)
{
  char buffer[2048] = "";
  char data[32] = "";
  size_t buf_size = sizeof(buffer);
  size_t used = 0;

  snprintf(buffer, buf_size, "\nListing directory: %s\n", dirname);
  SerialBT.write((uint8_t*)buffer, strlen(buffer));
  buffer[0] = '\0';
  used = 0;

  File root = fs.open(dirname);
  if(!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file)
  {
    if(file.isDirectory())
    {
      snprintf(buffer + used, buf_size - used, "\n  DIR : %s\n", file.name());
      used = strlen(buffer);

      if(levels)
      {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else
    {
      snprintf(data, sizeof(data), "%d", (int)file.size());
      snprintf(buffer + used, buf_size - used, "  FILE: %s  SIZE: %s\n", file.name(), data);
      used = strlen(buffer);
    }
    // Flush if buffer is getting full (leave 128 bytes headroom)
    if(used >= buf_size - 128)
    {
      SerialBT.write((uint8_t*)buffer, used);
      buffer[0] = '\0';
      used = 0;
    }
    file = root.openNextFile();
  }
  if(used > 0)
  {
    SerialBT.write((uint8_t*)buffer, used);
  }
}

void readFile(fs::FS &fs, const char * path)
{
  char data[64] = "";
  uint8_t buff[2];
  uint8_t i = 0;
  sprintf(data, "Reading file: %s\n", path);
  SerialBT.write((uint8_t*)data, strlen(data));
  File file = fs.open(path);
  if(!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }
  while(file.available())
  {
    buff[0] = file.read();
    SerialBT.write(buff, 1);
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message)
{
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file)
  {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message))
  {
    Serial.println("Message appended");
    file.print("\n"); // Add new line character to the end of each line
  }
  else
  {
    Serial.println("Append failed");
  }
  file.close();
}

void deleteFile(fs::FS &fs, const char * path)
{
  Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path))
  {
    Serial.println("File deleted");
  }
  else
  {
    Serial.println("Delete failed");
  }
}

void SDPrintNoCardError(void)
{
  const char* msg = "SD card is not inserted\n";
  SerialBT.write((uint8_t*)msg, strlen(msg));
}

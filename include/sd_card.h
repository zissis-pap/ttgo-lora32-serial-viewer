/**
  ******************************************************************************
  * @file           : sd_card.h
  * @brief          : Declarations for SD card functions.
  ******************************************************************************
  */

#ifndef __SD_CARD_H
#define __SD_CARD_H

#include <FS.h>

void SDListDir(void);
void SDOpenFile(const char* file_name);
void SDDeleteFile(const char* file_name);
void SDPrintNoCardError(void);
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void readFile(fs::FS &fs, const char * path);
void appendFile(fs::FS &fs, const char * path, const char * message);
void deleteFile(fs::FS &fs, const char * path);

#endif /* __SD_CARD_H */

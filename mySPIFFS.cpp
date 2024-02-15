#include "mySPIFFS.h"

void initSPIFFS(void) {
  if (!SPIFFS.begin(true)) {
    DEBUG("An error has occurred while mounting SPIFFS");
  }
  DEBUG("SPIFFS mounted successfully");
}
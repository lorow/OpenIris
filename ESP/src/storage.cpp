#include <LITTLEFS.h>
#include "storage.h"
#define LittleFS LITTLEFS

void OpenIris::Configuration::setup()
{
  // TODO add statuses
  if (already_loaded)
    return;
  bool status = LittleFS.begin();

  if (!status)
  {
    Serial.println("Couldn't mount the file system, formatting");
    status = LittleFS.format();

    if (!status)
    {
      Serial.println("Could not format the file system, ABORTING");
      return;
    }

    status = LittleFS.begin();
    if (!status)
    {
      Serial.println("Could not mount the file system after formatting, ABORTING");
      return;
    }
  }

  if (LittleFS.exists("/config.json"))
  {
    File configFile = LittleFS.open("/config.json", "r");
    StaticJsonDocument<CONFIG_SIZE> config_doc;

    configFile.seek(0);

    DeserializationError error = deserializeJson(config_doc, configFile);
    if (error)
    {
      Serial.println("Failed at parsing the configuration file");
      Serial.println(error.c_str());
      return;
    }
    loadConfig(config_doc);
    already_loaded = true;
  }
}

void OpenIris::Configuration::loadConfig(StaticJsonDocument<CONFIG_SIZE> &config_doc)
{
}

void OpenIris::Configuration::save()
{
}

void OpenIris::Configuration::reset()
{
  LittleFS.format();
}
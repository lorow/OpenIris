#include <LITTLEFS.h>
#include "storage.h"
#define LittleFS LITTLEFS

OpenIris::Configuration::Configuration(char *fileName)
{
  this->configFileName = strcat("/", fileName);
}

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
}

void OpenIris::Configuration::loadConfig()
{
  if (!LittleFS.exists(this->configFileName))
  {
    Serial.print("Config file doesn't exist, default values will be used");
    return;
  }

  File configFile = LittleFS.open(this->configFileName, "r");
  StaticJsonDocument<DESERIALIZE_CONFIG_SIZE> config_doc;

  configFile.seek(0);
  DeserializationError error = deserializeJson(config_doc, configFile);
  configFile.close();

  if (error)
  {
    Serial.println("Failed at parsing the configuration file");
    Serial.println(error.c_str());
    return;
  }

  config.device.name = config_doc["device"]["name"].as<std::string>();

  config.camera.vflip = config_doc["camera"]["vlip"];
  config.camera.href = config_doc["camera"]["href"];
  config.camera.pointX = config_doc["camera"]["pointX"];
  config.camera.pointY = config_doc["camera"]["pointY"];
  config.camera.outputX = config_doc["camera"]["outputX"];
  config.camera.outputY = config_doc["camera"]["outputY"];
  config.camera.quality = config_doc["camera"]["quality"];

  JsonArray WifiArray = config_doc["WifiConfig"].as<JsonArray>();

  for (int i = 0; i < WifiArray.size(); ++i)
  {
    config.networks[i].ssid = WifiArray[i]["ssid"].as<std::string>();
    config.networks[i].password = WifiArray[i]["password"].as<std::string>();
  }

  already_loaded = true;
}

void OpenIris::Configuration::save()
{
  if (LittleFS.exists(this->configFileName))
  {
    LittleFS.remove(this->configFileName);
  }

  File configFile = LittleFS.open(this->configFileName, "w");
  StaticJsonDocument<SERIALIZE_CONFIG_SIZE> configurationDoc;

  configurationDoc["device"]["name"] = this->config.device.name;

  JsonObject camera = configurationDoc.createNestedObject("camera");
  camera["vflip"] = this->config.camera.vflip;
  camera["framesize"] = this->config.camera.framesize;
  camera["href"] = this->config.camera.href;
  camera["pointX"] = this->config.camera.pointX;
  camera["pointY"] = this->config.camera.pointY;
  camera["outputX"] = this->config.camera.outputX;
  camera["outputY"] = this->config.camera.outputY;
  camera["quality"] = this->config.camera.quality;

  JsonObject wifi = configurationDoc.createNestedObject("wifi");

  JsonObject wifi_backup1 = wifi.createNestedObject("backup1");
  wifi_backup1["ssid"] = this->config.networks[0].ssid;
  wifi_backup1["pass"] = this->config.networks[0].password;

  JsonObject wifi_backup2 = wifi.createNestedObject("backup2");
  wifi_backup2["ssid"] = this->config.networks[1].ssid;
  wifi_backup2["pass"] = this->config.networks[1].ssid;

  if (serializeJson(configurationDoc, configFile) == 0)
  {
    Serial.println("Failed to save the config to the json file");
  }
  configFile.close();
}

void OpenIris::Configuration::reset()
{
  LittleFS.format();
}

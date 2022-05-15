#include <LITTLEFS.h>
#include "storage.h"
#define LittleFS LITTLEFS

void OpenIris::Configuration::setup(const char *fileName)
{
  Serial.print("Using configuration from: ");
  Serial.print(this->configFileName);
  Serial.println();

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
    else
      Serial.println("Successfully formatted the file system");
  }
  Serial.println("File system mounted");
}

void OpenIris::Configuration::loadConfig()
{
  if (!LittleFS.exists(this->configFileName))
  {
    Serial.print("Config file doesn't exist, default values will be used");
    return;
  }
  else
    Serial.println("Config found, loading it");

  File configFile = LittleFS.open(this->configFileName, "r");
  if (!configFile)
    Serial.println("Could not open config file, defaults will be used");

  StaticJsonDocument<DESERIALIZE_CONFIG_SIZE> config_doc;

  configFile.seek(0);
  DeserializationError error = deserializeJson(config_doc, configFile);

  if (error)
  {
    Serial.println("Failed at parsing the configuration file, defaults will be used");
    Serial.println(error.c_str());
  }

  configFile.seek(0);
  while (configFile.available())
  {
    Serial.write(configFile.read());
  }

  strlcpy(this->config.device.name, config_doc["device"]["name"], sizeof(this->config.device));
  strlcpy(this->config.device.OTAPassword, config_doc["device"]["OTAPassword"], sizeof(this->config.device.OTAPassword));
  this->config.device.OTAPort = config_doc["device"]["OTAPort"];

  this->config.camera.vflip = config_doc["camera"]["vlip"];
  this->config.camera.href = config_doc["camera"]["href"];
  this->config.camera.pointX = config_doc["camera"]["pointX"];
  this->config.camera.pointY = config_doc["camera"]["pointY"];
  this->config.camera.outputX = config_doc["camera"]["outputX"];
  this->config.camera.outputY = config_doc["camera"]["outputY"];
  this->config.camera.quality = config_doc["camera"]["quality"];

  for (JsonPair wifi_item : config_doc["wifi"].as<JsonObject>())
  {
    const char *ssid = wifi_item.value()["ssid"];
    const char *pass = wifi_item.value()["pass"];
    // in order to reduce memory footprint we will skip loading empty networks
    if (strcmp(ssid, (char *)"") != 0 || strcmp(pass, (char *)"") != 0)
    {
      WiFiConfig network = WiFiConfig();

      strlcpy(network.ssid, ssid, sizeof(network.ssid));
      strlcpy(network.password, pass, sizeof(network.password));
      this->config.networks.push_back(network);
    }
  }

  already_loaded = true;
  configFile.close();
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

  char *main_field = (char *)"main";
  char *backup_field = (char *)"backup";
  char *field_name = nullptr;

  for (uint8_t i = 0; i < this->config.networks.size(); ++i)
  {
    if (i == 0)
      field_name = main_field;
    else
      sprintf(field_name, "%s%d", backup_field, i);

    JsonObject network_config = wifi.createNestedObject(field_name);
    network_config["ssid"] = this->config.networks[i].ssid;
    network_config["pass"] = this->config.networks[i].password;
  }

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

OpenIris::DeviceConfig *OpenIris::Configuration::getDeviceConfig()
{
  return &this->config.device;
}

OpenIris::CameraConfig *OpenIris::Configuration::getCameraConfig()
{
  return &this->config.camera;
}

std::vector<OpenIris::WiFiConfig> *OpenIris::Configuration::getWifiConfigs()
{
  return &this->config.networks;
}
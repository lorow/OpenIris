#include "storage.h"

void OpenIris::Configuration::setup(const char *fileName)
{
  strlcpy(this->configFileName, fileName, sizeof(this->configFileName));

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
    Serial.print("Config file doesn't exist, exiting");
    return;
  }
  else
    Serial.println("Config found, loading it");

  File configFile = LittleFS.open(this->configFileName, "r");
  if (!configFile)
    Serial.println("Could not open config file, default values will be used");

  StaticJsonDocument<DESERIALIZE_CONFIG_SIZE> config_doc;

  configFile.seek(0);
  DeserializationError error = deserializeJson(config_doc, configFile);

  if (error)
  {
    Serial.println("Failed at parsing the configuration file, defaults will be used");
    Serial.println(error.c_str());
  }

#ifdef DEBUG_CONFIG
  configFile.seek(0);
  while (configFile.available())
  {
    Serial.write(configFile.read());
  }
#endif

  JsonObject deviceConfig = config_doc["device"].as<JsonObject>();
  JsonObject cameraConfig = config_doc["camera"].as<JsonObject>();
  this->updateDeviceConfig(deviceConfig, false);
  this->updateCameraConfig(cameraConfig, false);

  for (JsonPair wifi_item : config_doc["wifi"].as<JsonObject>())
  {
    const char *ssid = wifi_item.value()["ssid"];
    const char *pass = wifi_item.value()["pass"];
    // in order to reduce memory footprint we will skip loading empty networks
    if (strcmp(ssid, (char *)"") != 0 || strcmp(pass, (char *)"") != 0)
    {
      WiFiConfig network = WiFiConfig();
      strlcpy(network.name, wifi_item.key().c_str(), sizeof(network.name));
      strlcpy(network.ssid, ssid, sizeof(network.ssid));
      strlcpy(network.password, pass, sizeof(network.password));
      this->config.networks.push_back(network);
    }
  }

  already_loaded = true;
  configFile.close();
  this->notify(OpenIris::ObserverEvent::configLoaded);
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
  configurationDoc["device"]["OTAPassword"] = this->config.device.OTAPassword;
  configurationDoc["device"]["OTAPort"] = this->config.device.OTAPort;

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

  for (uint8_t i = 0; i < this->config.networks.size(); ++i)
  {
    JsonObject network_config = wifi.createNestedObject(this->config.networks[i].name);
    network_config["ssid"] = this->config.networks[i].ssid;
    network_config["pass"] = this->config.networks[i].password;
  }

  if (serializeJson(configurationDoc, configFile) == 0)
  {
    Serial.println("Failed to save the config to the json file");
  }
  configFile.close();
}

void OpenIris::Configuration::updateDeviceConfig(JsonObject &deviceConfig, bool shouldNotify)
{
  strlcpy(this->config.device.name, deviceConfig["name"], sizeof(this->config.device));
  strlcpy(this->config.device.OTAPassword, deviceConfig["OTAPassword"], sizeof(this->config.device.OTAPassword));
  this->config.device.OTAPort = deviceConfig["OTAPort"];
  if (shouldNotify)
    this->notify(OpenIris::ObserverEvent::deviceConfigUpdated);
}

void OpenIris::Configuration::updateCameraConfig(JsonObject &cameraConfig, bool shouldNotify)
{
  this->config.camera.framesize = cameraConfig["framesize"];
  this->config.camera.vflip = cameraConfig["vlip"];
  this->config.camera.href = cameraConfig["href"];
  this->config.camera.pointX = cameraConfig["pointX"];
  this->config.camera.pointY = cameraConfig["pointY"];
  this->config.camera.outputX = cameraConfig["outputX"];
  this->config.camera.outputY = cameraConfig["outputY"];
  this->config.camera.quality = cameraConfig["quality"];
  if (shouldNotify)
    this->notify(OpenIris::ObserverEvent::cameraConfigUpdated);
}

void OpenIris::Configuration::updateNetwork(char *networkName, JsonObject &wifiConfig, bool shouldNotify)
{
  WiFiConfig *networkToUpdate = nullptr;

  for (int i = 0; i < this->config.networks.size(); i++)
  {
    if (strcmp(this->config.networks[i].name, networkName) == 0)
      networkToUpdate = &this->config.networks[i];
  }

  if (networkToUpdate != nullptr)
  {
    strlcpy(networkToUpdate->ssid, wifiConfig["ssid"], sizeof(networkToUpdate->ssid));
    strlcpy(networkToUpdate->password, wifiConfig["ssid"], sizeof(networkToUpdate->password));
    if (shouldNotify)
      this->notify(OpenIris::ObserverEvent::networksConfigUpdated);
  }
}
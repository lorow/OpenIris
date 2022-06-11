#pragma once
#include <LITTLEFS.h>
#include "ArduinoJson.h"
#include "observer.h"
#include <vector>
#include <string>

#define LittleFS LITTLEFS
#define DEBUG_CONFIG true

#define DESERIALIZE_CONFIG_SIZE 1024
#define SERIALIZE_CONFIG_SIZE 1024

namespace OpenIris
{
  struct DeviceConfig
  {
    char name[32];
    char OTAPassword[64];
    int OTAPort = 0;
  };

  struct CameraConfig
  {
    int vflip = 0;
    int framesize = 0;
    int href = 0;
    int quality = 0;
  };

  struct WiFiConfig
  {
    char name[7];
    char ssid[64];
    char password[64];
  };
  struct TrackerConfig
  {
    DeviceConfig device{};
    CameraConfig camera{};
    std::vector<WiFiConfig> networks;
  };

  class Configuration : public ISubject
  {
  public:
    void setup(const char *fileName);
    void loadConfig();

    DeviceConfig *getDeviceConfig() { return &this->config.device; }
    CameraConfig *getCameraConfig() { return &this->config.camera; }
    std::vector<WiFiConfig> *getWifiConfigs() { return &this->config.networks; }

    void updateDeviceConfig(JsonObject &deviceConfig, bool shouldNotify);
    void updateCameraConfig(JsonObject &cameraConfig, bool shouldNotify);
    void updateNetwork(char *networkName, JsonObject &wifiConfig, bool shouldNotify);
    void reset() { LittleFS.format(); }
    void save();

  private:
    char configFileName[20];
    TrackerConfig config;
    bool already_loaded = false;
  };
}
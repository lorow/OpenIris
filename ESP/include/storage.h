#pragma once
#include "ArduinoJson.h"
#include <string>

#define DEBUG_CONFIG true

#define DESERIALIZE_CONFIG_SIZE 2048
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
    int pointX = 0;
    int pointY = 0;
    int outputX = 0;
    int outputY = 0;
    int quality = 0;
  };

  struct WiFiConfig
  {
    char ssid[64];
    char password[64];
  };
  struct TrackerConfig
  {
    DeviceConfig device{};
    CameraConfig camera{};
    std::vector<WiFiConfig> networks;
  };

  class Configuration
  {
  public:
    void setup(const char *fileName);
    void loadConfig();

    DeviceConfig *getDeviceConfig();
    CameraConfig *getCameraConfig();
    std::vector<WiFiConfig> *getWifiConfigs();
    void save();
    void reset();

  private:
    char configFileName[20];
    TrackerConfig config;
    bool already_loaded = false;
  };
}
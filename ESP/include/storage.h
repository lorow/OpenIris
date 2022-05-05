#pragma once
#include "ArduinoJson.h"
#include <string>

#define DEBUG_CONFIG true

#define DESERIALIZE_CONFIG_SIZE 768
#define SERIALIZE_CONFIG_SIZE 768

namespace OpenIris
{
  struct Config
  {
    struct Device
    {
      std::string name = "";
    };

    struct Camera
    {
      // default values to be used by the camera
      // in case of cropping, we shouldn't be setting
      int vflip = 0;
      int framesize = 3;
      int href = 0;
      int pointX = -1;
      int pointY = -1;
      int outputX = -1;
      int outputY = -1;
      int quality = 12;
    };

    struct WiFiConfig
    {
      std::string ssid = "";
      std::string password = "";
    };

    Device device;
    Camera camera;
    WiFiConfig networks[2];
  };

  class Configuration
  {
  public:
    Configuration(char *fileName);
    void setup();
    Config *getConfig();
    void save();
    void reset();

  private:
    char *configFileName;
    Config config;
    void loadConfig();
    bool already_loaded = false;
  };
}
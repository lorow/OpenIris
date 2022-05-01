#include "ArduinoJson.h"

#define CONFIG_SIZE 256

namespace OpenIris
{
  struct Config
  {
    struct Camera
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
      char ssid;
      char password;
    };

    Camera camera;
    std::vector<WiFiConfig> networks;
  };

  class Configuration
  {
  public:
    void setup();
    Config *getConfig();
    void save();
    void reset();

  private:
    void loadConfig(StaticJsonDocument<CONFIG_SIZE> &config_doc);
    bool already_loaded = false;
  };
}
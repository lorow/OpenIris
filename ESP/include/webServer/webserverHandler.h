#pragma once
#include <AsyncTCP.h>
#include "ArduinoJson.h"
#include "storage.h"
#include <ESPAsyncWebServer.h>
#include "esp_camera.h"

namespace OpenIris
{
  class HTTPDHandler
  {
  private:
    void config_update_handler(AsyncWebServerRequest *request);
    void stream_handler(AsyncWebServerRequest *request);
    AsyncWebServer *server;

  public:
    HTTPDHandler();
    void startStreamServer();
  };
}
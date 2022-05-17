#pragma once
#include <WiFi.h>
#include "storage.h"
#include "stateManager.h"
#include "pinout.h"

namespace OpenIris
{
  namespace WiFiHandler
  {
    void setupWifi(OpenIris::StateManager stateManager, OpenIris::Configuration trackerConfig);
  }
}
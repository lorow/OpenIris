#include <Arduino.h>
#include "pinout.h"
#include "WifiHandler.h"
#include "MDNSManager.h"
#include "cameraHandler.h"
#include "LEDManager.h"
#include "webServer/webserverHandler.h"
#include "OTA.h"
#include "storage.h"
#include "StateManager.h"

int STREAM_SERVER_PORT = 80;

auto ota = OpenIris::OTA();
auto ledManager = OpenIris::LEDManager(33);
auto cameraHandler = OpenIris::CameraHandler();
auto stateManager = OpenIris::StateManager();
auto httpdHandler = OpenIris::HTTPDHandler();
auto trackerConfig = OpenIris::Configuration();

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  trackerConfig.setup("config.json");
  trackerConfig.loadConfig();
  ledManager.setupLED();
  cameraHandler.setupCamera();
  OpenIris::WiFiHandler::setupWifi(stateManager, trackerConfig);
  OpenIris::MDNSHandler::setupMDNS(stateManager, trackerConfig);
  httpdHandler.startStreamServer();
  ledManager.on();
  ota.SetupOTA(trackerConfig);
}

void loop()
{
  ota.HandleOTAUpdate();
  ledManager.displayStatus();
}
#include <Arduino.h>
#include "observator.h"
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
auto trackerConfig = OpenIris::Configuration();
OpenIris::CameraHandler *cameraHandler = new OpenIris::CameraHandler();
auto stateManager = OpenIris::StateManager();
auto httpdHandler = OpenIris::HTTPDHandler();

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  ledManager.setupLED();

  trackerConfig.setup("config.json");
  trackerConfig.loadConfig();
  trackerConfig.attach(cameraHandler);

  cameraHandler->setupCamera();
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
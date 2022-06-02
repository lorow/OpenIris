#include "MDNSManager.h"

void OpenIris::MDNSHandler::startMDNS()
{
  auto deviceConfig = trackerConfig->getDeviceConfig();
  if (MDNS.begin(deviceConfig->name))
  {
    stateManager->setState(OpenIris::State::MDNSSuccess);
    MDNS.addService("openIrisTracker", "tcp", 80);
    MDNS.addServiceTxt("openIrisTracker", "tcp", "stream_port", String(80));
    Serial.println("MDNS initialized!");
  }
  else
  {
    stateManager->setState(OpenIris::State::MDNSError);
    Serial.println("Error initializing MDNS");
  }
}

void OpenIris::MDNSHandler::update(ObserverEvent::Event event)
{
  if (event == ObserverEvent::deviceConfigUpdated)
  {
    MDNS.end();
    startMDNS();
  }
}
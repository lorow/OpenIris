#include "WifiHandler.h"

void OpenIris::WiFiHandler::setupWifi(OpenIris::StateManager *stateManager, OpenIris::Configuration *trackerConfig)
{
  Serial.println("Initializing connection to wifi");

  std::vector<WiFiConfig> *networks = trackerConfig->getWifiConfigs();
  int connectionTimeout = 3000;

  for (auto networkIterator = networks->begin(); networkIterator != networks->end(); ++networkIterator)
  {
    Serial.printf("Trying to connect using %s\n\r", networkIterator->ssid);

    int timeSpentConnecting = 0;
    WiFi.begin(networkIterator->ssid, networkIterator->password);

    while (timeSpentConnecting < connectionTimeout && !WiFi.isConnected())
    {
      Serial.print(".");
      timeSpentConnecting += 300;
      delay(300);
    }

    if (!WiFi.isConnected())
      Serial.printf("\n\rCould not connect to %s, trying another network\n\r", networkIterator->ssid);
    else
    {
      Serial.printf("\n\rSuccessfully connected to %s\n\r", networkIterator->ssid);
      stateManager->setState(OpenIris::State::ConnectingToWifiSuccess);
      Serial.print("ESP will be streaming under 'http://");
      Serial.print(WiFi.localIP());
      Serial.print(":80/\r\n");
      Serial.print("ESP will be accepting commands under 'http://");
      Serial.print(WiFi.localIP());
      Serial.print(":80/control\r\n");
      return;
    }
  }

  Serial.println("Could not connected to any of the specified networks, check the configuration and try again");
  stateManager->setState(OpenIris::State::ConnectingToWifiSuccess);
}

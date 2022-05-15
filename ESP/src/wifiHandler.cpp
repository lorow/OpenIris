#include "WifiHandler.h"
#include "GlobalVars.h"

void OpenIris::WiFiHandler::setupWifi()
{
  Serial.println("Initializing connection to wifi");

  std::vector<WiFiConfig> *networks = trackerConfig.getWifiConfigs();
  int connectionTimeout = 3000;

  for (auto it = networks->begin(); it != networks->end(); ++it)
  {
    Serial.printf("Trying to connect using %s\n\r", it->ssid);

    int timeSpentConnecting = 0;
    WiFi.begin(it->ssid, it->password);

    while (timeSpentConnecting < connectionTimeout && !WiFi.isConnected())
    {
      Serial.print(".");
      timeSpentConnecting += 300;
      delay(300);
    }

    if (!WiFi.isConnected())
      Serial.printf("\n\rCould not connect to %s, trying another network\n\r", it->ssid);
    else
    {
      Serial.printf("\n\rSuccessfully connected to %s\n\r", it->ssid);
      stateManager.setState(OpenIris::State::ConnectingToWifiSuccess);
      Serial.print("WiFi connected");
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
  stateManager.setState(OpenIris::State::ConnectingToWifiSuccess);
}

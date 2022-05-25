#include "GlobalVars.h"
#include "webServer/asyncJPGResponse.h"
#include "webServer/webserverHandler.h"

OpenIris::HTTPDHandler::HTTPDHandler(Configuration *TrackerConfig)
{
  this->trackerConfig = TrackerConfig;
  this->server = new AsyncWebServer(80);
}

void OpenIris::HTTPDHandler::startStreamServer()
{
  this->server->on(
      "/",
      HTTP_GET,
      std::bind(&OpenIris::HTTPDHandler::stream_handler, this, std::placeholders::_1));
  this->server->on(
      "/config/update",
      HTTP_GET,
      std::bind(&OpenIris::HTTPDHandler::config_update_handler, this, std::placeholders::_1));

  Serial.println("Initializing web server");
  this->server->begin();
}

void OpenIris::HTTPDHandler::config_update_handler(AsyncWebServerRequest *request)
{
  AsyncWebParameter *p = request->getParam("body", true);
  DynamicJsonDocument doc(DESERIALIZE_CONFIG_SIZE);
  DeserializationError error = deserializeJson(doc, p->value());

  if (error)
    return request->send(403);

  if (doc.containsKey("device"))
  {
    JsonObject deviceConfig = doc["device"].as<JsonObject>();
    this->trackerConfig->updateDeviceConfig(deviceConfig, true);
  }
  if (doc.containsKey("camera"))
  {
    JsonObject cameraConfig = doc["camera"].as<JsonObject>();
    this->trackerConfig->updateCameraConfig(cameraConfig, true);
  }
  if (doc.containsKey("wifi"))
  {
    JsonObject wifiConfig = doc["wifi"].as<JsonObject>();
    // this->trackerConfig->updateNetwork();
  }

  this->trackerConfig->save();

  return request->send(200);
}

void OpenIris::HTTPDHandler::stream_handler(AsyncWebServerRequest *request)
{
  OpenIris::AsyncJpegStreamResponse *response = new OpenIris::AsyncJpegStreamResponse();
  if (!response)
  {
    request->send(501);
    return;
  }
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
}

#include <Arduino.h>
#include "storage.h"
#include "ArduinoJson.h"
#include <unity.h>

String STR_TO_TEST;

auto trackerConfig = OpenIris::Configuration();
String config_name = "/config.json";

void setUp(void)
{
  trackerConfig.setup(config_name.c_str());
  trackerConfig.loadConfig();
}

void tearDown(void)
{
}

void test_loading_config(void)
{
  // todo test this after adding statuses
}

void test_getting_device_config(void)
{
  OpenIris::DeviceConfig *config = trackerConfig.getDeviceConfig();
  UNITY_TEST_ASSERT_NOT_NULL(config, __LINE__, ("Device configuration should not be NULL"));
}

void test_getting_camera_config(void)
{
  OpenIris::CameraConfig *config = trackerConfig.getCameraConfig();
  UNITY_TEST_ASSERT_NOT_NULL(config, __LINE__, ("Device configuration should not be NULL"));
}

void test_getting_networks_config(void)
{
  OpenIris::DeviceConfig *config = trackerConfig.getDeviceConfig();
  UNITY_TEST_ASSERT_NOT_NULL(config, __LINE__, ("Device configuration should not be NULL"));
}

void test_updating_device_config(void)
{
  StaticJsonDocument<SERIALIZE_CONFIG_SIZE> configurationDoc;
  JsonObject deviceConfig = configurationDoc.createNestedObject("camera");

  deviceConfig["name"] = "test";
  deviceConfig["OTAPassword"] = "otapassword";
  deviceConfig["OTAPort"] = 2137;

  trackerConfig.updateDeviceConfig(deviceConfig, false);

  // UNITY_TEST_ASSERT_EQUAL_STRING()
}

void test_updating_camera_config(void)
{
  StaticJsonDocument<SERIALIZE_CONFIG_SIZE> configurationDoc;
  JsonObject cameraConfig = configurationDoc.createNestedObject("camera");

  trackerConfig.updateCameraConfig(cameraConfig, false);
}

void test_updating_network_config()
{

  // found an issue with updating the netoworks. We aren't storing their names!
  StaticJsonDocument<SERIALIZE_CONFIG_SIZE> configurationDoc;
  JsonObject networkConfig = configurationDoc.createNestedObject("camera");

  trackerConfig.updateNetwork(networkConfig, false);
}

void test_saving_config()
{
}

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  delay(10000); // service delay
  UNITY_BEGIN();

  RUN_TEST(test_loading_config);
  RUN_TEST(test_getting_device_config);
  RUN_TEST(test_getting_camera_config);
  RUN_TEST(test_getting_networks_config);
  RUN_TEST(test_updating_device_config);
  RUN_TEST(test_updating_camera_config);
  RUN_TEST(test_updating_network_config);
  RUN_TEST(test_saving_config);

  UNITY_END(); // stop unit testing
}

void loop()
{
}
#pragma once
#include "pinout.h"
#include "observer.h"
#include "storage.h"
#include "esp_camera.h"
#include <Arduino.h>

namespace OpenIris
{
    class CameraHandler : public IObserver
    {
    private:
        sensor_t *camera_sensor;
        camera_config_t config;
        Configuration *trackerConfig;

    public:
        CameraHandler(Configuration *config) : trackerConfig(config) {}
        int setupCamera();
        int setCameraResolution(framesize_t frameSize);
        int setVFlip(int direction);
        int setHFlip(int direction);
        void update(ObserverEvent::Event event);
    };
}
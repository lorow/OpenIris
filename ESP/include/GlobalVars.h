#pragma once
#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include "StateManager.h"
#include "LEDManager.h"
#include "cameraHandler.h"
#include "storage.h"

extern char *MDSNTrackerName;
extern int STREAM_SERVER_PORT;

extern OpenIris::LEDManager ledManager;
extern OpenIris::StateManager stateManager;
extern OpenIris::CameraHandler cameraHandler;
extern OpenIris::Configuration trackerConfig;

#endif
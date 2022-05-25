#pragma once
#include <ESPmDNS.h>
#include "StateManager.h"
#include "storage.h"

namespace OpenIris
{
  namespace MDNSHandler
  {
    void setupMDNS(OpenIris::StateManager *stateManager, OpenIris::Configuration *trackerConfig);
  }
}
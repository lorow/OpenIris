#pragma once
#include <ESPmDNS.h>
#include "StateManager.h"
#include "storage.h"

namespace OpenIris
{
  class MDNSHandler : public IObserver
  {
  private:
    StateManager *stateManager;
    Configuration *trackerConfig;

  public:
    MDNSHandler(StateManager *stateManager, Configuration *trackerConfig) : stateManager(stateManager), trackerConfig(trackerConfig) {}
    void startMDNS();
    void update(ObserverEvent::Event event);
  };
}
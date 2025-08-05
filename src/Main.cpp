#include "Papyrus.h"
#include "Events.h"

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);

    Utils::SetupLog();

    Papyrus::Register();
    Events::SinkEventHandlers();

    return true;
}

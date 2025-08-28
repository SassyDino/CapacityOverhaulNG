#include "Papyrus.h"
#include "Events.h"
#include "MCP.h"

SKSEPluginLoad(const SKSE::LoadInterface *skse)
{
    SKSE::Init(skse);

    Utils::SetupLog();

    Papyrus::Register();
    Events::SinkEventHandlers();

	GUI::MCP::Register();

    return true;
}
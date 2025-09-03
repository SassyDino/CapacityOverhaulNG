#include "Papyrus.h"
#include "Events.h"
#include "MCP.h"

SKSEPluginLoad(const SKSE::LoadInterface *skse)
{
	#ifdef _DEBUG
		if (GetKeyState(VK_RSHIFT) & 0x800) {
			constexpr auto text1 = L"Request for debugger detected. If you wish to attach one and press Ok, do so now if not please press Cancel.";
			constexpr auto text2 = L"Debugger still not detected. If you wish to continue without one please press Cancel.";
			constexpr auto caption = L"Debugger Required (DIMS)";

			int input = 0;

			do
			{
				input = MessageBox(NULL, !input ? text1 : text2, caption, MB_OKCANCEL);
			} while (!IsDebuggerPresent() && input != IDCANCEL);
		}


		if (GetKeyState(VK_RSHIFT) & 0x800 && IsDebuggerPresent())     {
			__debugbreak();
		}
	#endif

    SKSE::Init(skse);

    Utils::SetupLog();

	SKSE::GetMessagingInterface()->RegisterListener(Utils::MessageListener);

    Papyrus::Register();
    Events::SinkEventHandlers();

	GUI::MCP::Register();

    return true;
}
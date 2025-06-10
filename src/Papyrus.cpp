#include "Papyrus.h"
#include "Settings.h"

namespace Papyrus
{
    void CapacityOverhaulNG_MCM::OnConfigClose(RE::TESQuest*)
    {
        Settings::Init();
    }

    bool CapacityOverhaulNG_MCM::Register(RE::BSScript::IVirtualMachine* a_vm)
    {
        a_vm->RegisterFunction("OnConfigClose", "CapacityOverhaulNG_MCM", OnConfigClose);
        return true;
    }

    void Register()
    {
        auto papyrus = SKSE::GetPapyrusInterface();
        papyrus->Register(CapacityOverhaulNG_MCM::Register);
        logger::info("Registered MCM");
    }
}
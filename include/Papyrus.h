#pragma once

namespace Papyrus
{
    class CapacityOverhaulNG_MCM
    {
        public:
            static void OnConfigClose(RE::TESQuest*);

            static bool Register(RE::BSScript::IVirtualMachine* a_vm);
    };

    void Register();
}
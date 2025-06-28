#include "Events.h"
#include "WeightHandler.h"

namespace Events
{
    auto ContainerEventHandler::ProcessEvent(const RE::TESContainerChangedEvent*event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) -> Result {
        auto item = RE::TESForm::LookupByID(event->baseObj)->GetName();
        std::string from{}, to{};
        bool playerContainerEvent = (event->oldContainer || event->newContainer == 0x14);

        if (event->oldContainer) from = RE::TESForm::LookupByID<RE::TESObjectREFR>(event->oldContainer)->GetName();
        if (event->newContainer) to = RE::TESForm::LookupByID<RE::TESObjectREFR>(event->newContainer)->GetName();

        
        if (Settings::globalContainerLog) {
            logger::debug("{}x '{}' from '{}' <0x{:x}> ---> '{}' <0x{:x}>", 
                event->itemCount, item, from, event->oldContainer, to, event->newContainer);
        } else if (Settings::playerContainerLogOnly && playerContainerEvent) {
            logger::debug("{}x '{}' from '{}' <0x{:x}> ---> '{}' <0x{:x}>", 
                event->itemCount, item, from, event->oldContainer, to, event->newContainer);
        }

        return Result::kContinue;
    }

    auto WeightEventHandler::ProcessEvent(const RE::MenuOpenCloseEvent*event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) -> Result {
        bool relevantMenuEvent = (event->menuName == "Cursor Menu");
        auto menuType = event->menuName.c_str();
        std::string str(menuType);

        if (Settings::globalMenuLog) {
            logger::debug("MenuOpenCloseEvent detected by <Events::WeightEventHandler> -> Menu name: '{}', Opening: '{}'", 
                menuType, event->opening);
        } else if (Settings::relevantMenuLogOnly && relevantMenuEvent) {
            logger::debug("MenuOpenCloseEvent detected by <Events::WeightEventHandler> -> Menu name: '{}', Opening: '{}'", 
                menuType, event->opening);
        }

        // Currently only looking out for cursor events, as I think that covers most/all player menus, something else might be better though
        if (relevantMenuEvent) {
            logger::debug("{} identified", event->menuName);
            WeightHandler::UpdateWeightLimit(std::format("[MenuOpenCloseEvent:{}]", menuType));
        }

        return Result::kContinue;
    }

    auto WeightEventHandler::ProcessEvent(const RE::TESLoadGameEvent*, RE::BSTEventSource<RE::TESLoadGameEvent>*) -> Result {
        if (Settings::bLogLoadEvents) logger::debug("LoadGameEvent detected by <Events::WeightEventHandler>");
        
        // LoadGameEvent might potentially be redundant, might be able to just include "Fader Menu" or "Loading Menu" in the menu event processor
        WeightHandler::UpdateWeightLimit("[LoadGameEvent]");
        return Result::kContinue;
    }

    auto WeightEventHandler::ProcessEvent(const RE::TESEquipEvent*event, RE::BSTEventSource<RE::TESEquipEvent>*) -> Result {
        RE::NiPointer<RE::TESObjectREFR> equipActor = event->actor;
        std::string actorName = equipActor->GetName();
        std::string itemName = RE::TESForm::LookupByID<RE::TESObjectREFR>(event->baseObject)->GetName();
        
        RE::FormID actorID = equipActor->formID;
        bool playerEquipEvent = (actorID == 0x14);
        
        if (Settings::globalEquipLog) {
            logger::debug("EquipEvent detected by <Events::WeightEventHandler> -> Actor: '{}', BaseObj: '0x{:x}', originalRef: '0x{:x}', uniqueID: '{}', Equipped: '{}'", 
                actorName, event->baseObject, event->originalRefr, event->uniqueID, event->equipped);
        } else if (Settings::playerEquipLogOnly && playerEquipEvent) {
            logger::debug("EquipEvent detected by <Events::WeightEventHandler> -> Actor: '{}', BaseObj: '0x{:x}', originalRef: '0x{:x}', uniqueID: '{}', Equipped: '{}'", 
                actorName, event->baseObject, event->originalRefr, event->uniqueID, event->equipped);
        }
        
        if (playerEquipEvent) WeightHandler::UpdateWeightLimit(std::format("[EquipEvent:{}:{}]", actorName, itemName));

        return Result::kContinue;
    }

    void ContainerEventHandler::Register() {
        auto* eventSink = new ContainerEventHandler();
        auto* eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventSourceHolder->AddEventSink<RE::TESContainerChangedEvent>(eventSink);
    }

    WeightEventHandler& WeightEventHandler::GetSingleton() {
        static WeightEventHandler singleton;
        return singleton;
    }

    void WeightEventHandler::Register() {
        auto& weightEventHandler = WeightEventHandler::GetSingleton();
        RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(&weightEventHandler);
        RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESLoadGameEvent>(&weightEventHandler);
        RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESEquipEvent>(&weightEventHandler);
    }

    void SinkEventHandlers() {
        ContainerEventHandler::Register();
        WeightEventHandler::Register();
        logger::info("Registered all event handlers.");
    }

}
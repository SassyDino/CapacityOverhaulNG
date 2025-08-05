#include "Events.h"
#include "WeightHandler.h"
#include "CapacityHandler.h"

namespace Events
{
    auto CapacityEventHandler::ProcessEvent(const RE::TESContainerChangedEvent*event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) -> Result {
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

        if (playerContainerEvent) {
            CapacityHandler::Player::AdjustSingleCategory(event);
            if (CapacityHandler::Player::IsOverCapacity()) {
                //TODO: Debuff manager/function
            }
        }

        return Result::kContinue;
    }

    auto CapacityEventHandler::ProcessEvent(const RE::TESEquipEvent*event, RE::BSTEventSource<RE::TESEquipEvent>*) -> Result {
        // if player equip event:
        //      recalculate adjusted capacity limits (to account for backpacks, satchels, etc.)
		//		modify relevant category count (will need an overload of Player::AdjustSingleCategory for TESEquipEvents)
        //      	recount all item categories (include in above method)
        //      check if any categories over capacity
		return Result::kContinue;
    }

    // Try and add SkillIncrease event to recaclulate capacity limits
    // And LevelIncrease event to recalculate weight limit

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
            WeightHandler::UpdateWeightLimit(std::format("MenuOpenCloseEvent:{}", menuType));
        }

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
        
        if (playerEquipEvent) WeightHandler::UpdateWeightLimit(std::format("EquipEvent:{}:{}", actorName, itemName));

        return Result::kContinue;
    }

    auto LoadGameEventHandler::ProcessEvent(const RE::TESLoadGameEvent*, RE::BSTEventSource<RE::TESLoadGameEvent>*) -> Result {
        if (Settings::bLogLoadEvents) logger::debug("LoadGameEvent detected by <Events::LoadGameEventHandler>");
        
        Settings::Init();
        WeightHandler::UpdateWeightLimit("LoadGameEvent");
        CapacityHandler::Limits::UpdateBaseValues;

        return Result::kContinue;
    }

    CapacityEventHandler& CapacityEventHandler::GetSingleton() {
        static CapacityEventHandler singleton;
        return singleton;
    }

    void CapacityEventHandler::Register() {
        auto& capacityEventHandler = CapacityEventHandler::GetSingleton();
        RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESContainerChangedEvent>(&capacityEventHandler);
        RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESEquipEvent>(&capacityEventHandler);
    }

    WeightEventHandler& WeightEventHandler::GetSingleton() {
        static WeightEventHandler singleton;
        return singleton;
    }

    void WeightEventHandler::Register() {
        auto& weightEventHandler = WeightEventHandler::GetSingleton();
        RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(&weightEventHandler);
        RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESEquipEvent>(&weightEventHandler);
    }

    LoadGameEventHandler& LoadGameEventHandler::GetSingleton() {
        static LoadGameEventHandler singleton;
        return singleton;
    }

    void LoadGameEventHandler::Register() {
        auto& loadGameEventHandler = LoadGameEventHandler::GetSingleton();
        RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESLoadGameEvent>(&loadGameEventHandler);
    }

    void SinkEventHandlers() {
        CapacityEventHandler::Register();
        WeightEventHandler::Register();
        LoadGameEventHandler::Register();
        logger::info("Registered all event handlers.");
    }

}
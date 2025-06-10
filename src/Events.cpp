#include "Events.h"

namespace Events
{
    auto ContainerHandler::ProcessEvent(const RE::TESContainerChangedEvent*event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) -> Result {
            auto item = RE::TESForm::LookupByID(event->baseObj)->GetName();
            std::string from{}, to{};
            bool playerContainerEvent = (event->oldContainer || event->newContainer == 0x14);

            if (event->oldContainer) from = RE::TESForm::LookupByID<RE::TESObjectREFR>(event->oldContainer)->GetName();
            if (event->newContainer) to = RE::TESForm::LookupByID<RE::TESObjectREFR>(event->newContainer)->GetName();

            if (Settings::globalLog) {
                logger::debug("{}x '{}' from '{}' <0x{:x}> ---> '{}' <0x{:x}>", event->itemCount, item, from, event->oldContainer, to, event->newContainer);
            } else if (Settings::playerLogOnly && playerContainerEvent) {
                logger::debug("{}x '{}' from '{}' <0x{:x}> ---> '{}' <0x{:x}>", event->itemCount, item, from, event->oldContainer, to, event->newContainer);
            }

            return Result::kContinue;
    }

    void ContainerHandler::Register() {
        auto* eventSink = new ContainerHandler();
        auto* eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventSourceHolder->AddEventSink<RE::TESContainerChangedEvent>(eventSink);
    }

    void SinkEventHandlers() {
        ContainerHandler::Register();
        logger::info("Registered event handlers.");
    }

}
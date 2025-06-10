#include <spdlog/sinks/basic_file_sink.h>
#include "Papyrus.h"
#include "Events.h"

void SetupLog() 
{
    auto logsFolder = logger::log_directory();
    if (!logsFolder) SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
    auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
    auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
    auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
    auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
    spdlog::set_default_logger(std::move(loggerPtr));
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::trace);
}

struct EventProcessor : public RE::BSTEventSink<RE::TESContainerChangedEvent> {
    RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent*event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) {
        
        auto item = RE::TESForm::LookupByID(event->baseObj)->GetName();
        std::string from{};
        std::string to{};
        if (event->oldContainer) from = RE::TESForm::LookupByID<RE::TESObjectREFR>(event->oldContainer)->GetName();
        if (event->newContainer) to = RE::TESForm::LookupByID<RE::TESObjectREFR>(event->newContainer)->GetName();
        
        if (Settings::bLogContainerEvents && !Settings::bLogPlayerContainerEvents){
            logger::debug("{}x '{}' from '{}' <0x{:x}> ---> '{}' <0x{:x}>", event->itemCount, item, from, event->oldContainer, to, event->newContainer);
        } else if (Settings::bLogContainerEvents && Settings::bLogPlayerContainerEvents) {
            if (event->oldContainer || event->newContainer == 0x14) {
                logger::debug("{}x '{}' from '{}' <0x{:x}> ---> '{}' <0x{:x}>", event->itemCount, item, from, event->oldContainer, to, event->newContainer);
            }
        }
        
        return RE::BSEventNotifyControl::kContinue;
    }
};

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);

    SetupLog();

    auto* eventSink = new EventProcessor();
    auto* eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
    eventSourceHolder->AddEventSink<RE::TESContainerChangedEvent>(eventSink);

    Papyrus::Register();

    return true;
}

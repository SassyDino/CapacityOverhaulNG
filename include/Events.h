#pragma once

namespace Events
{
    using Result = RE::BSEventNotifyControl;

    class ContainerEventHandler : public RE::BSTEventSink<RE::TESContainerChangedEvent> {
        public:
            virtual Result ProcessEvent(const RE::TESContainerChangedEvent*event, RE::BSTEventSource<RE::TESContainerChangedEvent>*source) override;

            static void Register();
    };

    class WeightEventHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent>, public RE::BSTEventSink<RE::TESEquipEvent>, public RE::BSTEventSink<RE::TESLoadGameEvent> {
        WeightEventHandler() = default;
        ~WeightEventHandler() = default;
        WeightEventHandler(const WeightEventHandler&) = delete;
        WeightEventHandler(WeightEventHandler&&) = delete;
        WeightEventHandler& operator=(const WeightEventHandler&) = delete;
        WeightEventHandler& operator=(WeightEventHandler&&) = delete;

        public:
            virtual Result ProcessEvent(const RE::MenuOpenCloseEvent*event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*source) override;
            virtual Result ProcessEvent(const RE::TESEquipEvent*event, RE::BSTEventSource<RE::TESEquipEvent>*source) override;
            virtual Result ProcessEvent(const RE::TESLoadGameEvent*event, RE::BSTEventSource<RE::TESLoadGameEvent>*source) override;

            static WeightEventHandler& GetSingleton();

            static void Register();
    };

    void SinkEventHandlers();
}
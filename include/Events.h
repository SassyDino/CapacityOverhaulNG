#pragma once

namespace Events
{
    using Result = RE::BSEventNotifyControl;

    class CapacityEventHandler : public RE::BSTEventSink<RE::TESContainerChangedEvent>, public RE::BSTEventSink<RE::TESEquipEvent> {
        CapacityEventHandler() = default;
        ~CapacityEventHandler() = default;
        CapacityEventHandler(const CapacityEventHandler&) = delete;
        CapacityEventHandler(CapacityEventHandler&&) = delete;
        CapacityEventHandler& operator=(const CapacityEventHandler&) = delete;
        CapacityEventHandler& operator=(CapacityEventHandler&&) = delete;

        public:
			// Trigger capacity system methods (adjust count for relevant item category) upon player-centric ContainerChangedEvents (e.g. picking up or dropping an item).
            virtual Result ProcessEvent(const RE::TESContainerChangedEvent*event, RE::BSTEventSource<RE::TESContainerChangedEvent>*source) override;
			// Trigger capacity system methods (adjust count for relevant item category) upon player-centric EquipEvents.
            virtual Result ProcessEvent(const RE::TESEquipEvent*event, RE::BSTEventSource<RE::TESEquipEvent>*source) override;

            static CapacityEventHandler& GetSingleton();

            static void Register();
    };

    class WeightEventHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent>, public RE::BSTEventSink<RE::TESEquipEvent> {
        WeightEventHandler() = default;
        ~WeightEventHandler() = default;
        WeightEventHandler(const WeightEventHandler&) = delete;
        WeightEventHandler(WeightEventHandler&&) = delete;
        WeightEventHandler& operator=(const WeightEventHandler&) = delete;
        WeightEventHandler& operator=(WeightEventHandler&&) = delete;

        public:
			// Trigger weight system methods (recalculate carry capacity) upon certain MenuOpenCloseEvents.
            virtual Result ProcessEvent(const RE::MenuOpenCloseEvent*event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*source) override;
			// Trigger weight system methods (recalculate carry capacity) upon player-centric EquipEvents.
            virtual Result ProcessEvent(const RE::TESEquipEvent*event, RE::BSTEventSource<RE::TESEquipEvent>*source) override;

            static WeightEventHandler& GetSingleton();

            static void Register();
    };

    class LoadGameEventHandler : public RE::BSTEventSink<RE::TESLoadGameEvent> {
        LoadGameEventHandler() = default;
        ~LoadGameEventHandler() = default;
        LoadGameEventHandler(const LoadGameEventHandler&) = delete;
        LoadGameEventHandler(LoadGameEventHandler&&) = delete;
        LoadGameEventHandler& operator=(const LoadGameEventHandler&) = delete;
        LoadGameEventHandler& operator=(LoadGameEventHandler&&) = delete;

        public:
            virtual Result ProcessEvent(const RE::TESLoadGameEvent*event, RE::BSTEventSource<RE::TESLoadGameEvent>*source) override;

            static LoadGameEventHandler& GetSingleton();

            static void Register();
    };

    void SinkEventHandlers();
}
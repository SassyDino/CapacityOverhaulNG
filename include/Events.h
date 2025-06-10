#pragma once

namespace Events
{
    using Result = RE::BSEventNotifyControl;

    class ContainerHandler : public RE::BSTEventSink<RE::TESContainerChangedEvent> {
        public:
            virtual Result ProcessEvent(const RE::TESContainerChangedEvent*event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) override;

            static void Register();
    };

    void SinkEventHandlers();
}
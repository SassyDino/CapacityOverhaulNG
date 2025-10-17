#pragma once
#include "REX/REX/Singleton.h"

namespace Events
{
    using Result = RE::BSEventNotifyControl;

    class CapacityEventHandler : public REX::Singleton<CapacityEventHandler>, public RE::BSTEventSink<RE::TESContainerChangedEvent>, public RE::BSTEventSink<RE::TESEquipEvent>, 
		public RE::BSTEventSink<RE::SkillIncrease::Event>, public RE::BSTEventSink<RE::TESMagicEffectApplyEvent> {

        public:
			// Trigger capacity system methods (adjust count for relevant item category) upon player-centric ContainerChangedEvents (e.g. picking up or dropping an item).
            virtual Result ProcessEvent(const RE::TESContainerChangedEvent*event, RE::BSTEventSource<RE::TESContainerChangedEvent>*source) override;
			// Trigger capacity system methods (adjust count for relevant item category) upon player-centric EquipEvents.
            virtual Result ProcessEvent(const RE::TESEquipEvent*event, RE::BSTEventSource<RE::TESEquipEvent>*source) override;
			// Trigger capacity system methods (adjust count for relevant item category) upon player Skill increase.
			virtual Result ProcessEvent(const RE::SkillIncrease::Event*event, RE::BSTEventSource<RE::SkillIncrease::Event>*source) override;

			virtual Result ProcessEvent(const RE::TESMagicEffectApplyEvent*event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>*source) override;

            static void Register();
    };

    class WeightEventHandler : public REX::Singleton<WeightEventHandler>, public RE::BSTEventSink<RE::MenuOpenCloseEvent>, public RE::BSTEventSink<RE::TESEquipEvent>, 
		public RE::BSTEventSink<RE::LevelIncrease::Event>, public RE::BSTEventSink<RE::TESMagicEffectApplyEvent> {

        public:
			// Trigger weight system methods (recalculate carry capacity) upon certain MenuOpenCloseEvents.
            virtual Result ProcessEvent(const RE::MenuOpenCloseEvent*event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*source) override;
			// Trigger weight system methods (recalculate carry capacity) upon player-centric EquipEvents.
            virtual Result ProcessEvent(const RE::TESEquipEvent*event, RE::BSTEventSource<RE::TESEquipEvent>*source) override;
			// Trigger capacity system methods (adjust count for relevant item category) upon player Level increase.
			virtual Result ProcessEvent(const RE::LevelIncrease::Event*event, RE::BSTEventSource<RE::LevelIncrease::Event>*source) override;

			virtual Result ProcessEvent(const RE::TESMagicEffectApplyEvent*event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>*source) override;

            static void Register();
    };

    class UIEventHandler : public REX::Singleton<UIEventHandler>, public RE::BSTEventSink<RE::TESLoadGameEvent> {

        public:
            virtual Result ProcessEvent(const RE::TESLoadGameEvent*event, RE::BSTEventSource<RE::TESLoadGameEvent>*source) override;

            static void Register();
    };

    void SinkEventHandlers();
}
#include "Events.h"
#include "WeightHandler.h"
#include "CapacityHandler.h"
#include "BuffsDebuffs.h"
#include "Player.h"
#include "ExtraStorage.h"
#include "FormHandler.h"
#include "Hooks.h"
#include "Logging.h"

namespace Events
{
	auto EventHandler::ProcessEvent(const SKSE::CrosshairRefEvent*event, RE::BSTEventSource<SKSE::CrosshairRefEvent>*) -> Result {
		Hooks::crosshair_ref = event->crosshairRef;

		return Result::kContinue;
	}

	auto EventHandler::ProcessEvent(const RE::TESContainerChangedEvent*event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) -> Result {
        bool playerEvent = (event->oldContainer == 0x14 || event->newContainer == 0x14);
        if (!playerEvent) { return Result::kContinue; }

		auto item = RE::TESForm::LookupByID(event->baseObj)->GetName();
		std::string from{}, to{};
		if (event->oldContainer) from = RE::TESForm::LookupByID<RE::TESObjectREFR>(event->oldContainer)->GetName();
		if (event->newContainer) to = RE::TESForm::LookupByID<RE::TESObjectREFR>(event->newContainer)->GetName();

		// Consuming or dropping items triggers ContainerChangedEvents with a newContainer of 0x0, but dropping items provides a refHandle we can use to filter out consumption triggers
		if (auto refHandle = event->reference; (event->newContainer > 0x0) || refHandle) {
			logger::trace("<TESContainerChangedEvent> -> {}x '{}' from '{}' <0x{:X}> ---> '{}' <0x{:X}> | uID = {}", 
				event->itemCount, item, from, event->oldContainer, to, event->newContainer, event->uniqueID
			);

			CapacityHandler::AdjustSingleCategory(event);

			//TODO: Create WeightEventHandler function for ContainerChangedEvent, and move CheckWeight() into it
			SKSE::GetTaskInterface()->AddTask([]() {
				Debuffs::CapacityEffects();
				Debuffs::CheckWeight(); 
			});
		}
        
        return Result::kContinue;
    }

	auto EventHandler::ProcessEvent(const RE::TESEquipEvent*event, RE::BSTEventSource<RE::TESEquipEvent>*) -> Result {
		auto equipActor = event->actor.get();
		auto item = RE::TESForm::LookupByID(event->baseObject);

		bool playerEvent = equipActor->GetFormID() == 0x14;

		bool validCapacityItem = item->Is(RE::FormType::Armor) || item->Is(RE::FormType::Weapon) || item->Is(RE::FormType::AlchemyItem) || item->Is(RE::FormType::Ingredient);
		bool invalidWeightItem = item->Is(RE::FormType::Spell) || item->Is(RE::FormType::AlchemyItem) || item->Is(RE::FormType::Ingredient) || item->Is(RE::FormType::Book) || item->Is(RE::FormType::Ammo) || item->Is(RE::FormType::Enchantment);

		if (!playerEvent || (!validCapacityItem && invalidWeightItem)) { return Result::kContinue; }

		logger::trace("<TESEquipEvent> -> Actor: '{}', BaseObj: '{} [0x{:X}]', Equipped: '{}', oRef: '0x{:X}', uID: '{}'",
			equipActor->GetName(), item->GetName(), event->baseObject, event->equipped, event->originalRefr, event->uniqueID
		);

		// Prevent equipped apparel from taking up storage space/capacity slots
		if (validCapacityItem) {
			auto itemCategory = CapacityHandler::GetCategoryForEquip(item);
			logger::trace("Equipped Item Category: {}", itemCategory->name);

			if (!itemCategory->isWeaponCat) {
				bool isBag = CapacityHandler::Bonus::ItemIsStorage(item);
				if (event->equipped) {
					itemCategory->DecreaseCount(1);
					if (isBag) { CapacityHandler::Bonus::AddEquippedStorage(event->baseObject); }
				} else {
					itemCategory->IncreaseCount(1);
					if (isBag) { CapacityHandler::Bonus::RemoveEquippedStorage(event->baseObject); }
				}
			}

			SKSE::GetTaskInterface()->AddTask([]() {
				CapacityHandler::CalculateActualCapacities();
				CapacityHandler::UpdateTotalCount();
				CapacityHandler::LogAllCategories();
				Debuffs::CapacityEffects();
			});
		}

        if (!invalidWeightItem) {
			SKSE::GetTaskInterface()->AddTask([]() {
				WeightHandler::UpdateWeightLimit();
			});
			
			Logging::Testing::LogStaminaAVs();
        }

		return Result::kContinue;
    }

	auto EventHandler::ProcessEvent(const RE::TESMagicEffectApplyEvent*event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>*) -> Result {
		auto playerEvent = (event->caster->GetFormID() == 0x14) || (event->target->GetFormID() == 0x14);
		if (!playerEvent) { return Result::kContinue; }

		auto MGEFID = event->magicEffect;
		if (Forms::IsCONGForm(MGEFID)) { return Result::kContinue; }
		
		auto MGEF = RE::TESForm::LookupByID<RE::EffectSetting>(MGEFID);
		if (!MGEF) { return Result::kContinue; }

		logger::trace("<TESMagicEffectApplyEvent> -> Caster: '{}', Target: '{}', MGEF: '{} 0x[{:X}]'", 
			event->caster->GetName(), event->target->GetName(), RE::TESForm::LookupByID(event->magicEffect)->GetName(), event->magicEffect
		);

		// CapacityEventHandler
		auto MGEFAV = MGEF->data.primaryAV;
		bool isValid = (MGEFAV == RE::ActorValue::kAlchemy) || (MGEFAV == RE::ActorValue::kArchery) || (MGEFAV == RE::ActorValue::kSpeech) || (MGEFAV == RE::ActorValue::kLockpicking) || (MGEFAV == RE::ActorValue::kPickpocket);
		
		if (isValid) {
			SKSE::GetTaskInterface()->AddTask([]() {
				CapacityHandler::CalculateActualCapacities();
				CapacityHandler::LogAllCategories();
				Debuffs::CapacityEffects();
			});
		}

		if ((MGEF->data.castingType == RE::MagicSystem::CastingType::kConcentration) || (MGEFID == Forms::Effect::UnspecifiedDamageFFAimed)) { return Result::kContinue; }

		SKSE::GetTaskInterface()->AddTask([]() {
			WeightHandler::UpdateWeightLimit();
		});

		return Result::kContinue;
	}

	auto EventHandler::ProcessEvent(const RE::SkillIncrease::Event*event, RE::BSTEventSource<RE::SkillIncrease::Event>*) -> Result {
		logger::trace("<SkillIncrease::Event> -> Skill: '{}'", event->actorValue);

		CapacityHandler::CalculateActualCapacities();
		CapacityHandler::LogAllCategories();
		Debuffs::CapacityEffects();

		return Result::kContinue;
	}

	auto EventHandler::ProcessEvent(const RE::LevelIncrease::Event*event, RE::BSTEventSource<RE::LevelIncrease::Event>*) -> Result {
		logger::trace("<LevelIncrease::Event> -> Level: {}", event->newLevel);

		WeightHandler::UpdateWeightLimit();

		return Result::kContinue;
	}

	auto EventHandler::ProcessEvent(const RE::MenuOpenCloseEvent*event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) -> Result {
		//NOTE: If I get some sort of Magic Effect event working, I might not even need Menu events
		//? If not though, I could consider: InventoryMenu, MagicMenu, ContainerMenu, StatsMenu
        bool relevantMenuEvent = ((event->menuName == "InventoryMenu") || (event->menuName == "StatsMenu"));
        if (!relevantMenuEvent) {return Result::kContinue;}

		//TODO: This is effectively disabling this event, for testing. If I decide to keep this event, remove this check.
        if (relevantMenuEvent) {return Result::kContinue;}

		logger::trace("<MenuOpenCloseEvent> -> Menu name: '{}', Opening: '{}'", event->menuName.c_str(), event->opening);
		
		WeightHandler::UpdateWeightLimit();

        return Result::kContinue;
    }

	auto EventHandler::ProcessEvent(const RE::TESLoadGameEvent*, RE::BSTEventSource<RE::TESLoadGameEvent>*) -> Result {
        logger::trace("<TESLoadGameEvent>");
        
        Settings::Init();
		Utils::UpdateModules();
		Debuffs::AddDebuffSpellToPlayer();

        return Result::kContinue;
	}

	void EventHandler::Register() {
		SKSE::GetCrosshairRefEventSource()->AddEventSink<SKSE::CrosshairRefEvent>(GetSingleton());
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESContainerChangedEvent>(GetSingleton());
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESEquipEvent>(GetSingleton());
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESMagicEffectApplyEvent>(GetSingleton());
		RE::SkillIncrease::GetEventSource()->AddEventSink<RE::SkillIncrease::Event>(GetSingleton());
		RE::LevelIncrease::GetEventSource()->AddEventSink<RE::LevelIncrease::Event>(GetSingleton());
		RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(GetSingleton());
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESLoadGameEvent>(GetSingleton());
	}

    void SinkEventHandlers() {
		EventHandler::Register();
        logger::info("Registered all event handlers.");
    }
}
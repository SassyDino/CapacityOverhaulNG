#include "Events.h"
#include "WeightHandler.h"
#include "CapacityHandler.h"
#include "BuffsDebuffs.h"
#include "Player.h"
#include "ExtraStorage.h"
#include "FormHandler.h"
#include "Hooks.h"

namespace Events
{
    auto CapacityEventHandler::ProcessEvent(const RE::TESContainerChangedEvent*event, RE::BSTEventSource<RE::TESContainerChangedEvent>*) -> Result {
        bool playerEvent = (event->oldContainer == 0x14 || event->newContainer == 0x14);
        if (!playerEvent) { return Result::kContinue; }

		auto item = RE::TESForm::LookupByID(event->baseObj)->GetName();
		std::string from{}, to{};
		if (event->oldContainer) from = RE::TESForm::LookupByID<RE::TESObjectREFR>(event->oldContainer)->GetName();
		if (event->newContainer) to = RE::TESForm::LookupByID<RE::TESObjectREFR>(event->newContainer)->GetName();

		auto refHandle = event->reference;

		// Consuming or dropping items triggers ContainerChangedEvents with a newContainer of 0x0, but dropping items provides a refHandle we can use to filter out consumption triggers
		if ((event->newContainer > 0x0) || refHandle) {
			logger::trace("<CapacityEventHandler::ContainerChanged> -> {}x '{}' from '{}' <0x{:X}> ---> '{}' <0x{:X}> | uID = {}", 
				event->itemCount, item, from, event->oldContainer, to, event->newContainer, event->uniqueID);

			CapacityHandler::AdjustSingleCategory(event);

			//TODO: Create WeightEventHandler function for ContainerChangedEvent, and move CheckWeight() into it
			SKSE::GetTaskInterface()->AddTask([]() {
				Debuffs::CapacityEffects();
				Debuffs::CheckWeight(); 
			});
		}
        
        return Result::kContinue;
    }

    auto CapacityEventHandler::ProcessEvent(const RE::TESEquipEvent*event, RE::BSTEventSource<RE::TESEquipEvent>*) -> Result {
		auto equipActor = event->actor.get();
		auto item = RE::TESForm::LookupByID(event->baseObject);

		bool playerEvent = equipActor->GetFormID() == 0x14;
		bool validType = item->Is(RE::FormType::Armor) || item->Is(RE::FormType::Weapon) || item->Is(RE::FormType::AlchemyItem) || item->Is(RE::FormType::Ingredient);
		if (!(playerEvent && validType)) { return Result::kContinue; }

		logger::trace("<CapacityEventHandler::Equip> -> BaseObj: '{} [0x{:X}]', Equipped: '{}'", item->GetName(), item->GetFormID(), event->equipped);

		// Prevent equipped apparel from taking up storage space/capacity slots
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
		
		return Result::kContinue;
    }

	auto CapacityEventHandler::ProcessEvent(const RE::SkillIncrease::Event*event, RE::BSTEventSource<RE::SkillIncrease::Event>*) -> Result {
		logger::trace("<CapacityEventHandler::SkillIncrease> -> Skill: '{}'", event->actorValue);

		CapacityHandler::CalculateActualCapacities();
		CapacityHandler::LogAllCategories();
		Debuffs::CapacityEffects();

		return Result::kContinue;
	}

	auto CapacityEventHandler::ProcessEvent(const RE::TESMagicEffectApplyEvent*event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>*) -> Result {
		auto playerEvent = (event->caster->GetFormID() == 0x14) || (event->target->GetFormID() == 0x14);
		if (!playerEvent) { return Result::kContinue; }

		auto MGEFID = event->magicEffect;
		if (Forms::IsCONGForm(MGEFID)) {
			//logger::trace("<CapacityEventHandler::MagicEffect> CONG Effect Applied");
			return Result::kContinue;
		}
		
		auto MGEF = RE::TESForm::LookupByID<RE::EffectSetting>(MGEFID);
		auto MGEFAV = MGEF->data.primaryAV;
		bool isValid = (MGEFAV == RE::ActorValue::kAlchemy) || (MGEFAV == RE::ActorValue::kArchery) || (MGEFAV == RE::ActorValue::kSpeech) || (MGEFAV == RE::ActorValue::kLockpicking) || (MGEFAV == RE::ActorValue::kPickpocket);
		
		if (MGEF && isValid) {
			logger::trace("<CapacityEventHandler::MagicEffect> -> Caster: '{}', Target: '{}', MGEF: '{} 0x[{:X}]'", 
				event->caster->GetName(), event->target->GetName(), RE::TESForm::LookupByID(event->magicEffect)->GetName(), event->magicEffect);

			SKSE::GetTaskInterface()->AddTask([]() {
				CapacityHandler::CalculateActualCapacities();
				CapacityHandler::LogAllCategories();
				Debuffs::CapacityEffects();
			});
		}

		return Result::kContinue;
	}

    auto WeightEventHandler::ProcessEvent(const RE::MenuOpenCloseEvent*event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) -> Result {
		//NOTE: If I get some sort of Magic Effect event working, I might not even need Menu events
		//? If not though, I could consider: InventoryMenu, MagicMenu, ContainerMenu, StatsMenu
        bool relevantMenuEvent = ((event->menuName == "InventoryMenu") || (event->menuName == "StatsMenu"));
        if (!relevantMenuEvent) {return Result::kContinue;}

		//TODO: This is effectively disabling this event, for testing. If I decide to keep this event, remove this check.
        if (relevantMenuEvent) {return Result::kContinue;}
		
		auto menuType = event->menuName.c_str();
        std::string str(menuType);

		logger::trace("<WeightEventHandler::MenuOpenClose> -> Menu name: '{}', Opening: '{}'", menuType, event->opening);
		WeightHandler::UpdateWeightLimit();

        return Result::kContinue;
    }

    auto WeightEventHandler::ProcessEvent(const RE::TESEquipEvent*event, RE::BSTEventSource<RE::TESEquipEvent>*) -> Result {
        auto equipActor = event->actor.get();
		auto item = RE::TESForm::LookupByID(event->baseObject);

		bool playerEvent = equipActor->GetFormID() == 0x14;
		bool invalidItem = item->Is(RE::FormType::Spell) || item->Is(RE::FormType::AlchemyItem) || item->Is(RE::FormType::Ingredient) || item->Is(RE::FormType::Book) || item->Is(RE::FormType::Ammo);
        
        if ((playerEvent) && !(invalidItem)) {
			std::string actorName = equipActor->GetName();
			std::string itemName = item->GetName();

            logger::trace("<WeightEventHandler::Equip> -> Actor: '{}', BaseObj: '{} [0x{:X}]', Equipped: '{}', oRef: '0x{:X}', uID: '{}'", 
                actorName, itemName, event->baseObject, event->equipped, event->originalRefr, event->uniqueID);
			
			SKSE::GetTaskInterface()->AddTask([]() {
				WeightHandler::UpdateWeightLimit();
			});
			
			Utils::LogStaminaAVs();
        }

        return Result::kContinue;
    }

	auto WeightEventHandler::ProcessEvent(const RE::LevelIncrease::Event*event, RE::BSTEventSource<RE::LevelIncrease::Event>*) -> Result {
		logger::trace("<WeightEventHandler::LevelIncrease> -> Level: {}", event->newLevel);

		WeightHandler::UpdateWeightLimit();

		return Result::kContinue;
	}

	auto WeightEventHandler::ProcessEvent(const RE::TESMagicEffectApplyEvent*event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>*) -> Result {
		auto playerEvent = (event->caster->GetFormID() == 0x14) || (event->target->GetFormID() == 0x14);
		if (!playerEvent) { return Result::kContinue; }

		auto MGEFID = event->magicEffect;
		if (Forms::IsCONGForm(MGEFID)) {
			//logger::trace("<WeightEventHandler::MagicEffect> CONG Effect Applied");
			return Result::kContinue;
		}

		auto MGEF = RE::TESForm::LookupByID<RE::EffectSetting>(MGEFID);
		bool isConc = MGEF->data.castingType == RE::MagicSystem::CastingType::kConcentration;
		bool isGenericDamage = MGEFID == 0xEA075;
		
		if (MGEF && !isConc && !isGenericDamage) {
			logger::trace("<WeightEventHandler::MagicEffect> -> Caster: '{}', Target: '{}', MGEF: '{} 0x[{:X}]'", 
				event->caster->GetName(), event->target->GetName(), RE::TESForm::LookupByID(event->magicEffect)->GetName(), event->magicEffect);

			SKSE::GetTaskInterface()->AddTask([]() {
				WeightHandler::UpdateWeightLimit();
			});
		}

		return Result::kContinue;
	}

    auto UIEventHandler::ProcessEvent(const RE::TESLoadGameEvent*, RE::BSTEventSource<RE::TESLoadGameEvent>*) -> Result {
        logger::debug("<UIEventHandler::LoadGame>");
        
        Settings::Init();
		Debuffs::AddDebuffSpellToPlayer();

        return Result::kContinue;
    }
/* 
	auto EventHandler::ProcessEvent(const RE::UserEventEnabled*event, RE::BSTEventSource<RE::UserEventEnabled>*) -> Result {
		//logger::debug("<EventHandler::UserEventEnabled> -> Old Event Flag: '{:X}, {:X}' | New Event Flag: '{:X}, {:X}'", (uint32_t)event->oldUserEventFlag.get(), event->oldUserEventFlag.underlying(), (uint32_t)event->newUserEventFlag.get(), event->newUserEventFlag.underlying());

		return Result::kContinue;
	}
 */
	auto EventHandler::ProcessEvent(const SKSE::CrosshairRefEvent*event, RE::BSTEventSource<SKSE::CrosshairRefEvent>*) -> Result {
		Hooks::crosshair_ref = event->crosshairRef;

		return Result::kContinue;
	}
	
    void CapacityEventHandler::Register() {
        RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESContainerChangedEvent>(GetSingleton());
        RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESEquipEvent>(GetSingleton());
		RE::SkillIncrease::GetEventSource()->AddEventSink<RE::SkillIncrease::Event>(GetSingleton());
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESMagicEffectApplyEvent>(GetSingleton());
    }

    void WeightEventHandler::Register() {
        RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(GetSingleton());
        RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESEquipEvent>(GetSingleton());
		RE::LevelIncrease::GetEventSource()->AddEventSink<RE::LevelIncrease::Event>(GetSingleton());
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESMagicEffectApplyEvent>(GetSingleton());
    }
	
    void UIEventHandler::Register() {
		RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESLoadGameEvent>(GetSingleton());
	}

	void EventHandler::Register() {
		//RE::ControlMap::GetSingleton()->AddEventSink<RE::UserEventEnabled>(GetSingleton());
		SKSE::GetCrosshairRefEventSource()->AddEventSink<SKSE::CrosshairRefEvent>(GetSingleton());
	}

    void SinkEventHandlers() {
        CapacityEventHandler::Register();
        WeightEventHandler::Register();
        UIEventHandler::Register();
		EventHandler::Register();
        logger::info("Registered all event handlers.");
    }
}
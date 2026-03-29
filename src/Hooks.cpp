#include "Hooks.h"
#include "BuffsDebuffs.h"
#include "Player.h"
#include "SKSEMCP/SKSEMenuFramework.hpp"

namespace Hooks
{
	// Credits for the InputEvent hook and the conditional input blocking go to RavenKZP: https://github.com/RavenKZP
	// More specifically, this code is very similar to the logic for their mod "No Loot While Armed": https://github.com/RavenKZP/No-Loot-While-Armed
	void Input::thunk(RE::BSTEventSource<RE::InputEvent*>* a_source, RE::InputEvent* const* a_event)
	{
		if (!Settings::Get<bool>("bModEnabled") || !a_source || !a_event) {
			return func(a_source, a_event);
		}

		auto first = *a_event;
		auto last = *a_event;
		size_t length = 0;

		for (auto current = *a_event; current; current = current->next) {
			if (Input::ProcessInput(current)) {
				if (current != last) {
					last->next = current->next;
				} else {
					last = current->next;
					first = current->next;
				}
			} else {
				last = current;
				++length;
			}
		}

		if (length == 0) {
			constexpr RE::InputEvent* const dummy[] = {nullptr};
			func(a_source, dummy);
		} else {
			RE::InputEvent* const e[] = {first};
			func(a_source, e);
		}
	}

	bool Input::ProcessInput(RE::InputEvent* event)
	{
		bool block = false;

		if (const auto b = event->AsButtonEvent()) {
			const auto u = b->GetUserEvent();
			auto s = RE::UserEvents::GetSingleton();

			if ((u == s->readyWeapon) || (u == s->rightAttack) || (u == s->leftAttack)) {
				if (b->IsDown()) {
					if (!Debuffs::CanDrawWeapon() && !SKSEMenuFramework::IsAnyBlockingWindowOpened()) {
						logger::trace("HOOKS::INPUT -> Ready Weapon |or| Left/Right Attack");

						RE::DebugNotification("You cannot hold a weapon while your storage is overflowing!");
						block = true;
					}
				}
			} else if (u == s->activate) {
				if (b->IsDown()) {
					if (!Debuffs::CanPickup() && !PlayerStatus::AllowActivation()) {
						logger::trace("HOOKS::INPUT -> Activate");
						block = true;
					}
				}
			}
		}

		return block;
	}

	RE::UI_MESSAGE_RESULTS ContainerMenuEx::ProcessMessageEx(RE::UIMessage& a_message)
	{
		//logger::debug("HOOKS::CONTAINERMENUEX || Menu = {} | Type = {}", a_message.menu.c_str(), a_message.type.underlying());

		if (a_message.type.underlying() == (uint32_t)8)
		{
			logger::trace("HOOKS::CONTAINERMENUEX -> Returning kIgnore");
			return RE::UI_MESSAGE_RESULTS::kIgnore;
		}

		return _ProcessMessageFn(this, a_message);
	}

	void Install()
	{
		auto& trampoline = SKSE::GetTrampoline();
		constexpr size_t size_per_hook = 14;
		trampoline.create(size_per_hook * 1);

		const REL::Relocation<std::uintptr_t> InputHookFunc{REL::RelocationID(67315, 68617)};
		Input::func = trampoline.write_call<5>(InputHookFunc.address() + 0x7B, Input::thunk);

		REL::Relocation<uintptr_t> vtbl(RE::VTABLE_ContainerMenu[0]);
		ContainerMenuEx::_ProcessMessageFn = vtbl.write_vfunc(0x4, &ContainerMenuEx::ProcessMessageEx);
		
		logger::info("Hooks installed!");
	}
}
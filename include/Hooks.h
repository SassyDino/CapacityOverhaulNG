#pragma once

namespace Hooks
{
	inline RE::TESObjectREFRPtr crosshair_ref;

	struct Input
	{
		static void thunk(RE::BSTEventSource<RE::InputEvent*>* a_source, RE::InputEvent* const* a_event);
		static inline REL::Relocation<decltype(thunk)> func;
		static bool ProcessInput(RE::InputEvent* event);
	};

	class ContainerMenuEx : public RE::ContainerMenu
	{
		public:
			RE::UI_MESSAGE_RESULTS ProcessMessageEx(RE::UIMessage& a_message);
		
			using ProcessMessageFn = decltype(&RE::ContainerMenu::ProcessMessage);

			inline static REL::Relocation<ProcessMessageFn> _ProcessMessageFn;
	};
	
	void Install();
}
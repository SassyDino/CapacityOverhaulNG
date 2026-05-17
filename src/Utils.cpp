#include "Utils.h"
#include "WeightHandler.h"
#include "CapacityHandler.h"
#include "Player.h"
#include "ExtraStorage.h"
#include "FormHandler.h"
#include "Hooks.h"
#include "BuffsDebuffs.h"
#include "GUIAssets.h"
#include "MCPHeatmap.h"

namespace Utils
{   
	void UpdateModules()
	{
		clib_util::Timer timer;
		timer.start();
		logger::debug("{:=^50}", "Updating Modules");

		logger::debug("{:-^50}", "Weight: Settings");
		WeightHandler::UpdateFromSettings();
		logger::debug("{:-^50}", "Weight: Update");
        WeightHandler::UpdateWeightLimit();

		logger::debug("{:-^50}", "Capacity: Ratios");
		CapacityHandler::UpdateCategoryRatios();
		logger::debug("{:-^50}", "Capacity: Settings");
		CapacityHandler::UpdateBaseCapacities();
		logger::debug("{:-^50}", "Categories: Update");
		CapacityHandler::UpdateAllCategories(false);
		logger::debug("{:-^50}", "Capacity: Actual");
		CapacityHandler::CalculateActualCapacities();
		logger::debug("{:-^50}", "Capacity: Total");
		CapacityHandler::UpdateTotalCount();
		CapacityHandler::LogAllCategories();

		logger::debug("{:-^50}", "Debuffs: Weight");
		Debuffs::CheckWeight();
		logger::debug("{:-^50}", "Debuffs: Capacity");
		Debuffs::CapacityEffects();

		timer.stop();
		logger::debug("Modules updated! Time taken: {}μs / {}ms", timer.duration_μs(), timer.duration_ms());
		logger::debug("{}", std::string(50, '='));
	}
	
	void MessageListener(SKSE::MessagingInterface::Message* message) {
		switch (message->type) {
			case SKSE::MessagingInterface::kDataLoaded:
				logger::debug("MessagingInterface::kDataLoaded");

				Settings::Init();

				Hooks::Install();

				PlayerState::Char = RE::PlayerCharacter::GetSingleton();
				PlayerState::AsAV = PlayerState::Char->AsActorValueOwner();
				PlayerState::Race = PlayerState::Char->GetRace();
				
				PlayerState::Controls = RE::PlayerControls::GetSingleton();
				PlayerState::State = PlayerState::Char->AsActorState();

				//Lang::InitTranslations();

				Forms::LoadFromGame();

				GUI::Assets::LoadTextures();
				
				GUI::MCP::heatmap.Init();
				
				CapacityHandler::Bonus::ParseAllTOMLFiles();
				
				break;
			default:
				break;
		}
	}
}
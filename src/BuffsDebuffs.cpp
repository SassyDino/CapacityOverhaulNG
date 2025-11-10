#include "BuffsDebuffs.h"
#include "CapacityHandler.h"
#include "Player.h"

void Debuffs::CapacityEffects()
{
	CapacityHandler::Player::CheckIfOverCapacity();
	if (PlayerStatus::GetOverCapacityStatus()) {
		//TODO: Debuff manager
		logger::debug("Exceeding total capacity.");
	} else {
		logger::debug("Total not exceeding capacity.");
	}
}
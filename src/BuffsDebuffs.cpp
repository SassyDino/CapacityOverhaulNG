#include "BuffsDebuffs.h"
#include "CapacityHandler.h"

void Debuffs::CapacityEffects()
{
	auto overCapacity = CapacityHandler::Player::IsOverCapacity();
	if (overCapacity) {
		//TODO: Debuff manager
		logger::debug("Exceeding total capacity.");
	} else {
		logger::debug("Total not exceeding capacity.");
	}
}
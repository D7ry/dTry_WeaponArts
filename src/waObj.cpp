#include "waObj.h"
#include "Utils.h"
void waObj::launchWeaponArt(RE::Actor* a_actor) {
	Utils::damageav(a_actor, RE::ActorValue::kStamina, _staminaCost);
	Utils::damageav(a_actor, RE::ActorValue::kMagicka, _magickaCost);

}

//TODO:add blinking effects iff stamina/magicka isn't enough?
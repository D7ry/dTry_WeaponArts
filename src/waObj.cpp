#pragma once
#include "waObj.h"
#include "Utils.h"
void waObj::launchWeaponArt(RE::Actor* a_actor) {
	Utils::damageav(a_actor, RE::ActorValue::kStamina, _staminaCost);
	Utils::damageav(a_actor, RE::ActorValue::kMagicka, _magickaCost);
	//calculate maginutude override of spell.
	float magOverride;
	switch (_magOverride) {
	case MAG_OVERRIDE::attackDamage: magOverride = a_actor->GetAttackingWeapon()->object->As<RE::TESObjectWEAP>()->GetAttackDamage() * a_actor->GetActorValue(RE::ActorValue::kAttackDamageMult);
	case MAG_OVERRIDE::identity: magOverride = 1;
	default: magOverride = 1;
	}
	a_actor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->InstantCast(_spell, false, nullptr, 1, true, magOverride, a_actor);

}

//TODO:add blinking effects iff stamina/magicka isn't enough?
#pragma once
#include "waObj.h"
#include "Utils.h"
void waObj::processWeaponArt(RE::Actor* a_actor) {
	DEBUG("Matching payload, processing {} for {}", _name, a_actor->GetName());
	if (a_actor->GetActorValue(RE::ActorValue::kStamina) < _staminaCost || a_actor->GetActorValue(RE::ActorValue::kMagicka) < _magickaCost) {
		DEBUG("failed stamina/magicka check");
		return;
	}
	//passing all the checks above initializes weapon art.
	DEBUG("all checks passed, launching weapon art {}!", _name);
	Utils::damageav(a_actor, RE::ActorValue::kStamina, _staminaCost);
	Utils::damageav(a_actor, RE::ActorValue::kMagicka, _magickaCost);
	//calculate maginutude override of spell.
	float effectiveness = 0;
	switch (_effectivenessType) {
	case EFFECTIVENESS::EattackDamage: 
		if (a_actor->GetAttackingWeapon()) {
			effectiveness = a_actor->GetAttackingWeapon()->object->As<RE::TESObjectWEAP>()->GetAttackDamage() * a_actor->GetActorValue(RE::ActorValue::kAttackDamageMult);
		}
		else {
			effectiveness = Utils::getMaximumWeaponDamage(a_actor);
		}
		break;
	case EFFECTIVENESS::Ecustom: effectiveness = _effectivenessCustomVal; break;
	default: effectiveness = 0;
	}
	float magOverride = 0;
	switch (_magOverrideType) {
	case MAG_OVERRIDE::MattackDamage: 
		if (a_actor->GetAttackingWeapon()) {
			magOverride = a_actor->GetAttackingWeapon()->object->As<RE::TESObjectWEAP>()->GetAttackDamage() * a_actor->GetActorValue(RE::ActorValue::kAttackDamageMult);
		}
		else {
			magOverride = Utils::getMaximumWeaponDamage(a_actor);
		}
		break;
	case MAG_OVERRIDE::Mcustom: magOverride = _magOverrideCustomVal; break;
	default: magOverride = 0;
	}
	DEBUG("Casting {}. Caster: {}; Spell: {}; Effectiveness: {}; Magnitude override: {}; Stamina damage: {}; Magicka damage: {}",
		_name, a_actor->GetName(), _spell->GetName(), effectiveness, magOverride, _staminaCost, _magickaCost);
	a_actor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->InstantCast(_spell, false, nullptr, effectiveness, false, magOverride, a_actor);
}

//TODO:add blinking effects iff stamina/magicka isn't enough?
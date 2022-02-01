#pragma once
#include "Utils.h"
using namespace std;
class waObj {
public:
	/*calculation method for spell manitude*/
	enum MAG_OVERRIDE {
		attackDamage = 0, //equal to the character's attack damage
		identity = 1, //equal to 1
		custom = 2, //equal to the follow value
	};

	/*construct a weaponArt object*/
	waObj(std::string name, RE::SpellItem* spell, int magOverride_enum_int, float magOverrideCustomVal, float staminaCost, float magickaCost) {
		_spell = spell;
		_magOverrideType = static_cast<MAG_OVERRIDE>(magOverride_enum_int);
		_magOverrideCustomVal = magOverrideCustomVal;
		_staminaCost = staminaCost;
		_magickaCost = magickaCost;
		_name = name;
		INFO("successfully constructed weapon art obj. Name: {}, Spell: {}, Stamina Cost: {}, Magicka Cost: {}, Magnitde Override Method: {}, Custom Override Value: {}", _name, _spell->GetName(), _staminaCost, _magickaCost, _magOverrideType, magOverrideCustomVal);
	}

	void processWeaponArt(RE::Actor* a_actor) {
		if (a_actor->GetActorValue(RE::ActorValue::kStamina) < _staminaCost || a_actor->GetActorValue(RE::ActorValue::kMagicka) < _magickaCost) {
			DEBUG("failed stamina/magicka check");
			return;
		}
		//passing all the checks above initializes weapon art.
		DEBUG("all checks passed, launching weapon art {}!", _name);
		Utils::damageav(a_actor, RE::ActorValue::kStamina, _staminaCost);
		Utils::damageav(a_actor, RE::ActorValue::kMagicka, _magickaCost);
		//calculate maginutude override of spell.
		float magOverride;
		switch (_magOverrideType) {
		case MAG_OVERRIDE::attackDamage: magOverride = a_actor->GetAttackingWeapon()->object->As<RE::TESObjectWEAP>()->GetAttackDamage() * a_actor->GetActorValue(RE::ActorValue::kAttackDamageMult);
		case MAG_OVERRIDE::identity: magOverride = 1;
		case MAG_OVERRIDE::custom: magOverride = _magOverrideCustomVal;
		default: magOverride = 1;
		}
		a_actor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->InstantCast(_spell, false, nullptr, 1, true, magOverride, a_actor);
	}

	void printInfo() {
		if (_spell) {
			INFO("spell: {}", _spell->GetName());
		}
		else {
			INFO("spell not found!");
		}
		if (_weapon) {
			INFO("weapon: {}", _weapon->GetName());
		}
		else {
			INFO("weapon not found!");
		}
	};


private:
	/*spell to cast for the weapon art*/
	RE::SpellItem* _spell;
	/*the weapon bounded to weapon art*/
	RE::TESObjectWEAP* _weapon;
	/*stamina and magicka cost for each skill*/
	float _staminaCost;
	float _magickaCost;
	/*calculation method for spell magnitude*/
	MAG_OVERRIDE _magOverrideType;
	/*custom multiplier for spell magnitude; only works when magOverrideType is custom.*/
	float _magOverrideCustomVal;
public:
	/*my name*/
	std::string _name;


};

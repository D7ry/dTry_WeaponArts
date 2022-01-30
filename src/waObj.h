#pragma once
using namespace std;
class waObj {
public:
	/*parse a string from ini and construct a weaponArt object*/
	waObj(string ) {

	}
	/*calculation method for spell manitude*/
	enum MAG_OVERRIDE {
		identity = 0, //equal to 1
		attackDamage = 1, //equal to the character's attack damage
	};

	void processWeaponArt(RE::Actor* a_actor, string anno) {
		if (anno != _anno) {
			DEBUG("failed anno check: input anno: {} not equal to expected anno:{}", anno, _anno);
			return;
		}
		auto weapon = a_actor->GetAttackingWeapon();
		if (weapon) {
			DEBUG("actor has no attacking weapon");
			return;
		}
		if (weapon->object != _weapon) {
			DEBUG("failed weapon acheck: input weapon: {} not equal to expected weapon: {}", weapon->object->GetName(), _weapon->GetName());
		}
		if (_powerOnly) {
			if (a_actor && a_actor->currentProcess && a_actor->currentProcess->high && a_actor->currentProcess->high->attackData && a_actor->currentProcess->high->attackData->data.flags.any(RE::AttackData::AttackFlag::kPowerAttack)) {
				DEBUG("failed power check: actor has to be power attacking while they are not.");
				return;
			}
		}
		if (a_actor->GetActorValue(RE::ActorValue::kStamina) < _staminaCost || a_actor->GetActorValue(RE::ActorValue::kMagicka) < _magickaCost) {
			DEBUG("failed stamina/magicka check");
			return;
		}
		//passing all the checks above initializes weapon art.
		launchWeaponArt(a_actor);
	}

private:
	inline void launchWeaponArt(RE::Actor* a_actor);
	/*spell to cast for the weapon art*/
	RE::SpellItem* _spell;
	/*the weapon bounded to weapon art*/
	RE::TESObjectWEAP* _weapon;
	/*should the weapon art trigger during power attack or for all attacks?*/
	bool _powerOnly;
	/*should the weapon art trigger for player only?*/ //does not work for now, a place holder only
	bool _pcOnly;
	/*stamina and magicka cost for each skill*/
	float _staminaCost;
	float _magickaCost;
	/*annotation triggering the weapon art*/
	string _anno;
	/*calculation method for spell magnitude*/
	MAG_OVERRIDE _magOverride;
};
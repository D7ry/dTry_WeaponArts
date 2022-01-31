#pragma once
using namespace std;
class waObj {
public:
	/*calculation method for spell manitude*/
	enum MAG_OVERRIDE {
		identity = 0, //equal to 1
		attackDamage = 1, //equal to the character's attack damage
	};

	/*construct a weaponArt object*/
	waObj(RE::TESObjectWEAP* weapon, RE::SpellItem* spell, int magOverride_int, float staminaCost, float magickaCost, bool powerOnly, bool pcOnly) {
		_weapon = weapon;
		_spell = spell;
		_magOverride = static_cast<MAG_OVERRIDE>(magOverride_int);
		_staminaCost = staminaCost;
		_magickaCost = magickaCost;
		_powerOnly = powerOnly;
		_pcOnly = pcOnly;
		//_debugMsg = "poggers";
		DEBUG("successfully constructed weapon art obj! weapon: {}, spell: {}", weapon->GetName(), spell->GetName());
	}

	void processWeaponArt(RE::Actor* a_actor) {
		if (!a_actor->GetAttackingWeapon()) {
			DEBUG("actor has no attacking weapon");
			return;
		}
		auto weapon = a_actor->GetAttackingWeapon()->object;
		if (weapon) {
			DEBUG("weapon object doesn't exist");
			return;
		}
		if (weapon->GetFormID() != _weapon->GetFormID()) {
			DEBUG("failed weapon acheck: input weapon: {} not equal to expected weapon: {}", weapon->GetName(), _weapon->GetName());
			DEBUG("input weapon formid: {}, expected weapon formid: {}", weapon->formID, _weapon->formID);
			return;
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
		DEBUG("all checks passed, launching weapon art!");
		launchWeaponArt(a_actor, weapon->As<RE::TESObjectWEAP>());
		DEBUG("weapon art launched!");
	}

	void printInfo() {
		INFO("printing waObj info!");
		INFO("debug message: {}", _debugMsg);
		if (_spell) {
			INFO("spell is {}", _spell->GetName());
		}
		else {
			INFO("spell not found!");
		}
		if (_weapon) {
			INFO("weapon is {}", _weapon->GetName());
		}
		else {
			INFO("weapon not found!");
		}
		//INFO("a weapon art casting spell {}, for weapon {}, bounded to annotation {}", _spell->GetName(), _weapon->GetName(), _anno);
	};


private:
	void launchWeaponArt(RE::Actor* a_actor, RE::TESObjectWEAP* a_weapon);
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
	/*calculation method for spell magnitude*/
	MAG_OVERRIDE _magOverride;
public:
	std::string _debugMsg;


};

#pragma once
using namespace std;
class waObj {
public:
	/*calculation method for spell manitude*/
	enum MAG_OVERRIDE {
		attackDamage = 0, //equal to the character's attack damage
		identity = 1, //equal to 1
	};

	/*construct a weaponArt object*/
	waObj(std::string name, RE::TESObjectWEAP* weapon, RE::SpellItem* spell, int magOverride_int, float staminaCost, float magickaCost, bool powerOnly, bool pcOnly) {
		_weapon = weapon;
		_spell = spell;
		_magOverride = static_cast<MAG_OVERRIDE>(magOverride_int);
		_staminaCost = staminaCost;
		_magickaCost = magickaCost;
		_powerOnly = powerOnly;
		_pcOnly = pcOnly;
		_name = name;
		DEBUG("successfully constructed weapon art obj! weapon: {}, spell: {}", weapon->GetName(), spell->GetName());
	}

	void processWeaponArt(RE::Actor* a_actor) {
		if (!a_actor->GetAttackingWeapon()) {
			DEBUG("actor has no attacking weapon");
			return;
		}
		auto weapon = a_actor->GetAttackingWeapon()->object;
		if (!weapon) {
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
		DEBUG("all checks passed, launching weapon art {}!", _name);
		launchWeaponArt(a_actor, weapon->As<RE::TESObjectWEAP>());
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
	/*my name*/
	std::string _name;


};

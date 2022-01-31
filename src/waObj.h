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
	waObj(std::string name, RE::SpellItem* spell, int magOverride_int, float staminaCost, float magickaCost) {
		_spell = spell;
		_magOverride = static_cast<MAG_OVERRIDE>(magOverride_int);
		_staminaCost = staminaCost;
		_magickaCost = magickaCost;
		_name = name;
		INFO("successfully constructed weapon art obj for spell: {}", spell->GetName());
	}

	void processWeaponArt(RE::Actor* a_actor) {
		if (a_actor->GetActorValue(RE::ActorValue::kStamina) < _staminaCost || a_actor->GetActorValue(RE::ActorValue::kMagicka) < _magickaCost) {
			DEBUG("failed stamina/magicka check");
			return;
		}
		//passing all the checks above initializes weapon art.
		DEBUG("all checks passed, launching weapon art {}!", _name);
		launchWeaponArt(a_actor);
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
	void launchWeaponArt(RE::Actor* a_actor);
	/*spell to cast for the weapon art*/
	RE::SpellItem* _spell;
	/*the weapon bounded to weapon art*/
	RE::TESObjectWEAP* _weapon;
	/*stamina and magicka cost for each skill*/
	float _staminaCost;
	float _magickaCost;
	/*calculation method for spell magnitude*/
	MAG_OVERRIDE _magOverride;
public:
	/*my name*/
	std::string _name;


};

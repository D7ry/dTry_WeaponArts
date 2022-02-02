#pragma once
using namespace std;
class waObj {
public:
	/*calculation method for spell manitude*/
	enum MAG_OVERRIDE {
		Mcustom = 0,
		MattackDamage = 1
	};

	enum EFFECTIVENESS {
		Ecustom = 0,
		EattackDamage = 1
	};

	/*construct a weaponArt object*/
	waObj(std::string name, RE::SpellItem* spell, 
		int effectiveness_enum_int, float effectivenessCustomVal, 
		int magOverride_enum_int, float magOverrideCustomVal, 
		float staminaCost, float magickaCost) {
		_spell = spell;
		_effectivenessType = static_cast<EFFECTIVENESS>(effectiveness_enum_int);
		_effectivenessCustomVal = effectivenessCustomVal;
		_magOverrideType = static_cast<MAG_OVERRIDE>(magOverride_enum_int);
		_magOverrideCustomVal = magOverrideCustomVal;
		_staminaCost = staminaCost;
		_magickaCost = magickaCost;
		_name = name;
		INFO("successfully constructed weapon art obj. Name: {}, Spell: {}, Stamina Cost: {}, Magicka Cost: {}, Effectiveness Method: {}, Custom effectiveness value: {}, Magnitude Override Method: {}, Custom Override Value: {}", 
			_name, _spell->GetName(), _staminaCost, _magickaCost, _effectivenessType, _effectivenessCustomVal,_magOverrideType, magOverrideCustomVal);
	}

	void processWeaponArt(RE::Actor* a_actor);

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
	/*calculation method for spell effectiveness*/
	EFFECTIVENESS _effectivenessType;
	/*custom value for  spell effectiveness; only works when effectivenessType is custom. */
	float _effectivenessCustomVal;
	/*calculation method for spell magnitude*/
	MAG_OVERRIDE _magOverrideType;
	/*custom value for spell magnitude; only works when magOverrideType is custom.*/
	float _magOverrideCustomVal;
public:
	/*my name*/
	std::string _name;


};

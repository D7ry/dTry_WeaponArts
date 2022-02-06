#pragma once
namespace Utils
{
    /*returns maximum weapon damage of actor a. If a is not holding a wepaon,
    return their unarmed damage.*/
    inline float getMaximumWeaponDamage(RE::Actor* a) {
        if (a) {
            auto weaponR = a->GetEquippedObject(false);
            if (weaponR && weaponR->GetFormType() == RE::FormType::Weapon) {
                return weaponR->As<RE::TESObjectWEAP>()->GetAttackDamage();
            }
            auto weaponL = a->GetEquippedObject(true);
            if (weaponL && weaponL->GetFormType() == RE::FormType::Weapon) {
                return weaponL->As<RE::TESObjectWEAP>()->GetAttackDamage();
            }
            return a->GetActorValue(RE::ActorValue::kUnarmedDamage);
        }
        return 0;
    }

	inline void damageav(RE::Actor* a, RE::ActorValue av, float val)
	{
		if (a) {
			a->As<RE::ActorValueOwner>()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, av, -val);
		}
		DEBUG("{}'s {} damaged to {}",
			a->GetName(),
			av,
			a->GetActorValue(av));
	}

	inline bool ToInt(std::string str, int& value)
	{
		const char* strVal = str.c_str();
		char* endVal = NULL;
		long ret = strtol(strVal, &endVal, 0);
		if (ret == LONG_MAX || ret == LONG_MIN || (int)endVal != (int)strVal + strlen(strVal))
			return false;
		value = ret;
		return true;
	}
    using namespace std;
    inline vector<string> parseStr(const string& delimiter, const string& str)
    {
        vector<string> arr;

        int strleng = str.length();
        int delleng = delimiter.length();
        if (delleng == 0)
            return arr;//no change

        int i = 0;
        int k = 0;
        while (i < strleng)
        {
            int j = 0;
            while (i + j < strleng && j < delleng && str[i + j] == delimiter[j])
                j++;
            if (j == delleng)//found delimiter
            {
                arr.push_back(str.substr(k, i - k));
                i += delleng;
                k = i;
            }
            else
            {
                i++;
            }
        }
        arr.push_back(str.substr(k, i - k));
        return arr;
    }


}
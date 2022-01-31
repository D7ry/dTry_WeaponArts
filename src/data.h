#pragma once
#include "waObj.h"
#include "SimpleIni.h"
#include "events.h"
#include "Utils.h"
#include <iostream>
#include <sstream>

struct weaponArts {
public:

	/*a set of all weapon arts*/
	inline static std::unordered_map<std::string, std::vector<waObj>> weaponArtsMap;
};

class dataHandler {
public:

	/*read all weapon arts mappings from .ini file.*/
	static void readIni() {
		INFO("loading weapon art ini...");
		CSimpleIniA ini;
		#define SETTINGFILE_PATH "Data\\SKSE\\Plugins\\dTry_WeaponArts.ini"
		ini.LoadFile(SETTINGFILE_PATH);
		CSimpleIniA::TNamesDepend keys;
		ini.GetAllKeys("WeaponArts", keys);
		for (CSimpleIniA::TNamesDepend::iterator s_it1 = keys.begin(); s_it1 != keys.end(); s_it1++) {
			const char* wa_Name = s_it1->pItem;
			std::string wa_strToParse = ini.GetValue("WeaponArts", wa_Name);
			readLine(wa_strToParse, wa_Name);
		}
	}


	static void testInsertWeaponArt() {
		auto a_weapon = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESObjectWEAP>(0x00012EB7, "Skyrim.esm"); //iron sword
		auto a_spell = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(0x0001C789, "Skyrim.esm");
		auto a_waObj = waObj("fireBoom", a_weapon, a_spell, 1, 10, 10, false, false);
		safeInsert("HitFrame", a_waObj);
	}

private:
	/*read a line from ini, parse it and see if it's valid.
	if it's valid, insert the weapon art into the hash map.
	a sample line: hitframe|0x0001C789|Skyrim.esm|0x00012EB7|Skyrim.esm|30|20|1|0|0
	@param a_name the name of weapon art, as the key in the ini that's being passed in*/
	static inline void readLine(std::string a_line, std::string a_name) {
		INFO("reading: {} for weapon art: {}", a_line, a_name);
		string anno;
		RE::TESObjectWEAP* weapon;
		RE::SpellItem* spell;
		int magOverride_Int;
		float staminaCost;
		float magickCost;
		bool powerOnly;
		bool pcOnly;
		string arr[10];
		int i = 0;
		stringstream ssin(a_line);
		while (ssin.good() && i < 10) {
			ssin >> arr[i];
			++i;
		}
		int spellForm = 0;
		if (Utils::ToInt(arr[1], spellForm))
		{
			if (spellForm == 0) {
				return;
			}
		}
		int weaponForm = 0;
		if (Utils::ToInt(arr[3], weaponForm))
		{
			if (weaponForm == 0) {
				return;
			}
		}
		anno = arr[0];
		spell = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(spellForm, arr[2]);
		weapon = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESObjectWEAP>(weaponForm, arr[4]);
		staminaCost = std::stof(arr[5]);
		magickCost = std::stof(arr[6]);
		magOverride_Int = std::stoi(arr[7]);
		powerOnly = std::stoi(arr[8]);
		pcOnly = std::stoi(arr[9]);
		if (!weapon) {
			INFO("Error: Failed to read weapon; check your ini.");
			return;
		}
		if (!spell) {
			INFO("Error: Failed to read spell; check your ini.");
			return;
		}
		waObj a_waObj = waObj(a_name, weapon, spell, magOverride_Int, staminaCost, magickCost, powerOnly, pcOnly);
		safeInsert(anno, a_waObj);
	}
	static void safeInsert(std::string a_anno, waObj a_waObj) {
		DEBUG("registering weapon art {}", a_waObj._name);
		auto it = weaponArts::weaponArtsMap.find(a_anno);
		if (it != weaponArts::weaponArtsMap.end()) {
			std::vector<waObj>& arr = it->second;
			arr.push_back(a_waObj);
		}
		else {
			weaponArts::weaponArtsMap.insert(
				std::pair<std::string, std::vector<waObj>>(std::string(a_anno), std::vector<waObj> {a_waObj})
			
			);
		}
		DEBUG("registeration successful");
	};
};
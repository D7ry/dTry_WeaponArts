#pragma once
#include <unordered_set>
#include "waObj.h"
#include "SimpleIni.h"
#include "events.h"

struct weaponArts {
public:

	/*a set of all weapon arts*/
	inline static std::unordered_map<std::string, std::vector<waObj>> weaponArtsMap;
};

class dataHandler {
public:

	/*read all weapon arts mappings from .ini file.*/
	static void readIni() {
		INFO("reading weapon art!");
		CSimpleIniA ini;
		#define SETTINGFILE_PATH "Data\\MCM\\Settings\\StopOnSlash.ini"
		ini.LoadFile(SETTINGFILE_PATH);
	}
	static void testInsertWeaponArt() {
		auto a_weapon = RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESObjectWEAP>(0x00012EB7, "Skyrim.esm"); //iron sword
		auto a_spell = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(0x0001C789, "Skyrim.esm");
		auto a_waObj = waObj("Hitframe", a_weapon, a_spell, 1, 10, 10, false, false);
		safeInsert("HitFrame", a_waObj);
		DEBUG("weapon art inserted");
	}

private:
	static void safeInsert(std::string a_anno, waObj a_waObj) {
		DEBUG("inserting weapon art!");
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
		DEBUG("insertion complete! anno: {}", std::string(a_anno));
	};
};
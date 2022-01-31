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
	static void readIniS() {
		DEBUG("Loading settings directory");
		std::string path = "Data/SKSE/WeaponArts_Config";
		for (const auto& entry : std::filesystem::directory_iterator(path)) { //iterates through all .ini files
			string pathStr = entry.path().string();
			INFO("Reading from {}", pathStr);
			const char* cstr = pathStr.c_str();
			readSingleIni(cstr);
		}

	}


private:
	/*read a single .ini file*/
	static inline void readSingleIni(const char* ini_path) {
		CSimpleIniA ini;
		ini.LoadFile(ini_path);
		CSimpleIniA::TNamesDepend keys;
		ini.GetAllKeys("WeaponArts", keys);
		for (CSimpleIniA::TNamesDepend::iterator s_it1 = keys.begin(); s_it1 != keys.end(); s_it1++) {
			const char* wa_Name = s_it1->pItem;
			std::string wa_strToParse = ini.GetValue("WeaponArts", wa_Name);
			readLine(wa_strToParse, wa_Name);
		}
	}
	/*read a line from ini, parse it and see if it's valid.
	if it's valid, insert the weapon art into the hash map.
	a sample line: hitframe|0x0001C789|Skyrim.esm|0x00012EB7|Skyrim.esm|30|20|1|0|0
	@param a_name the name of weapon art, as the key in the ini that's being passed in*/
	static inline void readLine(std::string a_line, std::string a_name) {
		INFO("Found weapon art {}, config line: {}", a_line, a_name);
		string anno;
		RE::SpellItem* spell;
		int magOverride_Int;
		float staminaCost;
		float magickCost;
		int spellForm = 0;
		vector<string> arr = Utils::parseStr("|", a_line);
		anno = arr[0];
		if (Utils::ToInt(arr[1], spellForm))
		{
			if (spellForm == 0) {
				return;
			}
		}
		spell = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(spellForm, arr[2]);
		staminaCost = std::stof(arr[3]);
		magickCost = std::stof(arr[4]);
		magOverride_Int = std::stoi(arr[5]);
		if (!spell) {
			INFO("Error: Failed to read spell; check your ini.");
			return;
		}
		waObj a_waObj = waObj(a_name, spell, magOverride_Int, staminaCost, magickCost);
		safeInsert(anno, a_waObj);
	}
	static void safeInsert(std::string a_anno, waObj a_waObj) {
		INFO("registering weapon art {}", a_waObj._name);
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
		INFO("registeration successful");
	};
};
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

	inline static const string weaponArtsDir = "Data/SKSE/WeaponArts_Config";
};

class dataHandler {
public:

	/*read all weapon arts mappings from .ini file.*/
	static void readIniS();

private:
	/*read a single .ini file*/
	static inline void readSingleIni(const char* ini_path);


	static inline bool readLine(std::string a_line, std::string a_name);

	/*insert weapon art into the hashmap*/
	static void safeInsert(std::string a_anno, waObj a_waObj) {
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
	};
};
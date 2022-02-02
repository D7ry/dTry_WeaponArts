#include "data.h"
#include <regex>
/*iterate through all .ini files in the directory.*/
void dataHandler::readIniS() {
	INFO("loading config from: " + weaponArts::weaponArtsDir);
	for (const auto& entry : std::filesystem::directory_iterator(weaponArts::weaponArtsDir)) { //iterates through all .ini files
		string pathStr = entry.path().string();
		INFO("Reading from {}", pathStr);
		const char* cstr = pathStr.c_str();
		readSingleIni(cstr);
	}
}

void dataHandler::readSingleIni(const char* ini_path) {
	int weaponArtCt = 0;
	CSimpleIniA ini;
	ini.LoadFile(ini_path);
	CSimpleIniA::TNamesDepend keys;
	ini.GetAllKeys("WeaponArts", keys);
	for (CSimpleIniA::TNamesDepend::iterator s_it1 = keys.begin(); s_it1 != keys.end(); s_it1++) {
		const char* wa_Name = s_it1->pItem;
		std::string wa_strToParse = ini.GetValue("WeaponArts", wa_Name);
		if (readLine(wa_strToParse, wa_Name)) {
			weaponArtCt++;
		}
	}
	INFO("Successfully loaded {} weapon art.", weaponArtCt);
}

/*read a line from ini, parse it and see if it's valid.
if it's valid, insert the weapon art into the hash map.
a sample line: hitframe|0x00012EB7|Skyrim.esm|30|20|1|
@param a_name the name of weapon art, as the key in the ini that's being passed in*/
bool dataHandler::readLine(std::string a_line, std::string a_name) {
	INFO("Found weapon art {}, config line: {}", a_name, a_line);
	if (!std::regex_match(a_line, std::regex("((\\w|\\.|\\s)+\\|){3}((\\d|\\.)+\\|){5}(\\d)+"))) {
		INFO("Error: wrong config line format; failed to read this weapon art.");
		return false;
	}
	string anno;
	RE::SpellItem* spell;
	int effectiveness_enum_int;
	float effectiveness_CustomVal;
	int magOverride_enum_int;
	float magOverride_CustomVal;
	float staminaCost;
	float magickCost;
	int spellForm = 0;
	vector<string> arr = Utils::parseStr("|", a_line);
	anno = arr[0];
	if (Utils::ToInt(arr[1], spellForm))
	{
		if (spellForm == 0) {
			INFO("Error: wrong spell formID, failed to read this weapon art.");
			return false;
		}
	}
	spell = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(spellForm, arr[2]);
	staminaCost = std::stof(arr[3]);
	magickCost = std::stof(arr[4]);
	effectiveness_enum_int = std::stoi(arr[5]);
	effectiveness_CustomVal = std::stof(arr[6]);
	magOverride_enum_int = std::stoi(arr[7]);
	magOverride_CustomVal = std::stof(arr[8]);
	if (!spell) {
		INFO("Error: Incorrect spell formID or incorrect plugin name. Failed to read spell from plugin ", arr[2]);
		return false;
	}
	waObj a_waObj = waObj(a_name, spell, effectiveness_enum_int, effectiveness_CustomVal, magOverride_enum_int, magOverride_CustomVal, staminaCost, magickCost);
	safeInsert(anno, a_waObj);
	return true;
}
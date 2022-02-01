#include "data.h"
/*iterate through all .ini files in the directory.*/
void dataHandler::readIniS() {
	INFO("Loading settings from directory: " + weaponArts::weaponArtsDir);
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
		readLine(wa_strToParse, wa_Name);
		weaponArtCt++;
	}
	INFO("Loaded {} weapon art.", weaponArtCt);
}

/*read a line from ini, parse it and see if it's valid.
if it's valid, insert the weapon art into the hash map.
a sample line: hitframe|0x00012EB7|Skyrim.esm|30|20|1|
@param a_name the name of weapon art, as the key in the ini that's being passed in*/
void dataHandler::readLine(std::string a_line, std::string a_name) {
	INFO("Found weapon art {}, config line: {}", a_name, a_line);
	string anno;
	RE::SpellItem* spell;
	int magOverride_enum_int;
	float magOverride_CustomVal;
	float staminaCost;
	float magickCost;
	int spellForm = 0;
	DEBUG("parsing...");
	vector<string> arr = Utils::parseStr("|", a_line);
	DEBUG("finished parsing");
	anno = arr[0];
	DEBUG("1");
	if (Utils::ToInt(arr[1], spellForm))
	{
		if (spellForm == 0) {
			return;
		}
	}
	DEBUG("2");
	spell = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(spellForm, arr[2]);
	DEBUG("3");
	staminaCost = std::stof(arr[3]);
	DEBUG("4");
	magickCost = std::stof(arr[4]);
	DEBUG("5");
	magOverride_enum_int = std::stoi(arr[5]);
	DEBUG("6");
	magOverride_CustomVal = std::stof(arr[6]);
	DEBUG("7");
	DEBUG("finished assigning");
	if (!spell) {
		INFO("Error: Failed to read spell; check your ini.");
		return;
	}
	waObj a_waObj = waObj(a_name, spell, magOverride_enum_int, magOverride_CustomVal, staminaCost, magickCost);
	safeInsert(anno, a_waObj);
}
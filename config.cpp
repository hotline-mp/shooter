#include <iostream>
#include "Game.hpp"

using namespace libconfig;

const char* cfg_filename = "shooter.cfg";

int Game::getConfig() {
	// default config
	keys["up"] = sf::Keyboard::W;
	keys["down"] = sf::Keyboard::S;
	keys["left"] = sf::Keyboard::A;
	keys["right"] = sf::Keyboard::D;
	keys["reload"] = sf::Keyboard::R;
    sfx_volume = 20;
    music_volume = 30;

	try
	{
		cfg.readFile(cfg_filename);
	}
	catch(const FileIOException &fioex)
	{
		/* el fitxer no existia, probablement per que es la primera
		 * vegada que es juga, ignorar */
		return 1;
	}
	catch(const ParseException &pex)
	{
		// TODO: error grafic
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
			<< " - " << pex.getError() << std::endl;
		return 1;
	}

	std::vector<std::string> to_read {"up", "down", "left", "right", "reload"};
	for (std::string key : to_read) {
		try {
			keys[key] = cfg.lookup(key);
		} catch(const SettingNotFoundException &nfex) {
			/* si no esta a la config no pasa res,
			 * ja hem definit el valor per defecte abans */
		}
	}
	try {
		sfx_volume = cfg.lookup("sfx_volume");
	} catch(const SettingNotFoundException &nfex) {
	}
	try {
		music_volume = cfg.lookup("music_volume");
	} catch(const SettingNotFoundException &nfex) {
	}
	return 0;
}

int Game::saveConfig() {
	Setting &root = cfg.getRoot();
	for (auto& kv : keys) {
		if (root.exists(kv.first)) {
			root.remove(kv.first);
		}
		root.add(kv.first, Setting::TypeInt) = kv.second;
	}
	if (root.exists("sfx_volume")) {
		root.remove("sfx_volume");
	}
	root.add("sfx_volume", Setting::TypeInt) = sfx_volume;
	if (root.exists("music_volume")) {
		root.remove("music_volume");
	}
	root.add("music_volume", Setting::TypeInt) = music_volume;

	try
	{
		cfg.writeFile(cfg_filename);
	}
	catch(const FileIOException &fioex)
	{
		// TODO: error grafic
		std::cerr << "I/O error while writing file: " << cfg_filename << std::endl;
		return 1;
	}
	return 0;
}




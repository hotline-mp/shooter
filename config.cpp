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

	std::vector<std::string> to_read {"up", "down", "left", "right"};
	for (std::string key : to_read) {
		try {
			keys[key] = cfg.lookup(key);
		} catch(const SettingNotFoundException &nfex) {
			/* si no esta a la config no pasa res,
			 * ja hem definit el valor per defecte abans */
		}
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




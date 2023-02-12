
#include "app/TodoApp.hpp"
#include "fs/FileCache.hpp"

int main(int argc, char** argv)
{
	std::string cfgPath = (argc>1) ? argv[1] : "appconfig.json";
	
	TodoAppConfig cfg {};
	if(auto jsonFile = TryReadFile(cfgPath)) {
		cfg = TodoAppConfig::Parse(crow::json::load(*jsonFile));
	}
	
	TodoApp app(cfg);
	app.run();
}

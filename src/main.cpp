
#include "app/TodoApp.hpp"

int main(int argc, char** argv)
{
	TodoApp app; //TODO read config from json file at argv[1]
	app.run();
}


#ifndef SIGMATODO_TEXTFILECOMPONENT_HPP
#define SIGMATODO_TEXTFILECOMPONENT_HPP

#include "Component.hpp"
#include "../fs/FileCache.hpp"

struct TextFileComponentData {
	FileCache& cache;
	std::string filename;
};

class TextFileComponent: public Component<TextFileComponentData> {
	using Component::Component;
	
	void renderTo(std::string &target) override;
};


#endif //SIGMATODO_TEXTFILECOMPONENT_HPP

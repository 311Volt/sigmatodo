
#ifndef SIGMATODO_DICTCOMPONENT_HPP
#define SIGMATODO_DICTCOMPONENT_HPP

#include "Component.hpp"
#include <unordered_map>

class DictComponent: public Component<std::unordered_map<std::string, std::string>> {
	using Component::Component;
	
	void renderTo(std::string &target) override;
};


#endif //SIGMATODO_DICTCOMPONENT_HPP

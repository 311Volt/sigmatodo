
#ifndef SIGMATODO_PLAINTEXTCOMPONENT_HPP
#define SIGMATODO_PLAINTEXTCOMPONENT_HPP

#include "Component.hpp"

class PlaintextComponent: public Component<std::string> {
public:
	using Component::Component;
	void renderTo(std::string& target) override;
};


#endif //SIGMATODO_PLAINTEXTCOMPONENT_HPP


#ifndef SIGMATODO_SUBVIEWCOMPONENT_HPP
#define SIGMATODO_SUBVIEWCOMPONENT_HPP

#include "Component.hpp"
#include "../view/HTMLView.hpp"

class SubviewComponent: public Component<HTMLViewInstance> {
public:
	using Component::Component;
	
	void renderTo(std::string& target) override;
};


#endif //SIGMATODO_SUBVIEWCOMPONENT_HPP

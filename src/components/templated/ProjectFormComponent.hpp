
#ifndef SIGMATODO_PROJECTFORMCOMPONENT_HPP
#define SIGMATODO_PROJECTFORMCOMPONENT_HPP

#include "../TemplatedComponent.hpp"
#include "../../entities/Project.hpp"

class ProjectFormComponent: public TemplatedComponent<Project, ProjectFormComponent> {
public:
	using TemplatedComponent::TemplatedComponent;
	void renderTo(std::string &target) override;
};


#endif //SIGMATODO_PROJECTFORMCOMPONENT_HPP

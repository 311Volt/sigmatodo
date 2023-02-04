
#ifndef SIGMATODO_PROJECTFORMCOMPONENT_HPP
#define SIGMATODO_PROJECTFORMCOMPONENT_HPP

#include "../TemplatedComponent.hpp"
#include "../../entities/Project.hpp"

struct ProjectFormComponentData {
	Project project;
	std::string action;
};

class ProjectFormComponent: public TemplatedComponent<ProjectFormComponentData, ProjectFormComponent> {
public:
	using TemplatedComponent::TemplatedComponent;
	void renderTo(std::string &target) override;
};


#endif //SIGMATODO_PROJECTFORMCOMPONENT_HPP

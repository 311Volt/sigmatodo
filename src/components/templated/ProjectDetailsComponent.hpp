
#ifndef SIGMATODO_PROJECTDETAILSCOMPONENT_HPP
#define SIGMATODO_PROJECTDETAILSCOMPONENT_HPP

#include "../TemplatedComponent.hpp"
#include "../../entities/Project.hpp"

class ProjectDetailsComponent: public TemplatedComponent<Project, ProjectDetailsComponent> {
public:
	using TemplatedComponent::TemplatedComponent;
	void renderTo(std::string &target) override;
};


#endif //SIGMATODO_PROJECTDETAILSCOMPONENT_HPP

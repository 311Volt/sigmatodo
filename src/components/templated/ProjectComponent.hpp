#ifndef SRC_COMPONENTS_PROJECTLISTCOMPONENT
#define SRC_COMPONENTS_PROJECTLISTCOMPONENT

#include "../TemplatedComponent.hpp"
#include "../../entities/Project.hpp"

#include <vector>

class ProjectComponent : public TemplatedComponent<Project, ProjectComponent> {
public:
	using TemplatedComponent::TemplatedComponent;
	void renderTo(std::string &target) override;
private:
};

#endif /* SRC_COMPONENTS_PROJECTLISTCOMPONENT */

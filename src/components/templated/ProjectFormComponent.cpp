
#include "ProjectFormComponent.hpp"

void ProjectFormComponent::renderTo(std::string &target)
{
	const auto& action = getData().action;
	const auto& [PROJECT_FIELD_NAMES] = getData().project;
	
	fmt::format_to(
		std::back_inserter(target), fmt::runtime(templateSource->get()),
		action, name, shortName, description
	);
}

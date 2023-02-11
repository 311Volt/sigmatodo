
#include "ProjectFormComponent.hpp"

void ProjectFormComponent::renderTo(std::string &target)
{
	const auto& [PROJECT_FIELD_NAMES] = getData().project;
	bool edit = getData().edit;
	
	fmt::format_to(
		std::back_inserter(target), fmt::runtime(templateSource->get()),
		name, shortName, description,
		edit ? "edit" : "new"
	);
}

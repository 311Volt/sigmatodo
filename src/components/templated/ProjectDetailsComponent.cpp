
#include "ProjectDetailsComponent.hpp"
#include "../../time/Time.hpp"

void ProjectDetailsComponent::renderTo(std::string &target)
{
	const auto& [PROJECT_FIELD_NAMES] = getData();
	fmt::format_to(
		std::back_inserter(target), fmt::runtime(templateSource->get()),
		name,
		shortName,
		Time::DateTimeStr(timeCreated),
		description,
		id
	);
}

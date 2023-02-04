#include "ProjectComponent.hpp"

#include <fmt/format.h>

void ProjectComponent::renderTo(std::string &target)
{

	const auto& [PROJECT_FIELD_NAMES] = getData();
	fmt::format_to(
		std::back_inserter(target),
		fmt::runtime(templateSource->get()),
		id, shortName
	);
}

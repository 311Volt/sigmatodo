#include <fmt/format.h>
#include "TaskComponent.hpp"

void TaskComponent::renderTo(std::string &target)
{

	const auto& [TASK_HDR_FIELD_NAMES] = getData();
	fmt::format_to(
		std::back_inserter(target),
		fmt::runtime(templateSource->get()),
		name
	);
}

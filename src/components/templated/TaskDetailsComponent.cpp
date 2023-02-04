
#include "TaskDetailsComponent.hpp"

#include <fmt/format.h>


void TaskDetailsComponent::renderTo(std::string &target) {
	
	const auto& [TASK_FIELD_NAMES] = getData();
	fmt::format_to(
		std::back_inserter(target),
		fmt::runtime(templateSource->get()),
		number, title, description
	);
}


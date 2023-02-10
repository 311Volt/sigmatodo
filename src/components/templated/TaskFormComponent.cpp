
#include "TaskFormComponent.hpp"

#include <fmt/format.h>

void TaskFormComponent::renderTo(std::string &target)
{
	const auto& [TASK_FIELD_NAMES] = getData().task;
	bool edit = getData().edit;
	fmt::format_to(
		std::back_inserter(target),
		fmt::runtime(templateSource->get()),
		projectId,
		name,
		title,
		description,
		dueDate,
		status,
		edit ? "edit" : "addtask",
		edit ? "Status" : "Initial status"
	);
	
}

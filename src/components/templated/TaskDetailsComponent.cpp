
#include "TaskDetailsComponent.hpp"
#include "../../time/Time.hpp"

#include <fmt/format.h>

void TaskDetailsComponent::renderTo(std::string &target) {
	
	const auto& [TASK_FIELD_NAMES] = getData();
	fmt::format_to(
		std::back_inserter(target),
		fmt::runtime(templateSource->get()),
		name,
		title,
		description,
		Time::DateTimeStr(dateCreated),
		Time::DateTimeStr(dateUpdated),
		Time::DateTimeStr(dueDate),
		status,
		Task::StatusNames.at(status),
		Task::StatusCodeNames.at(status),
		projectId
	);
}


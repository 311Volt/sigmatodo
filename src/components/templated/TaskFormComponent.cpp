
#include "TaskFormComponent.hpp"

#include <chrono>
#include <fmt/format.h>
#include <fmt/chrono.h>

namespace chr = std::chrono;

void TaskFormComponent::renderTo(std::string &target)
{
	const auto& [TASK_FIELD_NAMES] = getData().task;
	bool edit = getData().edit;
	auto tp = chr::time_point<chr::system_clock> {chr::seconds(dueDate)};
	
	std::string options;
	options.reserve(256);
	
	for(int i=0; i<Task::StatusNames.size(); i++) {
		fmt::format_to(
			std::back_inserter(options),
			R"({}<option value="{}"{}>{}</option>)",
			"\t\t\t",
			i,
			i==status ? " selected" : "",
			Task::StatusNames[i]
		);
	}
	
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
		edit ? "Status" : "Initial status",
		fmt::format("{:%Y-%m-%d}", tp),
		fmt::format("{:%H:%M}", tp),
		options
	);
	
}

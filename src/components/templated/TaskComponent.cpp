#include <fmt/format.h>
#include "TaskComponent.hpp"
#include "../../entities/Task.hpp"

#include "../../time/Time.hpp"
#include <chrono>

namespace chr = std::chrono;

std::string DueDateString(int64_t dueDate, bool& overdue)
{
	auto now = Time::SecsSinceEpoch();
	chr::seconds secs {std::abs(now - dueDate)};
	auto mins = chr::duration_cast<chr::minutes>(secs);
	auto hours = chr::duration_cast<chr::hours>(secs);
	auto days = chr::duration_cast<chr::days>(secs);
	auto weeks = chr::duration_cast<chr::weeks>(secs);
	auto months = chr::duration_cast<chr::months>(secs);
	auto years = chr::duration_cast<chr::years>(secs);
	
	std::string dur;
	
	if(years.count() > 0) dur = fmt::format("{}y", years.count());
	else if(months.count() > 0) dur = fmt::format("{}mo", months.count());
	else if(weeks.count() > 0) dur = fmt::format("{}w", weeks.count());
	else if(days.count() > 0) dur = fmt::format("{}d", days.count());
	else if(hours.count() > 0) dur = fmt::format("{}h", hours.count());
	else if(mins.count() > 0) dur = fmt::format("{}min", mins.count());
	else if(secs.count() > 0) dur = fmt::format("{}s", secs.count());
	
	if(dueDate > now) {
		overdue = false;
		return "in " + dur;
	} else {
		overdue = true;
		return dur + " ago";
	}
}


void TaskComponent::renderTo(std::string &target)
{
	const auto& [TASK_HDR_FIELD_NAMES] = getData();
	bool overdue = false;
	std::string dueDateStr = DueDateString(dueDate, overdue);
	
	fmt::format_to(
		std::back_inserter(target),
		fmt::runtime(templateSource->get()),
		name, title, dueDateStr, Task::StatusNames.at(status),
		overdue ? "tle-overdue" : "",
		Task::StatusCodeNames.at(status)
	);
	

}

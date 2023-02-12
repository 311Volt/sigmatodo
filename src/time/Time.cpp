#include "Time.hpp"

#include <chrono>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <iomanip>

namespace chr = std::chrono;

int64_t Time::SecsSinceEpoch() {
	auto now = chr::system_clock::now();

	return chr::duration_cast<chr::seconds>(now.time_since_epoch()).count();
}

std::string Time::DateStr(int64_t time) {
	chr::time_point<chr::system_clock> tp{chr::seconds(time)};
	return fmt::format("{:%Y-%m-%d}", tp);
}

std::string Time::DateTimeStr(int64_t time) {
	chr::time_point<chr::system_clock> tp{chr::seconds(time)};
	return fmt::format("{:%Y-%m-%d %H:%M:%S}", tp);
}

int64_t Time::ParseDateTime(const std::string& str)
{
	std::stringstream ss(str);
	std::tm tmDueDate {};
	ss >> std::get_time(&tmDueDate, "%Y-%m-%d %H:%M");
	return std::mktime(&tmDueDate);
}

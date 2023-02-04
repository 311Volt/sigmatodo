#include "Time.hpp"

#include <chrono>
#include <fmt/format.h>
#include <fmt/chrono.h>

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

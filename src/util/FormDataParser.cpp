
#include "FormDataParser.hpp"

#include <crow.h>

std::unordered_map<std::string, std::string> ParseQueryString(const std::string &str)
{
	std::unordered_map<std::string, std::string> ret;
	crow::query_string formdata("?" + str);
	for(const auto& key: formdata.keys()) {
		const char* val = formdata.get(key);
		ret[key] = val;
	}
	return ret;
}

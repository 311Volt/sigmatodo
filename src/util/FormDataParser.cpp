
#include "FormDataParser.hpp"

#include <crow.h>

std::unordered_map<std::string, std::string> QueryStrToMap(const crow::query_string& qstr)
{
	std::unordered_map<std::string, std::string> ret;
	for(const auto& key: qstr.keys()) {
		const char* val = qstr.get(key);
		ret[key] = val;
	}
	return ret;
}

std::unordered_map<std::string, std::string> ParseQueryString(const std::string &str)
{
	return QueryStrToMap(crow::query_string{"?" + str});
}

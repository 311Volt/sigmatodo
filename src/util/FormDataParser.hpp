
#ifndef SIGMATODO_FORMDATAPARSER_HPP
#define SIGMATODO_FORMDATAPARSER_HPP

#include <unordered_map>
#include <string>

#include <crow/query_string.h>

std::unordered_map<std::string, std::string> QueryStrToMap(const crow::query_string& qstr);
std::unordered_map<std::string, std::string> ParseQueryString(const std::string& str);

#endif //SIGMATODO_FORMDATAPARSER_HPP

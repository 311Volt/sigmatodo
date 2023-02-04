
#include "DictComponent.hpp"
#include <fmt/format.h>

void DictComponent::renderTo(std::string &target)
{
	target += "<table>";
	for(const auto& [key, value]: getData()) {
		fmt::format_to(std::back_inserter(target), "<tr><td>{0}</td><td>{1}</td></tr>", key, value);
	}
}

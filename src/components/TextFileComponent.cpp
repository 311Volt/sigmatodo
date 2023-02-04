
#include "TextFileComponent.hpp"

void TextFileComponent::renderTo(std::string &target)
{
	const auto& [cache, filename] = getData();
	target.append(cache.get(filename));
}

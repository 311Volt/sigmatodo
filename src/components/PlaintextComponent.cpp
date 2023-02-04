
#include "PlaintextComponent.hpp"

void PlaintextComponent::renderTo(std::string &target)
{
	target.append(getData());
}

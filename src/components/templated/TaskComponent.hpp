#ifndef SIGMATODO_TASKCOMPONENT_HPP
#define SIGMATODO_TASKCOMPONENT_HPP

#include "../TemplatedComponent.hpp"
#include "../../dto/dto.hpp"

class TaskComponent: public TemplatedComponent<TaskHeader, TaskComponent> {
public:
	using TemplatedComponent::TemplatedComponent;
	void renderTo(std::string &target) override;
};


#endif //SIGMATODO_TASKCOMPONENT_HPP

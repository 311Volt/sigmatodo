
#ifndef SIGMATODO_TASKFORMCOMPONENT_HPP
#define SIGMATODO_TASKFORMCOMPONENT_HPP

#include "../TemplatedComponent.hpp"
#include "../../entities/Task.hpp"

struct TaskFormComponentData {
	Task task;
	bool edit = false;
};

class TaskFormComponent: public TemplatedComponent<TaskFormComponentData, TaskFormComponent> {
public:
	using TemplatedComponent::TemplatedComponent;
	void renderTo(std::string &target) override;
};


#endif //SIGMATODO_TASKFORMCOMPONENT_HPP

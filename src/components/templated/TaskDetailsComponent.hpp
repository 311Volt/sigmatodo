#ifndef SIGMATODO_TASKDETAILSCOMPONENT_HPP
#define SIGMATODO_TASKDETAILSCOMPONENT_HPP


#include "../../entities/Task.hpp"
#include "../TemplatedComponent.hpp"


class TaskDetailsComponent : public TemplatedComponent<Task, TaskDetailsComponent> {
public:
	using TemplatedComponent::TemplatedComponent;
	void renderTo(std::string &target) override;
private:
};


#endif //SIGMATODO_TASKDETAILSCOMPONENT_HPP

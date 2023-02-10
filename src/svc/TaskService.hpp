#ifndef SRC_SVC_TASKSERVICE
#define SRC_SVC_TASKSERVICE

#include "../entities/Task.hpp"
#include "../dto/dto.hpp"
#include "ProjectService.hpp"

#include <sqlite.hpp>
#include <memory>
#include <optional>

class TaskService {
public:
	TaskService(std::shared_ptr<sql::Sqlite> db, ProjectService &projectService);

	int32_t createTask(const WriteTaskRequest &req);

	void editTask(int32_t id, const WriteTaskRequest &req);

	void updateStatus(int32_t id, int32_t status);

	std::optional<Task> getTask(int32_t id);

	std::optional<Task> getTaskByName(const std::string &name);

	std::vector<Task> getTaskList(int32_t projectId);

	std::vector<TaskHeader> getTaskHeaderList(int32_t projectId);
	
	std::vector<TaskHeader> getActiveTaskHeaderList();

private:

	using TaskStmtT = sql::Statement<TASK_FIELD_TYPES>;

	std::shared_ptr<sql::Sqlite> db;
	ProjectService &projectService;

	sql::Statement<void> stmtCreateTask, stmtUpdateTask, stmtUpdateStatus;
	TaskStmtT stmtGetTask, stmtGetTaskByName, stmtGetTaskList;
	TaskHeader::StmtT stmtGetTaskHeaders, stmtGetActiveTaskHeaders;
	sql::Statement<int32_t> stmtLastInsertRowID;
};

#endif /* SRC_SVC_TASKSERVICE */

#include "TaskService.hpp"

#include <fmt/format.h>
#include "../time/Time.hpp"

#ifndef STR
#define STR(x...) #x
#define XSTR(...) STR(__VA_ARGS__)
#endif

TaskService::TaskService(std::shared_ptr<sql::Sqlite> db, ProjectService &projectService)
		: db(db),
	projectService(projectService),
	
	stmtCreateTask(db->prepare(fmt::format(R"(
		INSERT INTO tasks ({}) VALUES ({});
	)", XSTR(TASK_FIELD_NAMES_NOID), TASK_PLACEHOLDERS_NOID).c_str())),
	
	stmtUpdateTask(db->prepare(R"(
		UPDATE tasks SET title = ?, description = ?, status = ?, dueDate = ?, dateUpdated = ? WHERE id = ?;
	)")),
	
	stmtUpdateStatus(db->prepare(R"(
		UPDATE tasks SET status = ?, dateUpdated = ? WHERE id = ?;
	)")),
	
	stmtGetTask(db->prepare<TASK_FIELD_TYPES>(fmt::format(R"(
		SELECT {} FROM tasks WHERE id = ?;
	)", XSTR(TASK_FIELD_NAMES)).c_str())),
	
	stmtGetTaskByName(db->prepare<TASK_FIELD_TYPES>(fmt::format(R"(
		SELECT {} FROM tasks WHERE name = ?;
	)", XSTR(TASK_FIELD_NAMES)).c_str())),
	
	stmtGetTaskList(db->prepare<TASK_FIELD_TYPES>(fmt::format(R"(
		SELECT {} FROM tasks WHERE projectId = ? ORDER BY status, dueDate;
	)", XSTR(TASK_FIELD_NAMES)).c_str())),
	
	stmtGetTaskHeaders(db->prepare<TASK_HDR_FIELD_TYPES>(fmt::format(R"(
		SELECT {} FROM tasks WHERE projectId = ? ORDER BY status, dueDate;
	)", XSTR(TASK_HDR_FIELD_NAMES)).c_str())),
	
	stmtGetActiveTaskHeaders(db->prepare<TASK_HDR_FIELD_TYPES>(fmt::format(R"(
		SELECT {} FROM tasks WHERE status = 0 ORDER BY status, dueDate;
	)", XSTR(TASK_HDR_FIELD_NAMES)).c_str())),
	
	stmtLastInsertRowID(db->prepare<int32_t>("SELECT last_insert_rowid()")) {

}

int32_t TaskService::createTask(const WriteTaskRequest &req) {
	auto taskName = projectService.getNewTaskNameAndIncrement(req.projectId).value();

	Task task = {
			.projectId = req.projectId,
			.name = taskName,
			.title = req.title,
			.description = req.description,
			.number = 0,
			.dateCreated = Time::SecsSinceEpoch(),
			.dateUpdated = Time::SecsSinceEpoch(),
			.dueDate = req.dueDate,
			.status = req.status
	};

	const auto &[TASK_FIELD_NAMES] = task;
	stmtCreateTask.execute(TASK_FIELD_NAMES_NOID);

	return stmtLastInsertRowID.execute()[0];
}

void TaskService::editTask(int32_t id, const WriteTaskRequest &req)
{
	stmtUpdateTask.execute(req.title, req.description, req.status, req.dueDate, Time::SecsSinceEpoch(), id);
}

void TaskService::updateStatus(int32_t id, int32_t status)
{
	stmtUpdateStatus.execute(status, Time::SecsSinceEpoch(), id);
}


using SelectResultT = std::vector<std::tuple<TASK_FIELD_TYPES>>;

std::optional<Task> GetFirstTask(const SelectResultT &rv) {
	if (!rv.empty()) {
		const auto &[TASK_FIELD_NAMES] = rv[0];
		return Task{TASK_FIELD_NAMES};
	}
	return {};
}

std::vector<Task> GetAllTasks(const SelectResultT &rv) {
	std::vector<Task> ret;
	for (const auto &[TASK_FIELD_NAMES]: rv)
		ret.push_back(Task{TASK_FIELD_NAMES});
	return ret;
}


std::optional<Task> TaskService::getTask(int32_t id) {
	return GetFirstTask(stmtGetTask.execute(id));
}

std::optional<Task> TaskService::getTaskByName(const std::string &name) {
	return GetFirstTask(stmtGetTaskByName.execute(name));
}

std::vector<Task> TaskService::getTaskList(int32_t projectId) {
	return GetAllTasks(stmtGetTaskList.execute(projectId));
}

std::vector<TaskHeader> TaskService::getTaskHeaderList(int32_t projectId) {
	auto rv = stmtGetTaskHeaders.execute(projectId);
	std::vector<TaskHeader> ret;
	for (const auto &[TASK_HDR_FIELD_NAMES]: rv)
		ret.push_back(TaskHeader{TASK_HDR_FIELD_NAMES});
	return ret;
}

std::vector<TaskHeader> TaskService::getActiveTaskHeaderList()
{
	auto rv = stmtGetActiveTaskHeaders.execute();
	std::vector<TaskHeader> ret;
	for (const auto &[TASK_HDR_FIELD_NAMES]: rv)
		ret.push_back(TaskHeader{TASK_HDR_FIELD_NAMES});
	return ret;
}

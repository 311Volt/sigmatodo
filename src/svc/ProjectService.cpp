#include "ProjectService.hpp"

#include "../time/Time.hpp"

#include <fmt/format.h>


#ifndef STR
#define STR(x...) #x
#define XSTR(...) STR(__VA_ARGS__)
#endif

ProjectService::ProjectService(std::shared_ptr<sql::Sqlite> db)
		: db(db),
	stmtCreate(db->prepare(fmt::format(R"(
		INSERT INTO projects ({}) VALUES ({});
	)", XSTR(PROJECT_FIELD_NAMES_NOID), PROJECT_PLACEHOLDERS_NOID).c_str())),
	
	stmtUpdate(db->prepare(R"(
		UPDATE projects SET name = ?, shortName = ?, description = ?
		WHERE id = ?;
	)")),
	
	stmtDelete(db->prepare(R"(
		DELETE FROM projects WHERE id = ?;
	)")),
	
	stmtIncrement(db->prepare(R"(
		UPDATE projects SET taskCounter = ? WHERE id = ?;
	)")),
	
	stmtGetProjectNames(db->prepare<std::string>(R"(
		SELECT name FROM projects;
	)")),
	
	stmtGetProject(db->prepare<PROJECT_FIELD_TYPES>(fmt::format(R"(
		SELECT {} FROM projects WHERE id = ?;
	)", XSTR(PROJECT_FIELD_NAMES)).c_str())),
	
	stmtGetProjects(db->prepare<PROJECT_FIELD_TYPES>(fmt::format(R"(
		SELECT {} FROM projects;
	)", XSTR(PROJECT_FIELD_NAMES)).c_str())),
	
	stmtLastInsertRowID(db->prepare<int32_t>("SELECT last_insert_rowid()"))
{}

int32_t ProjectService::createProject(const WriteProjectRequest &req) {
	Project project = {
			.name = req.name,
			.shortName = req.shortName,
			.description = req.description,
			.timeCreated = Time::SecsSinceEpoch(),
			.taskCounter = 1
	};

	const auto &[PROJECT_FIELD_NAMES] = project;
	stmtCreate.execute(PROJECT_FIELD_NAMES_NOID);

	return stmtLastInsertRowID.execute()[0];
}

std::optional<std::string> ProjectService::getNewTaskNameAndIncrement(int32_t projectId) {
	if (auto project = getProject(projectId)) {
		stmtIncrement.execute(project->taskCounter + 1, projectId);
		return fmt::format("{}-{}", project->shortName, project->taskCounter);
	}
	return {};
}

void ProjectService::updateProject(int32_t id, const WriteProjectRequest &req) {
	stmtUpdate.execute(req.name, req.shortName, req.description, id);
}

void ProjectService::deleteProject(int32_t id) {
	stmtDelete.execute(id);
}

std::vector<std::string> ProjectService::getProjectNames() {
	return stmtGetProjectNames.execute();
}

std::optional<Project> ProjectService::getProject(int32_t projectId) {
	auto result = stmtGetProject.execute(projectId);
	if (!result.empty()) {
		const auto &[PROJECT_FIELD_NAMES] = result[0];
		return Project{PROJECT_FIELD_NAMES};
	}
	return {};
}

std::vector<Project> ProjectService::getProjects() {
	std::vector<Project> ret;
	auto result = stmtGetProjects.execute();
	for (const auto &[PROJECT_FIELD_NAMES]: result) {
		ret.push_back(Project{PROJECT_FIELD_NAMES});
	}
	return ret;
}

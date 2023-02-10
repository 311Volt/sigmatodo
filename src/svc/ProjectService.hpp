#ifndef SRC_SVC_PROJECTSERVICE
#define SRC_SVC_PROJECTSERVICE

#include "../entities/Project.hpp"
#include "../dto/dto.hpp"
#include <sqlite.hpp>
#include <memory>
#include <optional>

class ProjectService {
public:
	explicit ProjectService(std::shared_ptr<sql::Sqlite> db);

	int32_t createProject(const WriteProjectRequest &req);

	void updateProject(int32_t id, const WriteProjectRequest &req);

	void deleteProject(int32_t id);

	std::vector<std::string> getProjectNames();

	std::optional<Project> getProject(int32_t projectId);

	std::vector<Project> getProjects();

	std::optional<std::string> getNewTaskNameAndIncrement(int32_t projectId);

private:
	using ProjectStmtT = sql::Statement<PROJECT_FIELD_TYPES>;

	std::shared_ptr<sql::Sqlite> db;

	sql::Statement<void> stmtCreate, stmtUpdate, stmtDelete, stmtIncrement;

	sql::Statement<std::string> stmtGetProjectNames;
	ProjectStmtT stmtGetProject, stmtGetProjects;
	sql::Statement<int32_t> stmtLastInsertRowID;
};

#endif /* SRC_SVC_PROJECTSERVICE */

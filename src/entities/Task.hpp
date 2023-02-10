#ifndef SRC_ENTITIES_TASK
#define SRC_ENTITIES_TASK

#include <string>
#include <stdint.h>
#include <array>

struct Task {
	int32_t id = 0;
	int32_t projectId = 0;
	std::string name;
	std::string title;
	std::string description;
	int32_t number = 0;
	int64_t dateCreated = 0;
	int64_t dateUpdated = 0;
	int64_t dueDate = 0;
	int32_t status = Todo;

#define TASK_FIELD_NAMES id, projectId, name, title, description, number, dateCreated, dateUpdated, dueDate, status
#define TASK_FIELD_NAMES_NOID projectId, name, title, description, number, dateCreated, dateUpdated, dueDate, status
#define TASK_PLACEHOLDERS "?, ?, ?, ?, ?, ?, ?, ?, ?, ?"
#define TASK_PLACEHOLDERS_NOID "?, ?, ?, ?, ?, ?, ?, ?, ?"
#define TASK_FIELD_TYPES int32_t, int32_t, std::string, std::string, std::string, int32_t, int64_t, int64_t, int64_t, int32_t

	static constexpr char DDL[] = R"(
		CREATE TABLE IF NOT EXISTS tasks (
			id           INTEGER PRIMARY KEY AUTOINCREMENT,
			projectId    INTEGER,
			name         TEXT,
			title        TEXT,
			description  TEXT,
			number       INTEGER,
			dateCreated  INTEGER,
			dateUpdated  INTEGER,
			dueDate      INTEGER,
			status       INTEGER,

			FOREIGN KEY (projectId)
				REFERENCES projects(id)
				ON UPDATE CASCADE ON DELETE CASCADE
		);

		CREATE INDEX IF NOT EXISTS idx_task_name ON tasks(name);
		CREATE INDEX IF NOT EXISTS idx_task_projectid ON tasks(projectId);
		CREATE INDEX IF NOT EXISTS idx_task_status ON tasks(status);
	)";


	enum Status {
		Active = 0,
		OnHold = 1,
		Todo = 2,
		Done = 3,
		WontDo = 4
	};

	static constexpr std::array<std::string_view, 5> StatusNames = {
		"active",
		"on hold",
		"todo",
		"done",
		"won't do"
	};
	
	static constexpr std::array<std::string_view, 5> StatusCodeNames = {
		"active",
		"onhold",
		"todo",
		"done",
		"wontdo"
	};
};

#endif /* SRC_ENTITIES_TASK */

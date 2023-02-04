#ifndef SRC_DTO_DTO
#define SRC_DTO_DTO

#include <stdint.h>
#include <string>

#include <sqlite.hpp>

struct AddNoteRequest {
	int32_t taskId;
	std::string content;
};

struct CreateProjectRequest {
	std::string name;
	std::string shortName;
	std::string description;
};

struct CreateTaskRequest {
	int32_t projectId;
	std::string title;
	std::string description;
	int64_t dueDate;
};

struct EditTaskRequest {
	std::string title;
	std::string description;
	int32_t status;
};

struct TaskHeader {
	std::string name;
	std::string title;
	int64_t dueDate;
	int32_t status;

#define TASK_HDR_FIELD_NAMES name, title, dueDate, status
#define TASK_HDR_FIELD_TYPES std::string, std::string, int64_t, int32_t

	using StmtT = sql::Statement<TASK_HDR_FIELD_TYPES>;
};

#endif /* SRC_DTO_DTO */

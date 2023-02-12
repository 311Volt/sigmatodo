#ifndef SRC_ENTITIES_PROJECT
#define SRC_ENTITIES_PROJECT

#include <stdint.h>
#include <string>
#include <fmt/format.h>

struct Project {
	int32_t id = 0;
	std::string name;
	std::string shortName;
	std::string description;
	int64_t timeCreated = 0;
	int32_t taskCounter = 0;

#define PROJECT_FIELD_NAMES id, name, shortName, description, timeCreated, taskCounter
#define PROJECT_FIELD_NAMES_NOID name, shortName, description, timeCreated, taskCounter
#define PROJECT_FIELD_TYPES int32_t, std::string, std::string, std::string, int64_t, int32_t
#define PROJECT_PLACEHOLDERS_NOID "?, ?, ?, ?, ?"

	static constexpr char DDL[] = R"(
		CREATE TABLE IF NOT EXISTS projects (
			id           INTEGER PRIMARY KEY AUTOINCREMENT,
			name         TEXT UNIQUE,
			shortName    TEXT UNIQUE,
			description  TEXT,
			timeCreated  INTEGER,
			taskCounter  INTEGER
		);
		
		CREATE UNIQUE INDEX IF NOT EXISTS idx_proj_name ON projects(name);
		CREATE UNIQUE INDEX IF NOT EXISTS idx_proj_shortname ON projects(shortName);
	)";

	inline std::string title() {
		return fmt::format("[{}] {}", shortName, name);
	}
	
	inline std::string nameOfNextTask() {
		return fmt::format("{}-{}", shortName, taskCounter);
	}
};


#endif /* SRC_ENTITIES_PROJECT */

#ifndef SRC_ENTITIES_NOTE
#define SRC_ENTITIES_NOTE

#include <string>
#include <stdint.h>

struct Note {
	uint32_t id;
	uint32_t taskId;
	int64_t dateCreated;
	std::string content;

	static constexpr std::string_view DDL = R"(
		CREATE TABLE IF NOT EXISTS notes (
			id           INTEGER PRIMARY KEY AUTOINCREMENT,
			taskId       INTEGER,
			dateCreated  INTEGER

			FOREIGN KEY (taskId)
				REFERENCES tasks(id)
				ON UPDATE CASCADE ON DELETE CASCADE;
		);
	)";
};

#endif /* SRC_ENTITIES_NOTE */

#ifndef SRC_TIME_TIME
#define SRC_TIME_TIME

#include <stdint.h>
#include <string>

namespace Time {
	int64_t SecsSinceEpoch();

	std::string DateStr(int64_t time);

	std::string DateTimeStr(int64_t time);
}


#endif /* SRC_TIME_TIME */

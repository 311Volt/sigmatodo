
#ifndef SIGMATODO_DIRWATCHER_HPP
#define SIGMATODO_DIRWATCHER_HPP

/// TODO add inotify support for linux

#include <string>
#include <chrono>
#include <unordered_map>

class DirWatcher {
public:
	explicit DirWatcher(std::string dirname);
	bool detectChanges();
private:
	using WriteTimeTP = std::chrono::time_point<std::chrono::file_clock>;
	
	std::string dirname;
	std::unordered_map<std::string, WriteTimeTP> lastKnownState;
};


#endif //SIGMATODO_DIRWATCHER_HPP

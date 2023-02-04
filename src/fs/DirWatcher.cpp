
#include "DirWatcher.hpp"

#include <unordered_set>
#include <stdexcept>
#include <filesystem>
namespace fs = std::filesystem;

DirWatcher::DirWatcher(std::string dirname)
	: dirname(std::move(dirname))
{
	if(fs::directory_entry(this->dirname).is_directory() == false) {
		throw std::runtime_error(this->dirname + " is not a directory");
	}
}

bool DirWatcher::detectChanges()
{
	bool result = false;
	fs::recursive_directory_iterator iter(dirname);
	
	std::unordered_set<std::string> missingFiles;
	for(const auto& [path, date]: lastKnownState) {
		missingFiles.insert(path);
	}
	
	for(const auto& entry: iter) {
		auto cacheIt = lastKnownState.find(entry.path());
		
		if(cacheIt == lastKnownState.end()) {
			lastKnownState.insert({entry.path(), entry.last_write_time()});
			result = true; //change detected: new file
		} else {
			auto& [path, date] = *cacheIt;
			missingFiles.erase(path);
			if(entry.last_write_time() > date) {
				date = entry.last_write_time();
				result = true; //change detected: write time has changed
			}
		}
	}
	
	if(!missingFiles.empty()) {
		for(const auto& path: missingFiles) {
			lastKnownState.erase(path);
		}
		result = true; //change detected: file has been deleted
	}
	
	return result;
}

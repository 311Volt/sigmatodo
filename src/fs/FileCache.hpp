
#ifndef SIGMATODO_FILECACHE_HPP
#define SIGMATODO_FILECACHE_HPP

#include <stdint.h>
#include <unordered_map>

#include "StringSource.hpp"

class FileCacheStringSource;

struct CachedFileInfo {
	std::string content;
	std::string mimeType;
};

class FileCache {
public:
	const std::string& get(const std::string& path);
	const CachedFileInfo& getWithInfo(const std::string& path);
	void invalidate();
	uint64_t getSize() const;
	void setEnabled(bool value);

	FileCacheStringSource getSource(const std::string& path);
	
	void cacheRecursive(const std::string& dir);
private:
	bool isEnabled = false;
	uint64_t size = 0;
	
	std::unordered_map<std::string, CachedFileInfo> data;
};


class FileCacheStringSource: public StringSource {
public:
	FileCacheStringSource(FileCache& cache, std::string path);

	const std::string& get() override;
private:
	std::string path;
	FileCache& cache;
};


#endif //SIGMATODO_FILECACHE_HPP

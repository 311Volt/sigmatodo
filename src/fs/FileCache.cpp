
#include <fmt/format.h>
#include <optional>
#include "FileCache.hpp"
#include <crow.h>

#include <filesystem>
namespace fs = std::filesystem;

//TODO always use canonical paths


std::optional<std::string> TryReadFile(const std::string& filename);

const std::string &FileCache::get(const std::string &path)
{
	return getWithInfo(path).content;
}

const CachedFileInfo& FileCache::getWithInfo(const std::string &path)
{
	if(auto it = data.find(path); it != data.end()) {
		//fmt::print("  cache hit: {}\n", path);
		return it->second;
	}
	//fmt::print("  cache miss: {}\n", path);
	
	auto file = TryReadFile(path);
	if(!file) {
		throw std::runtime_error("no such file: " + path);
	}
	
	auto lastDot = path.find_last_of('.');
	auto pathExt = path.substr(std::min(path.size(), lastDot+1));
	
	auto ins = data.insert({path, {
		.content = *file,
		.mimeType = crow::response::get_mime_type(pathExt)
	}});
	size += file->size();
	
	return ins.first->second;
}

void FileCache::invalidate()
{
	data.clear();
}

uint64_t FileCache::getSize() const
{
	return size;
}

void FileCache::setEnabled(bool value)
{
	isEnabled = value;
}

FileCacheStringSource FileCache::getSource(const std::string &path)
{
	return FileCacheStringSource(*this, path);
}

void FileCache::cacheRecursive(const std::string &dir)
{
	std::error_code ec;
	for(const auto& entry: fs::recursive_directory_iterator(dir, ec)) {
		if(entry.is_regular_file()) {
			fmt::print("caching {}\n", entry.path().string());
			this->get(entry.path());
		}
	}
}


std::optional<std::string> TryReadFile(const std::string& filename)
{
	std::ifstream in(filename.c_str());
	if (in.is_open())
	{
		std::string ret;
		in.seekg(0, std::ios::end);
		ret.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(ret.data(), (long)ret.size());
		return ret;
	}

	return {};
}

FileCacheStringSource::FileCacheStringSource(FileCache &cache, std::string path)
 : path(std::move(path)), cache(cache)
{

}

const std::string &FileCacheStringSource::get()
{
	return cache.get(path);
}

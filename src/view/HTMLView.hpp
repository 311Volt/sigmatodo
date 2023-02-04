#ifndef SIGMATODO_HTMLVIEW_HPP
#define SIGMATODO_HTMLVIEW_HPP

#include <unordered_map>
#include "../components/Component.hpp"
#include "../fs/FileCache.hpp"

#include <vector>

class HTMLViewInstance;

class HTMLView {
public:
	HTMLView(FileCache &cache, std::string filename);
	explicit HTMLView(const std::string &content);
	
	HTMLView(const HTMLView&) = delete;
	HTMLView& operator=(const HTMLView& rhs) = delete;
	
	
	HTMLView (HTMLView &&other) = default;
	HTMLView & operator=(HTMLView &&) = default;
	
	HTMLView() = default;
	
	HTMLViewInstance createInstance();
	void reset();
	void reload();
private:
	friend class HTMLViewInstance;
	void loadContent(const std::string& content);
	
	FileCache* fileCache = nullptr;
	std::string filename;
	std::vector<std::string> fragments;
};

class HTMLViewInstance {
public:
	std::string render() const;
	void renderTo(std::string& target) const;
	
	template<ComponentType ComponentT>
	void addComponent(const std::string& name, ComponentT::DataT&& data)
	{
		components.emplace(name, std::unique_ptr<BaseComponent>(new ComponentT(std::move(data))));
	}
	
	template<ComponentType ComponentT>
	void addComponent(const std::string& name, const ComponentT::DataT& data)
	{
		components.emplace(name, std::unique_ptr<BaseComponent>(new ComponentT(data)));
	}
private:
	friend class HTMLView;
	const HTMLView& view;
	std::unordered_map<std::string, std::unique_ptr<BaseComponent>> components{};
	
	explicit HTMLViewInstance(const HTMLView& view);
	
};

#endif //SIGMATODO_HTMLVIEW_HPP

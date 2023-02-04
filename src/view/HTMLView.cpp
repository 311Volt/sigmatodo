
#include <fmt/format.h>
#include "HTMLView.hpp"

#include <algorithm>
#include <utility>

static constexpr char StrComponentBegin[] = "____";
static constexpr char StrComponentEnd[] = ";";


HTMLView::HTMLView(FileCache &cache, std::string filename)
	: fileCache(&cache),
	  filename(std::move(filename))
{
	reload();
}


HTMLView::HTMLView(const std::string &content)
{
	loadContent(content);
}

void HTMLView::reload()
{
	auto viewFile = fileCache->get(filename);
	loadContent(viewFile);
}

void HTMLView::loadContent(const std::string &content)
{
	enum State {
		Norm,
		SubToken
	};
	State parserState = Norm;
	fragments.clear();
	for(unsigned i=0; i<content.size();) {
		unsigned long next;
		if(parserState == Norm) {
			next = content.find(StrComponentBegin, i);
			parserState = SubToken;
		} else {
			next = content.find(StrComponentEnd, i);
			next = std::min(next+1, content.size()); //TODO don't assume strlen(StrComponentEnd)==1
			parserState = Norm;
		}
		fragments.push_back(content.substr(i, next-i));
		i = next;
	}
}


HTMLViewInstance HTMLView::createInstance()
{
	return HTMLViewInstance(*this);
}

HTMLViewInstance::HTMLViewInstance(const HTMLView &view)
	: view(view)
{

}



std::string HTMLViewInstance::render() const
{
	std::string ret;
	ret.reserve(4096);
	renderTo(ret);
	return ret;
}

void HTMLViewInstance::renderTo(std::string &target) const
{
	for(const auto& frag: view.fragments) {
		if(frag.starts_with(StrComponentBegin)) {
			auto fragId = frag.substr(std::size(StrComponentBegin)-1);
			if(!fragId.empty()) {
				fragId.pop_back(); //TODO don't assume strlen(StrComponentEnd)==1
			}
			if(auto comp = components.find(fragId); comp != components.end()) {
				comp->second->renderTo(target);
			}
		} else {
			target += frag;
		}
	}
}


#ifndef SIGMATODO_TEMPLATEDCOMPONENT_HPP
#define SIGMATODO_TEMPLATEDCOMPONENT_HPP

#include "Component.hpp"
#include "../fs/StringSource.hpp"

template<typename DataType, typename Derived>
class TemplatedComponent: public Component<DataType> {
public:
	using Base = Component<DataType>;
	using Base::Base;

	template<typename StrSrcT>
		requires std::is_base_of_v<StringSource, StrSrcT>
	static void setTemplateSource(StrSrcT&& src)
	{
		templateSource = std::unique_ptr<StringSource>(new StrSrcT(std::forward<StrSrcT>(src)));
	}
	virtual void renderTo(std::string& target) = 0;

protected:
	//TODO accessor method that throws when templateSource == nullptr
	static std::unique_ptr<StringSource> templateSource;
};

template<typename DataType, typename Derived>
std::unique_ptr<StringSource> TemplatedComponent<DataType, Derived>::templateSource = {};

#endif //SIGMATODO_TEMPLATEDCOMPONENT_HPP

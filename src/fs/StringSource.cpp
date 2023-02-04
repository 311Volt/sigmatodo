
#include "StringSource.hpp"

#include <utility>

const ConstantStringSource ConstantStringSource::Empty = ConstantStringSource("");

ConstantStringSource::ConstantStringSource(std::string str)
 : str(std::move(str))
{

}

const std::string &ConstantStringSource::get()
{
	return str;
}

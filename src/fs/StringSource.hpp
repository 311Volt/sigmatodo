
#ifndef SIGMATODO_STRINGSOURCE_HPP
#define SIGMATODO_STRINGSOURCE_HPP

#include <string>

class StringSource {
public:
	virtual const std::string& get() = 0;
};

class ConstantStringSource: public StringSource {
public:
	explicit ConstantStringSource(std::string str);
	const std::string& get() override;

	static const ConstantStringSource Empty;
private:
	std::string str;
};


#endif //SIGMATODO_STRINGSOURCE_HPP

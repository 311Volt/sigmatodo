#ifndef SRC_COMPONENTS_COMPONENT
#define SRC_COMPONENTS_COMPONENT

#include <string>
#include <type_traits>
#include <variant>
#include <memory>

class BaseComponent
{
public:
	virtual void renderTo(std::string &target) = 0;
};

template<typename DataType>
class Component: public BaseComponent {
public:
	using DataT = DataType;

	Component() = delete;
	Component(const Component&) = delete;
	Component(Component&&) = delete;

	explicit Component(DataT&& data): data(std::make_unique<DataT>(std::move(data))) {}
	explicit Component(const DataT& data): data(&data) {}
	explicit Component(const DataT* data): data(data) {}
	virtual ~Component() = default;
	
	void renderTo(std::string &target) override = 0; //for clangd
	
protected:
	const DataT& getData()
	{
		if(std::holds_alternative<const DataT*>(data))
			return *std::get<const DataT*>(data);
		else
			return *std::get<std::unique_ptr<DataT>>(data);
	}

private:
	std::variant<const DataT*, std::unique_ptr<DataT>> data;
};

template<typename T>
concept ComponentType =
	std::is_base_of_v<BaseComponent, T> &&
	requires(T x){
		typename T::DataT;
	};

#endif /* SRC_COMPONENTS_COMPONENT */

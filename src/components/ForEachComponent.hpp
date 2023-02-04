//
// Created by volt on 28/01/23.
//

#ifndef SIGMATODO_FOREACHCOMPONENT_HPP
#define SIGMATODO_FOREACHCOMPONENT_HPP

#include <vector>
#include "Component.hpp"

template<typename ElemDataType>
struct ForEachComponentData {
	std::vector<ElemDataType> elements;
};

template<ComponentType ElemComponentT>
class ForEachComponent: public Component<ForEachComponentData<typename ElemComponentT::DataT>> {
public:
	using Base = Component<ForEachComponentData<typename ElemComponentT::DataT>>;
	using Base::Base;

	inline void renderTo(std::string& target) override
	{
		for(const auto& elem: this->getData().elements) {
			ElemComponentT component(&elem);
			component.renderTo(target);
		}
	}
private:

};


#endif //SIGMATODO_FOREACHCOMPONENT_HPP

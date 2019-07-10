#include "../component.h"

namespace trigger::comp
{
	class collider : public component
	{
	public:
		enum collider_type
		{
			_2D,
			_3D
		};
		property is_trigger;
		property type;

		collider() : component(T_CLASS)
		{
			is_trigger = property(false, trigger::property::data_type::Bool, "is Trigger", &this->properties);
		}
	};
}
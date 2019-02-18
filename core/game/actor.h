#include <string>
#include "component.h"
#include "fsm.h"
#include "vec.h"

namespace trigger
{
	struct transform
	{
		trigger::vec position, rotation, scale;
	};

	class actor : public trigger::component
	{
	public:
		bool is_static;
		std::string name;
		std::unique_ptr<actor> parent;
		std::unique_ptr<actor> child;
		transform s_transform;
		trigger::fsm::map fsm;
		std::list<component> s_components;
		
		virtual void update( float delta ) noexcept override
		{
			fsm.update( delta );
		}

		actor() : component()
		{
			s_transform.position = vec( 0, 0, 0 );
			s_transform.rotation = vec( 0, 0, 0 );
			s_transform.scale = vec( 1, 1, 1 );

			//when that macro call in construtor, it working Initialized variable.
			//so when new actor born and wake up, his name r "Actor".
			SAVE_TOML(position, s_transform.position.to_toml());
			SAVE_TOML(rotation, s_transform.rotation.to_toml());
			SAVE_TOML(scale, s_transform.scale.to_toml());

			s_components = std::list<component>();
		}

		~actor()
		{

		}
	};
}



#include "gizmo.h"
#include "../../../trigger-engine/core/game/transform.h"
void gizmo::update(float delta)
{
	get_my_transform()->position.x += delta;
	this->set_property("TEST", *this->properties[hash_str("TEST")].get_float() + delta );
	trigger::tools::console::get_instance()->log("call");
};
gizmo::~gizmo()
{
};
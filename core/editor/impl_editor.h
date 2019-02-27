#ifndef EDITOR_H
#define EDITOR_H

#include "../../imgui/imgui.h"

#include "../game/component.h"

namespace trigger::edit
{
	class impl_editor : public trigger::component
	{
	public:
		// Using imgui Draw module.
		// engine->editors
		virtual bool draw() const noexcept = 0;

		// Update editor vals
		virtual void update(float delta) noexcept = 0;
	};
} // namespace trigger::edit

#endif
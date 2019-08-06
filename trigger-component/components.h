#pragma once
#include "../trigger-engine/core/game/component.h"

#define regi_component(type) trigger::manager::class_manager::regi_component<type>(#type);
//DO NOT EDIT THIS FILE!
#include "src/camera/camera.h"
#include "src/Shader/Shader.h"
#include "src/material/material.h"

//Regist Component
static void reload(){
	regi_component(camera);
	regi_component(Shader);
	regi_component(material);
}

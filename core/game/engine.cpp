#include "engine.h"


bool trigger::core::engine::init()
{
    this->renderer = new trigger::renderer::REND(800,600,true);
    
    run();
    return true;
}

int trigger::core::engine::run()
{
    return 0;
}

trigger::core::engine::~engine()
{

}
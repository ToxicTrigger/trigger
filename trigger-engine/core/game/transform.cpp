#include "transform.h"
trigger::transform::~transform()
{
	clear_child();
	 
	this->components.clear();
	this->parent = nullptr;
	this->childs.clear();
	//delete parent;
	T_LOG("transform : Delete " + this->name + ":" + std::to_string(this->get_instance_id()));
}

void trigger::transform::update(float delta)
{
	for (auto&& i : this->components)
	{
		if(i.second->active) 
			i.second->update(delta);
	}
}

trigger::transform *trigger::transform::get_parent()
{
    return this->parent;
}

void trigger::transform::set_parent(trigger::transform *target)
{
	if (this->parent == nullptr || this->parent->get_instance_id() != target->get_instance_id())
	{
		this->parent = target;
		target->add_child(this);
	}
}

trigger::transform *trigger::transform::get_child_at(size_t index)
{
    return this->childs[index];
}

std::vector<trigger::transform*> trigger::transform::get_childs()
{
    return this->childs;
}

void trigger::transform::add_child(trigger::transform *child)
{
	child->parent = this;
	bool already = false;
	for (auto& id : this->childs)
	{
		if (id->get_instance_id() == child->get_instance_id())
		{
			already = true;
			break;
		}
	}

	if(!already) this->childs.push_back(child);
}

void trigger::transform::remove_parent()
{
	if (this->parent != nullptr)
	{
		this->parent->remove_child(this);
		this->parent = nullptr;
	}
}

// disconnect parent
// because Object Delete is only can hierarchy or Object self. 
void trigger::transform::remove_child(size_t index)
{
	this->childs[index]->parent = nullptr;
}

void trigger::transform::remove_child(trigger::transform *child)
{
	for(auto& i : this->childs)
	{
		if(i->get_instance_id() == child->get_instance_id())
		{
			i->parent = nullptr;
		}
	}
}

void trigger::transform::clear_child()
{
	for(auto& i : this->childs)
	{
		i->remove_parent();
	}
	this->childs.clear();
}

// Delete All Child
void trigger::transform::clear_and_destroy_child()
{
	for(auto i : this->childs)
	{
		delete i;
		i = nullptr;
	}
	this->clear_child();
}

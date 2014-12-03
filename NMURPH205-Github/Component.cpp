#include "Component.h"
#include "GameObject.h"	//Added lab 6 - 8.1

Component::Component()		//This is the constructor
{
	m_Type = "Component";
	m_Active = true;
}

Component::~Component()		//This is the deonstructor
{

}
//Classes like this usually have empty functions
void Component::init()
{

}

void Component::update()
{

}

void Component::render()
{

}

void Component::destroy()
{

}

const std::string& Component::getType()
{
	return m_Type;
}

bool Component::isActive()
{
	return m_Active;
}

void Component::toggleActive()
{
	m_Active = !m_Active;
}

void Component::setParent(GameObject *object)
{
	m_Parent = object;
}
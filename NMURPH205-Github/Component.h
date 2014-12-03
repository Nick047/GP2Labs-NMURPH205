#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

class GameObject;

class Component
{
public:
	Component();		//Constructor with no parameters
	virtual ~Component();	//Virtual deconstructor

	virtual void init();

	virtual void update();

	virtual void render();

	virtual void destroy();

	//^ These functions are virtual to allow for any child classes to overrride them
	const std::string& getType();

	bool isActive();	//returns bool

	void toggleActive();	//returns void

	void setParent(GameObject * object);
protected:
	std::string m_Type;
	bool m_Active;
	GameObject * m_Parent;
	//^ Some of these variables may need to be included
private:
};

#endif
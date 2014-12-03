#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Component.h"

class Transform :public Component	//inherits from component
{
public:
	Transform();	//This is a public no parameter constructor
	~Transform();	//This is a public deconstructor

	void update();

	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);

	vec3& getPosition();
	vec3& getScale();
	vec3& getRotation();
	mat4& getModel();
private:
	vec3 m_Position;
	vec3 m_Scale;
	vec3 m_Rotation;
	mat4 m_Model;
protected:
};


#endif
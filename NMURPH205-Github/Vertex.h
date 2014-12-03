#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>
using glm::vec4;
using glm::vec3;
using glm::vec2;

struct Vertex
{
	vec3 position;	//3dimensional coordinates
	vec2 texCoords;	//The u,v coordinates of thetexture
	vec4 colours;	//Colors & alpha
	vec3 tangentNormals;
	vec3 binormals;
};

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#endif
#version 150

in vec3 vertexPostion;
//
in vec2 VertexTexCoords;
out vec2 VertexTexCoordsOut;

uniform mat4 MVP;

void main()
{
	VertexTexCoordsOut = vertexTexCoords;
	gl_Position = MVP * vec4(vetexPosition, 1.0);

}
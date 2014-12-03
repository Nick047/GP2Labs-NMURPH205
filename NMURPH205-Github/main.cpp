//Add the shader/assets folder to the solution directory
//Add a text file to the project called simpleVS.glsl		//make sure the locations point to the shade folder
//Add an additional include directory for C:\Specialised Applications\glm
//Put the headers in it's own folder as the main directoy is cramped

//Maths headers
#include <glm/glm.hpp>
using glm::mat4;
using glm::vec4;
using glm::vec3;

//Header Files
#include <iostream>	//Adds peripheral capabilities
#include <GL/glew.h>		//The header for GLEW functionality

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/* ^genertates about 60 errors
^These headers are to be used ith the above directory */


#ifdef __APPLE__	//The definitions for use with mac format
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <OpenGL/glu.h>
#include <CoreFoundation/CoreFoundation.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>

#elif WIN32 
#include <SDL.h>		//The header for SDL2 functionality
#include <SDL_opengl.h>		//The header for the OpenGL headers
#include <gl\GLU.h>
#include <SDL_image.h>	//LINKS THE sdl_iMAGE HEADER
#include <SDL_ttf.h>	//Adds font capability
#endif	//Need this in when using mac

#ifdef _DEBUG && WIN32		//Asst paths for windows
const std::string ASSET_PATH = "../assets/";	//The asset folder path to be used 
const std::string SHADER_PATH = "shaders/";		//The sub folder path of assets for shaders
const std::string TEXTURE_PATH = "textures/";	//The sub folder path of assets for textures
const std::string FONT_PATH = "fonts/";		//The sub folder path of assets for fonts
const std::string MODEL_PATH = "model/";	//The sub folder path of assets for models

#elif __APPLE__		//Asset paths for mac
const std::string ASSET_PATH;
const std::string SHADER_PATH;
const std::string TEXTURE_PATH;
const std::string FONT_PATH;
condt std::string MODEL_PATH;

#else
const std::string ASSETS_PATH = "/assets/";
const std::string SHADER_PATH = "shaders/";
condt std::string TEXTURE_PATH = "textures/";
const std::string FONT_PATH = "fonts/";
const std::string MODEL_PATH = "models/";
#endif

#include <vector>


//Local Headers
#include "Texture.h"	//Links our texture header
#include "Shader.h"		//Links our shader header
#include "Vertex.h"		//LInks our vertex header
#include "GameObject.h"	//Links our GameObect header
#include "Transform.h"	//Links our Transform header
#include "Mesh.h"		//Links our Mesh header
#include "Material.h"	//LInks our material header
#include "Camera.h"		//Links our Camera header
#include "Light.h"		//Links our Light header
#include "FBXLoader.h"	//Links our FBXLoader header

#include "PostProcessing.h"	//Links our PostProcessing header
#include "ColourFilters.h"	//Links our ColourFilters header

//const std : string FONT_PATH = "/fonts";	//This is alreay initalised above
//Do i need a textre ath similar to the fonts and shaders above.
//GLuint fontTexture = 0;	//This might be incorrect

//GLOBAL VARIABLES
/*
//UI
GLuint UIVBO;
GLuint UIEBO;
GLuint UIVAO;

//3D World
GLuint gameVBO;
GLuint gameEBO;
GLuint gameVAO;

//Shader Program
GLuint shaderProgramUI = 0;
GLuint shaderProgramGame = 0;

//Matrices
mat4 viewMatrix;

//UI
mat4 projMatrixUI;
mat4 worldMatrixUI;

mat4 projMatrixGame;
mat4 worldMatrixGame;

//Texture
GLuint texture = 0;
GLuint nameTexture = 0;
GLuint fontTexture = 0;
*/

//SDL Window
SDL_Window * window = NULL;
//pointers to our SDL Windows
//SDL Gl Context
SDL_GLContext glcontext = NULL;

//Constants to control window creation
//Window Width
const int WINDOW_WIDTH = 640;
//Window Height
const int WINDOW_HEIGHT = 480;

vec4 ambientLightColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
/* //Not needed anymore?
GLuint triangleEBO;
GLuint triangleVBO;
GLuint VAO;
GLuint shaderProgram = 0;
GLuint texture = 0;
*/

bool running = true;

std::vector<GameObject*> displayList;	//Vector of GameObjects
GameObject * mainCamera;
GameObject * mainLight;
PostProcessing postProcessor;

//Below not needed anymore
//float triangleData[] = { 0.0f, 1.0f, 0.0f, /*Top*/ -1.0f, -1.0f, 0.0f, /*Bottom Left*/ 1.0f, -1.0f, 0.0f}; /* Bottom Right*/	//Changed to the array below in Lab 2 -3D 4.2
//Vertex triangleData[] = {{0.0f, 1.0f, 0.0f, /*x,y,z*/ 1.0f, 0.0f, 1.0f}, /*r,g,b,a*/ {-1.0f, -1.0f, 0.0f, /*x,y,z*/ 0.0f, 1.0f, 0.0f, 1.0f}, /*r,g,b,a*/ {1.0f, -1.0f, 0.0f, /*x,y,z*/ 0.0f, 0.0f, 1.0f, 1.0f}}; /*r,g,b,a*/	//Creates a single triangle woth colored vertices
//^Changed to the array below in Lab 2 -3D 6.1 this format of code seems a lot more readable
//Changed the array below to contain triangle indices to be able to store the corners of a cube
/*
Vertex triangleData[] = {
	//Front Cube Face
	{ vec3(-0.5f, 0.5f, 0.5f), vec2(0.0f, 0.0f), vec4(1.0f, 0.0f, 1.0f, 1.0f) },// Top Left 	coord - x, y, z,  coords - u, v color - r, g, b, a
	{ vec3(-0.5f, -0.5f, 0.5f), vec2(0.0f, 1.0f), vec4(1.0f, 1.0f, 0.0f, 1.0f) },// Bottom Left 	coord - x, y, z,  coords - u, v color - r, g, b, a
	{ vec3(0.5f, -0.5f, 0.5f), vec2(1.0f, 1.0f), vec4(0.0f, 1.0f, 1.0f, 1.0f) }, //Bottom Right 	coord - x, y, z,  coords - u, v color - r, g, b, a
	{ vec3(0.5f, 0.5f, 0.5f), vec2(1.0f, 0.0f), vec4(1.0f, 0.0f, 1.0f, 1.0f) },// Top Right 	coord - x, y, z,  coords - u, v color - r, g, b, a
	//Back Cube Face
	{ vec3(-0.5f, 0.5f, -0.5f), vec2(0.0f, 0.0f), vec4(1.0f, 0.0f, 1.0f, 1.0f) },// Top Left 	coord - x, y, z, coords - u, v color - r, g, b, a
	{ vec3(-0.5f, -0.5f, -0.5f), vec2(0.0f, 1.0f), vec4(1.0f, 1.0f, 0.0f, 1.0f) },// Bottom Left 	coord - x, y, z,  coords - u, v color - r, g, b, a
	{ vec3(0.5f, -0.5f, -0.5f),vec2(1.0f, 1.0f), vec4(0.0f, 1.0f, 1.0f, 1.0f) }, //Bottom Right 	coord - x, y, z,  coords - u, v color - r, g, b, a
	{ vec3(0.5f, 0.5f, -0.5f), vec2(1.0f, 0.0f), vec4(1.0f, 0.0f, 1.0f, 1.0f) },// Top Right 	coord - x, y, z,  coords - u, v color - r, g, b, a
};

GLuint indices[] = {
	// Front
	0, 1, 2, 0, 3, 2,

	// Left
	4, 5, 1, 4, 1, 0,

	// Right
	3, 7, 2, 7, 6, 2,

	// Bottom
	1, 5, 2, 6, 2, 1,

	// Top
	5, 0, 7, 5, 7, 3,

	// Back
	4, 5, 6, 4, 7, 6
};


GLuint shaderProgram = 0;
*/	//Above not needed anymore


void CheckForErrors()
{
	GLenum error;
	do
	{
		error = glGetError();
	}
	while (error != GL_NO_ERROR);
}

//GLOBAL FUNCTIONS

void InitWindow(int width, int height, bool fullscreen)
{
	//Creates a window
	window = SDL_CreateWindow(
		"Lab 1",	//Window Title
		SDL_WINDOWPOS_CENTERED, //x position, centered
		SDL_WINDOWPOS_CENTERED, //y position, centered
		width,	//Width, in pixels
		height,	//Height, in pixels
		SDL_WINDOW_OPENGL//flags
		);
}

/*
void create2DScene()
{
	std::string fontPath = ASSET_PATH + FONT_PATH + "OratorStd.otf";
	nameTexture = loadTextureFromFont(fontPath, 28, "Test");

	GLuint vertexShaderProgram = 0;
	std::string vsPath = ASSET_PATH + SHADER_PATH + "vertexColourTextureVS.glsl";
	vertexShaderProgram = loadShaderFromFile(vsPath, VERTEX_SHADER);

	GLuint fragmentShaderProgram = 0;
	std::string fsPath = ASSET_PATH + SHADER_PATH + "vertexColourTextureFS.glsl";
	fragmentShaderProgram = loadShaderFromFile(fsPath, FRAGMENT_SHADER);

	shaderProgramUI = glCreateProgram();
	glAttachShader(shaderProgramUI, vertexShaderProgram);
	glAttachShader(shaderProgramUI, fragmentShaderProgram);
	glLinkProgram(shaderProgramUI);
	checkForLinkErrors(shaderProgramUI);

	//now we can delete the VS & FS Programs
	glDeleteShader(vertexShaderProgram);
	glDeleteShader(fragmentShaderProgram);

	glBindAttribLocation(shaderProgramUI, 0, "vertexPosition");
	glBindAttribLocation(shaderProgramUI, 1, "vertexTexCoords");
	glBindAttribLocation(shaderProgramUI, 2, "vertexColour");

	int width, height;

	glBindTexture(GL_TEXTURE_2D, nameTexture);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	Vertex spriteData[] = {
		//Front
		{ vec3(0.0, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) },// Top Left

		{ vec3(0.0f, height, 0.0f), vec2(0.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) },// Bottom Left

		{ vec3(width, height, 0.0f), vec2(1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }, //Bottom Right

		{ vec3(width, 0.0f, 0.0f), vec2(1.0f, 0.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f) }// Top Right

	};

	GLuint spriteIndices[] = {
		//front
		0, 1, 2,
		0, 3, 2,
	};

	glGenVertexArrays(1, &UIVAO);
	glBindVertexArray(UIVAO);
	//Create buffer
	glGenBuffers(1, &UIVBO);
	// Make the new VBO active
	glBindBuffer(GL_ARRAY_BUFFER, UIVBO);
	//Copy Vertex Data to VBO
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), spriteData, GL_STATIC_DRAW);

	//create buffer
	glGenBuffers(1, &UIEBO);
	//Make the EBO active
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, UIEBO);
	//Copy Index data to the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), spriteIndices, GL_STATIC_DRAW);

	//Tell the shader that 0 is the position element
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)sizeof(vec3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3)+sizeof(vec2)));
}
*/

/*
void create3DScene()
{
	std::string texturePath = ASSET_PATH + TEXTURE_PATH + "test.png";
	texture = loadTextureFromFile(texturePath);

	GLuint vertexShaderProgram = 0;
	std::string vsPath = ASSET_PATH + SHADER_PATH + "textureVS.glsl";
	vertexShaderProgram = loadShaderFromFile(vsPath, VERTEX_SHADER);

	GLuint fragmentShaderProgram = 0;
	std::string fsPath = ASSET_PATH + SHADER_PATH + "textureFS.glsl";
	fragmentShaderProgram = loadShaderFromFile(fsPath, FRAGMENT_SHADER);

	shaderProgramGame = glCreateProgram();
	glAttachShader(shaderProgramGame, vertexShaderProgram);
	glAttachShader(shaderProgramGame, fragmentShaderProgram);
	glLinkProgram(shaderProgramGame);
	checkForLinkErrors(shaderProgramGame);

	//now we can delete the VS & FS Programs
	glDeleteShader(vertexShaderProgram);
	glDeleteShader(fragmentShaderProgram);

	glBindAttribLocation(shaderProgramGame, 0, "vertexPosition");
	glBindAttribLocation(shaderProgramGame, 1, "vertexTexCoords");
	glBindAttribLocation(shaderProgramGame, 2, "vertexColour");

	Vertex triangleData[] = {
		//Front
		{ vec3(-0.5, 0.5f, 0.0f), vec2(0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },// Top Left

		{ vec3(-0.5f, -0.5f, 0.0f), vec2(0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) },// Bottom Left

		{ vec3(0.5f, -0.5f, 0.0f), vec2(1.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) }, //Bottom Right

		{ vec3(0.5f, 0.5f, 0.0f), vec2(1.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) }// Top Right

	};


	GLuint indices[] = {
		//front
		0, 1, 2,
		0, 3, 2,
	};

	glGenVertexArrays(1, &gameVAO);
	glBindVertexArray(gameVAO);
	//Create buffer
	glGenBuffers(1, &gameVBO);
	// Make the new VBO active
	glBindBuffer(GL_ARRAY_BUFFER, gameVBO);
	//Copy Vertex Data to VBO
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), triangleData, GL_STATIC_DRAW);

	//create buffer
	glGenBuffers(1, &gameEBO);
	//Make the EBO active
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gameEBO);
	//Copy Index data to the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), indices, GL_STATIC_DRAW);

	//Tell the shader that 0 is the position element
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)sizeof(vec3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3)+sizeof(vec2)));
}
*/
/*
void CleanUp2DScene()
{
	glDeleteTextures(1, &nameTexture);
	glDeleteProgram(shaderProgramUI);
	glDeleteBuffers(1, &UIVBO);
	glDeleteBuffers(1, &UIEBO);
	glDeleteVertexArrays(1, &UIVAO);
}
*/
/*
void CleanUp3DScene()
{
	glDeleteTextures(1, &texture);
	glDeleteProgram(shaderProgramUI);
	glDeleteProgram(shaderProgramGame);
	glDeleteBuffers(1, &gameVBO);
	glDeleteBuffers(1, &gameEBO);
	glDeleteVertexArrays(1, &gameVAO);
}
*/

/*
void createShader()
{
	GLuint vertexShaderProgram = 0;
	std::string vsPath = ASSET_PATH + SHADER_PATH + "/simpleVS.glsl";
	vertexShaderProgram = loadShaderFromFile(vsPath, VERTEX_SHADER);

	GLuint fragmentShaderProgram = 0;
	std::string fsPath = ASSET_PATH + SHADER_PATH + "/simpleFS.glsl";
	fragmentShaderProgram = loadShaderFromFile(fsPath, FRAGMENT_SHADER);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderProgram);
	glAttachShader(shaderProgram, fragmentShaderProgram);
	glLinkProgram(shaderProgram);

	glBindAttribLocation(shaderProgram, 0, "vertexPosition");
	glBindAttribLocation(shaderProgram, 1, "vertexTexCoords");
	glBindAttribLocation(shaderProgram, 2, "vertexColour");

	//Now we can delete the VS and FS Programs
	glDeleteShader(vertexShaderProgram);
	glDeleteShader(fragmentShaderProgram);
}
*/

/*
void initGeometry()
{
	//Create the buffer	//Changed from VBO to EBO in Lab 2 - 3D 7.4
	glGenBuffers(1, &triangleEBO);
	//Make the new EBO active
	glBindBuffer(GL_ARRAY_BUFFER, triangleEBO);
	//Copy the Index Data to EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//change to?//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(int), indices, GL_STATIC_DRAW);

}
*/
//createTexture();







void CleanUp()
{

	auto iter = displayList.begin();
	while (iter != displayList.end())
	{
		(*iter)->destroy();
		if ((*iter))
		{
			delete (*iter);
			(*iter) = NULL;
			iter = displayList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	displayList.clear();


	postProcessor.destroy();	//Clean up, reverse order
	/*
	if (physics){
		physics->destroy();
		delete physics;
		physics = NULL;
	}
	*/
	/*
	// clean up, reverse order!!!
	CleanUp2DScene();	//This gets rid of 2D elements
	CleanUp3DScene();	//This gets rid of 3D elements
	*/
	SDL_GL_DeleteContext(glcontext);	
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

//The function to initialise OpenGL
void initOpenGL()
{
	//Ask for version 3.2 of OpenGL to be used
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Create OpenGL Context
	glcontext = SDL_GL_CreateContext(window);
	//If this is NULL then there is an error in creating the context
	glewExperimental = GL_TRUE;
	//Initialise glew

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong */
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}

	/*
	if (!glcontext)
	{
		std::cout << "Error Creating OpenGL Context" << SDL_GetError() <<
			std::endl;
	}
	*/		//Not needed anymore

	//Smooth Shading
	glShadeModel(GL_SMOOTH);

	//Clear the Background to Black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Clear the depth buffer to 1.0
	glClearDepth(1.0f);

	//Enable Depth TEsting
	glEnable(GL_DEPTH_TEST);

	//The depth test to use
	glDepthFunc(GL_LEQUAL);

	//Turn on the best perspective correction
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	
}

//The Function to set and reset the viewport
void setViewport(int width, int height)
{
	/*
	//Screen Ratio
	GLfloat ratio;
	*/
	//Make sure the height is always above 1
	if (height == 0)
	{
		height = 1;
	}
	/*
	//Calculates the screen's ratio and setups the viewport
	ratio = (GLfloat)width / (GLfloat)height;
	*/
	//Setup viewport
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);


}

void Initialise()
{
	std::string vsPath = ASSET_PATH + SHADER_PATH + "/passThroughVS.glsl";
	std::string fsPath = ASSET_PATH + SHADER_PATH + "/boxFilterBlurFS.glsl";

	postProcessor.init(WINDOW_WIDTH, WINDOW_HEIGHT, vsPath, fsPath);

	mainCamera = new GameObject();
	mainCamera->setName("MainCamera");

	Transform *t = new Transform();
	t->setPosition(0.0f, 0.0f, 2.0f);
	mainCamera->setTransform(t);

	Camera * c = new Camera();
	c->setAspectRatio((float)(WINDOW_WIDTH / WINDOW_HEIGHT));
	c->setFOV(45.0f);
	c->setNearClip(0.1f);
	c->setFarClip(1000.0f);

	mainCamera->setCamera(c);
	displayList.push_back(mainCamera);

	mainLight = new GameObject();
	mainLight->setName("MainLight");

	t = new Transform();
	t->setPosition(0.0f, 0.0f, 0.0f);
	mainLight->setTransform(t);

	Light * light = new Light();
	mainLight->setLight(light);
	displayList.push_back(mainLight);

	//alternative sytanx
	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		(*iter)->init();
	}


	std::string modelPath = ASSET_PATH + MODEL_PATH + "armoredrecon.fbx";
	GameObject * go = loadFBXFromFile(modelPath);
	for (int i = 0; i < go->getChildCount(); i++)
	{
		Material * material = new Material();
		material->init();
		std::string vsPath = ASSET_PATH + SHADER_PATH + "/BumpmappingVS.glsl";
		std::string fsPath = ASSET_PATH + SHADER_PATH + "/BumpmappingFS.glsl";
		material->loadShader(vsPath, fsPath);

		std::string diffTexturePath = ASSET_PATH + TEXTURE_PATH + "/armoredrecon_diff.png";
		material->loadDiffuseMap(diffTexturePath);

		std::string specTexturePath = ASSET_PATH + TEXTURE_PATH + "/armoredrecon_spec.png";
		material->loadSpecularMap(specTexturePath);

		std::string bumpTexturePath = ASSET_PATH + TEXTURE_PATH + "/armoredrecon_N.png";
		material->loadBumpMap(bumpTexturePath);

		go->getChild(i)->setMaterial(material);
	}
	go->getTransform()->setPosition(2.0f, -2.0f, -6.0f);
	go->getTransform()->setRotation(0.0f, -40.0f, 0.0f);
	displayList.push_back(go);

	go = loadFBXFromFile(modelPath);
	for (int i = 0; i < go->getChildCount(); i++)
	{
		Material * material = new Material();
		material->init();
		std::string vsPath = ASSET_PATH + SHADER_PATH + "/ParallaxMappingVS.glsl";
		std::string fsPath = ASSET_PATH + SHADER_PATH + "/ParallaxMappingFS.glsl";
		material->loadShader(vsPath, fsPath);

		std::string diffTexturePath = ASSET_PATH + TEXTURE_PATH + "/armoredrecon_diff.png";
		material->loadDiffuseMap(diffTexturePath);

		std::string specTexturePath = ASSET_PATH + TEXTURE_PATH + "/armoredrecon_spec.png";
		material->loadSpecularMap(specTexturePath);

		std::string bumpTexturePath = ASSET_PATH + TEXTURE_PATH + "/armoredrecon_N.png";
		material->loadBumpMap(bumpTexturePath);

		std::string heightTexturePath = ASSET_PATH + TEXTURE_PATH + "/armoredrecon_Height.png";
		material->loadHeightMap(heightTexturePath);

		go->getChild(i)->setMaterial(material);
	}
	go->getTransform()->setPosition(-2.0f, -2.0f, -6.0f);
	go->getTransform()->setRotation(0.0f, -40.0f, 0.0f);
	displayList.push_back(go);

}

/*
void render2D()
{
	//Turn on Alpha Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(UIVAO);

	glUseProgram(shaderProgramUI);
	GLint MVPLocation = glGetUniformLocation(shaderProgramUI, "MVP");
	mat4 MVP = projMatrixUI*viewMatrix*worldMatrixUI;
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));

	GLint texture0Location = glGetUniformLocation(shaderProgramUI, "texture0");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, nameTexture);
	glUniform1i(texture0Location, 0);

	//Actually draw the triangle, giving the number of vertices provided
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}
*/

/*
void render3D()
{
	glBindVertexArray(gameVAO);

	glUseProgram(shaderProgramGame);
	GLint MVPLocation = glGetUniformLocation(shaderProgramGame, "MVP");
	mat4 MVP = projMatrixGame*viewMatrix*worldMatrixGame;
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));

	GLint texture0Location = glGetUniformLocation(shaderProgramGame, "texture0");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(texture0Location, 0);

	//Actually draw the triangle, giving the number of vertices provided
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
*/

void renderGameObject(GameObject * pObject)
{
	if (!pObject)
		return;

	pObject->render();

	Mesh * currentMesh = pObject->getMesh();
	Transform * currentTransform = pObject->getTransform();
	Material * currentMaterial = pObject->getMaterial();

	if (currentMesh && currentMaterial && currentTransform)
	{
		currentMaterial->bind();
		currentMesh->bind();

		GLint MVPLocation = currentMaterial->getUniformLocation("MVP");
		GLint ModelLocation = currentMaterial->getUniformLocation("Model");
		GLint ambientMatLocation = currentMaterial->getUniformLocation("ambientMaterialColour");
		GLint ambientLightLocation = currentMaterial->getUniformLocation("ambientLightColour");
		GLint diffuseMatLocation = currentMaterial->getUniformLocation("diffuseMaterialColour");
		GLint diffuseLightLocation = currentMaterial->getUniformLocation("diffuseLightColour");
		GLint lightDirectionLocation = currentMaterial->getUniformLocation("lightDirection");
		GLint specularMatLocation = currentMaterial->getUniformLocation("specularMaterialColour");
		GLint specularLightLocation = currentMaterial->getUniformLocation("specularLightColour");
		GLint specularpowerLocation = currentMaterial->getUniformLocation("specularPower");
		GLint cameraPositionLocation = currentMaterial->getUniformLocation("cameraPosition");
		GLint diffuseTextureLocation = currentMaterial->getUniformLocation("diffuseMap");
		GLint specTextureLocation = currentMaterial->getUniformLocation("specMap");
		GLint bumpTextureLocation = currentMaterial->getUniformLocation("bumpMap");
		GLint heightTextureLocation = currentMaterial->getUniformLocation("heightMap");

		Camera * cam = mainCamera->getCamera();
		Light* light = mainLight->getLight();


		mat4 MVP = cam->getProjection()*cam->getView()*currentTransform->getModel();
		mat4 Model = currentTransform->getModel();

		vec4 ambientMaterialColour = currentMaterial->getAmbientColour();
		vec4 diffuseMaterialColour = currentMaterial->getDiffuseColour();
		vec4 specularMaterialColour = currentMaterial->getSpecularColour();
		float specularPower = currentMaterial->getSpecularPower();

		vec4 diffuseLightColour = light->getDiffuseColour();
		vec4 specularLightColour = light->getSpecularColour();
		vec3 lightDirection = light->getDirection();

		vec3 cameraPosition = mainCamera->getTransform()->getPosition();

		glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(Model));
		glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
		glUniform3fv(cameraPositionLocation, 1, glm::value_ptr(cameraPosition));

		glUniform4fv(ambientMatLocation, 1, glm::value_ptr(ambientMaterialColour));
		glUniform4fv(diffuseMatLocation, 1, glm::value_ptr(diffuseMaterialColour));
		glUniform4fv(specularMatLocation, 1, glm::value_ptr(specularMaterialColour));
		glUniform1f(specularpowerLocation, specularPower);

		glUniform4fv(ambientLightLocation, 1, glm::value_ptr(ambientLightColour));
		glUniform4fv(diffuseLightLocation, 1, glm::value_ptr(diffuseLightColour));
		glUniform4fv(specularLightLocation, 1, glm::value_ptr(specularLightColour));
		glUniform3fv(lightDirectionLocation, 1, glm::value_ptr(lightDirection));

		glUniform1i(diffuseTextureLocation, 0);
		glUniform1i(specTextureLocation, 1);
		glUniform1i(bumpTextureLocation, 2);
		glUniform1i(heightTextureLocation, 3);

		glDrawElements(GL_TRIANGLES, currentMesh->getIndexCount(), GL_UNSIGNED_INT, 0);
	}

	for (int i = 0; i < pObject->getChildCount(); i++)
	{
		renderGameObject(pObject->getChild(i));
	}
}

//Function to draw/render
void render()
{
	/*
	//Set the clear color(background)
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Make the new VBO active. Repeat here as a sanity check( may have changed since initialisation)
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);

	glUseProgram(shaderProgram);	//This line and the three below might have to go below the EBO linkBuffer
	//Gets the location of the MVP, Creates a combined Model View Projection Matrix and then sends it to the shader
	GLint MVPLocation = glGetUniformLocation(shaderProgram, "MVP");
	mat4 MVP = projMatrix * viewMatrix * worldMatrix;
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));

	//Tells the Shader that 0 is the position element
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3)+sizeof(vec2)));
	glEnableVertexAttribArray(2);
	glVertexArrayAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3)+sizeof(vec2)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)sizeof(vec3));
	
	GLint texture0Location = glGetUniformLocation(shaderProgram, "texture0");	//This gets the texture location and sends the location of the texture to the shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Changed in Lab 2 - 3D to allow to draw both EBOs and VBOs
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//required to swap the back and front buffer
	SDL_GL_SwapWindow(window);
	*/		//Not needed anymore
	postProcessor.bind();

	//old imediate mode!
	//Set the clear colour(background)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	//clear the colour and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//Alterantive Syntax
	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		renderGameObject((*iter));
	}

	/*
	render2D();		//Calls the render 2D function
	render3D();		//Calls the render 3D function
	*/
	//Switching to the normal framebuffer
	postprocessor.preDraw();
	//Grab stuff from the shader
	GLint colourFilterLocation = postProcessor.getUniformVariableLocation("colourFilter");
	glUniformMatrix3fv(colourFilterLocation, 1, GL_FALSE, glm::value_ptr(SEPIA_FILTER));

	//Draw
	postProcessor.Draw();

	//Post Draw
	postProcessor.postDraw();

	SDL_GL_SwapWindow(window);

}

/*
void update2DScene()
{
	viewMatrix = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projMatrixUI = glm::ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, 0.1f, 100.0f);
	worldMatrixUI = glm::translate(mat4(1.0f), vec3(10.0f, 10.0f, -10.0f));
}
*/

/*
void update3DScene()
{
	projMatrixGame = glm::perspective(45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	viewMatrix = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	worldMatrixGame = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, -10.0f));
}
*/


//Function to update the game state
void update()
{
	//Alternataive syntax
	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		(*iter)->update();
	}

	/*
	update2Dscene();
	update3Dscene();
	*/
	/*
	projMatrix = glm::perspective(45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	viewMatrix = glm::lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	worldMatrix = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	*/ //^Not needed anymore
}

/*
void createTexture()
{
	std::string texturePath = ASSET_PATH + TEXTURE_PATH + " /texture.png";
	texture = loadTextureFromFile(texturePath);
}
*/
/*
void createFontTexture()
{
std:string fontPath = ASSET_PATH + FONT_PATH + " /font.otf";	//This line may be incorrect
	font = loadFontFromFile(fontPath);
}
*/
/*
void initGeometryFromTexture(GLuint textureID)
{
int width, height;

glBindTexture(GL_TEXTURE_2D, textureID);
glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
}
//I'm fairly sure this shouldn't be here sfter refactoring the code and creating separate renderers.
*/

//Main Method - Entry Point
int main(int argc, char* args[])
{
	
	// Setup asset path, no real work required for Windows. Mac needs to load assets from a bundle
	// ----------------------------------------------------------------------------
	// http://stackoverflow.com/questions/516200/relative-paths-not-working-in-xcode-c
	// This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
	
#ifdef __APPLE__
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	char path[PATH_MAX];
	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
	{
		// error!
	}
	CFRelease(resourcesURL);

	chdir(path);
	std::cout << "Current Path: " << path << std::endl;
#endif
	
	
	//Init everthing - SDL, if it is nonzero we have a problem
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "ERROR SDL_Init" << SDL_GetError() << std::endl;

		return -1;
	}

	int imageInitFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int returnInitFlags = IMG_Init(imageInitFlags);
	if (((returnInitFlags)& (imageInitFlags)) != imageInitFlags)
	{
		std::cout << "ERROR SDL_Image Init " << IMG_GetError() << std::endl;
		// handle error
	}

	if (TTF_Init() == -1)
	{
		std::cout << "ERROR TFF_Init: " << TTF_GetError();
	}
	/*
	//The code below initialises SDL Image for jpg and pngs
	int imageInitFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int returnInitFlags = IMG_Init(imageInitFlags);
	if (((returnInitFlags)& (imageInitFlags)) != imageInitFlags)
	{
		std::cout << "ERROR SDL_Image Init" << IMG_GetError() << std::endl;
		//Handles and gets Error
	}
	*/
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false);
	//Call our InitOpenGL Function
	initOpenGL();
	CheckForErrors();
	//Set our viewport
	setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

	/*
	create2DScene();
	create3DScene();
	*/
	Initialise();
	/*
	GLenum error;
	do{
		error = glGetError();
	} 
	while (error != GL_NO_ERROR);
	*/
	/*
	//Call out InitGeometry Function
	initGeometry();
	*/

	//createShader();
	//Value to hold the event generated by SDL
	SDL_Event event;
	//Game Loop
	while (running)
	{
		//Whike we still have events in the queue
		while (SDL_PollEvent(&event))
		{
			//Get event Type
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
			{
				//set out boolean value, which controls the game loop to false
				running = false;
			}
		}
		update();
		render();
	}

	CleanUp();

	return 0;
}





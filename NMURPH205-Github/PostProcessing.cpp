#include "PostProcessing.h"
#include "Shader.h"

PostProcessing::PostProcessing()
{
	m_FrameBufferObject = 0;
	m_DepthBufferObject = 0;
	m_FBOTexture = 0;
	m_FullScreenVBO = 0;
	m_PostProcessingProgram = 0;
}

PostProcessing::~PostProcessing()
{

}

void PostProcessing::init(int width, int height, std::string& vertexShaderFilename, std::string& fragmentShaderFilename)
{
	createFramebuffer(width, height);
	createShader(vertexShaderFilename, fragmentShaderFilename);
	createFullScreenQuad();
}

void PostProcessing::createShader(std::string& vertexShaderFilename, std::string& fragmentShaderFilename)
{
	GLuint vertexShaderProgram = 0;
	vertexShaderProgram = loadShaderFromFile(vertexShaderFilename, VERTEX_SHADER);

	GLuint fragmentShaderProgram = 0;
	fragmentShaderProgram = loadShaderFromFile(fragmentShaderFilename, FRAGMENT_SHADER);

	m_PostProcessingProgram = glCreateProgram();
	glAttachShader(m_PostProcessingProgram, vertexShaderProgram);
	glAttachShader(m_PostProcessingProgram, fragmentShaderProgram);
	glLinkProgram(m_PostProcessingProgram);
	checkForLinkErrors(m_PostProcessingProgram);

	//now we can delete the VS & FS Programs
	glDeleteShader(vertexShaderProgram);
	glDeleteShader(fragmentShaderProgram);

	glBindAttribLocation(m_PostProcessingProgram, 0, "vertexPosition");

}

void PostProcessing::createFullScreenQuad()
{
	/* init_resources */
	GLfloat vertices[] = {
		-1, -1,
		1, -1,
		-1, 1,
		1, 1,
	};
	glGenBuffers(1, &m_FullScreenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_FullScreenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PostProcessing::createFramebuffer(int width, int height)
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_FBOTexture);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	/* Depth buffer */
	glGenRenderbuffers(1, &m_DepthBufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	/* Framebuffer to link everything together */
	glGenFramebuffers(1, &m_FrameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBOTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBufferObject);
	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		//something went wrong
	}

}

void PostProcessing::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);
}

void PostProcessing::preDraw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FBOTexture);

	glUseProgram(m_PostProcessingProgram);
	GLint textureLocation = glGetUniformLocation(m_PostProcessingProgram, "texture0");
	glUniform1i(textureLocation, 0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_FullScreenVBO);
	glVertexAttribPointer(
		0,  // attribute
		2,                  // number of elements per vertex, here (x,y)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
		);
}

void PostProcessing::postDraw()
{
	glDisableVertexAttribArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessing::draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


}

void PostProcessing::destroy()
{
	glDeleteBuffers(1, &m_FullScreenVBO);
	glDeleteProgram(m_PostProcessingProgram);
	glDeleteRenderbuffers(1, &m_DepthBufferObject);
	glDeleteTextures(1, &m_FBOTexture);
	glDeleteFramebuffers(1, &m_FrameBufferObject);
}

GLint PostProcessing::getUniformVariableLocation(const std::string& name)
{
	return glGetUniformLocation(m_PostProcessingProgram, name.c_str());
}
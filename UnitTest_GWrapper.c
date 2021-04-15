#include "GWrapper/GWrapper.h"
#include <stdio.h>
#include "GL/gl.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "ExternLib/Bin/stb-master/stb_image.h"

#include "/usr/include/GL/glx.h"
#include "/usr/include/GL/glxext.h"

PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLCREATEPROGRAMPROC glCreateProgram;

PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;

PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;

const GLchar *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\n";
const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"

"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";
int Texture(int *width, int *height, int *nrChannels, char Image[256]){
	unsigned int texture;
	unsigned char *data = stbi_load(Image, &width, &height, &nrChannels, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	return 0;
}

main(){
	glfwInit();
	GWrapperInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//CALL TO TELL OPENGL TO DO MSAA SAMPLING WITH THE GIVEN NUMBER
	glfwWindowHint(GLFW_SAMPLES, 1024);
	
	GLFWwindow * window = glfwCreateWindow(800, 600, "A", NULL, NULL);

	glfwMakeContextCurrent(window);

	glViewport(0, 0, 800, 600);

	
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	float transform[4][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0.45, 0.15, 0, 1}
	};



	//ansi c uses row major, therefore this will be stored in row major order instead of column major order (RIP the common practice of column major order in computer graphics)
	//13, 14, 15 are the translation parts of the matrix	
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	




	int fragmentShader, vertexShader;
	
	vertexShadR(&vertexShader, vertexShaderSource);

	/*vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	*/
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("%s", infoLog);
	}

	fragmentShadR(&fragmentShader, fragmentShaderSource);
		
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("%s", infoLog);
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	//glDeleteShader(vertexShader);
	//glDeleteShader(fragmentShader);




	unsigned int transformloc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformloc, 1, GL_FALSE, &transform);




	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	while(!glfwWindowShouldClose(window)){
		glfwPollEvents();
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
			transform [3][0] = transform [3][0] + 0.05f;
		}
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
			transform [3][0] = transform [3][0] - 0.05f;
		}
		if(glfwGetKey(window, GLFW_KEY_W)){
			transform [3][1] = transform [3][1] + 0.05f;
		}
		if(glfwGetKey(window, GLFW_KEY_S)){
			transform [3][1] = transform [3][1] - 0.05f;
		}
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	
		glUniformMatrix4fv(transformloc, 1, GL_FALSE, &transform);	
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
	}
}

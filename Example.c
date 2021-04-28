#include "GWrapper/GWrapper.h"
#include <stdio.h>
#include <unistd.h>
#include "GL/gl.h"
#include "GLFW/glfw3.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "ExternLib/Bin/stb-master/stb_image.h"

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

PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

PFNGLUNIFORM2FPROC glUniform2f;
const GLchar *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"TexCoord = aTexCoord;\n"
"}\n";
const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"uniform vec2 TexCoordShift;\n"
"uniform vec2 CoordEdit;\n"
"void main()\n"
"{\n"
"	vec2 Coord;\n"
"	Coord = TexCoord;\n"
"	Coord = Coord*CoordEdit;\n"
"	if(CoordEdit.x < 0){\n"
"		Coord.x = Coord.x + (1.0/7.0) + 0.0001;\n"
"	}\n"
"	FragColor = texture(ourTexture, (Coord + TexCoordShift));\n"
"}\n";
//having an if statement here should be okay as it is doing a very lightweight
//calculation supposedly.
//https://stackoverflow.com/questions/4176247/efficiency-of-branching-in-shaders//read up more and make a decision on whether to have it done every time uniform2f function is called on the cpu or in the shader on the GPU, here it should be relatively safe, though be careful in future decisions as just making a function to do it on the cpu would most likely be safer.

int main(){
	GWrapperInit();
	printf("%d\n",glfwInit());

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//CALL TO TELL OPENGL TO DO MSAA SAMPLING WITH THE GIVEN NUMBER
	//glfwWindowHint(GLFW_SAMPLES, 4);
	//^ do not use sampling with pixel art, bad idea
	GLFWwindow * window = glfwCreateWindow(800, 800, "A", NULL, NULL);
	if(window == NULL){
		printf("Failed to create window\n");
		return -1;
	}
	glfwMakeContextCurrent(window);

	glViewport(0, 0, 800, 800);

	
	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_LINE_SMOOTH);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, (1.0/7.0)+ 0.0001, 0.0f,
		0.5f, 0.5f, 0.0f, (1.0/7.0)+0.0001, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
	};
	//adding very small value to texcoord x position to ensure that it really does get the sampling within range to count a pixel, ensures no vibrating
	float transform[4][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, -0.5, 0, 1}
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
	ProgCrtLnk(&shaderProgram, fragmentShader, vertexShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);




	unsigned int transformloc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformloc, 1, GL_FALSE, &transform[0][0]);
	unsigned int TexCoordShiftLoc = glGetUniformLocation(shaderProgram, "TexCoordShift");
	glUniform2f(TexCoordShiftLoc, 0, 0);
	unsigned int CoordEditLoc = glGetUniformLocation(shaderProgram, "CoordEdit");
	int xyz = 1;
	glUniform2f(CoordEditLoc, xyz, 1);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);



	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//DO NOT FORGET TO ADD x * sizeof(float) for where to start, otherwise it will start at the wrong digit and get a wacky number to load the texture with
	glEnableVertexAttribArray(1);
	
	//Research Blending more when possible	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//remember to enable blending with transparent images, as this allows colors with less than 255 alpha to blend with background colors, basic explanation in this reddit question forum: https://www.reddit.com/r/opengl/comments/5ups85/struggling_with_png_transparency_via_stb_image/

	/*unsigned int texture;
	glGenTextures(1, &texture);
	
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(True);
	unsigned char *data = stbi_load("Atlas.png", &width, &height, &nrChannels, 0);
	if (data){
    		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    		glGenerateMipmap(GL_TEXTURE_2D);
	}	

	stbi_image_free(data);
	*/
	unsigned int texture;
	_LOADIMAGE(&texture, "Atlas.png");



	int second = 0;
	float TexAtlas1 = 0.0f;	
	while(!glfwWindowShouldClose(window)){
		glfwPollEvents();
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
			transform [3][0] = transform [3][0] + 0.05f;
		}
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
			transform [3][0] = transform [3][0] - 0.05f;
			//update texture
		}
		if(glfwGetKey(window, GLFW_KEY_W)){
			if(second == 1){
				if(roundf(TexAtlas1 * 1000) == roundf((6.0/7.0)*1000)){
					TexAtlas1 = (0.0/7.0);
				}
				TexAtlas1 = TexAtlas1 + (1.0/7.0 + 0.00001f);
				second = 0;
			}
			else{
				second = 1;
			}
			glUniform2f(TexCoordShiftLoc,TexAtlas1, 0);
			//transform [3][1] = transform [3][1] + 0.05f;
			usleep(41000);
		}
		else{
			TexAtlas1 = 0.0f;
			glUniform2f(TexCoordShiftLoc, TexAtlas1, 0);
		}
		if(glfwGetKey(window, GLFW_KEY_S)){
			xyz = -xyz;
			glUniform2f(CoordEditLoc, xyz, 1);	
			usleep(100000);
			//glUniform2f(TexCoordShiftLoc, 0.0f, 0);
			//transform [3][1] = transform [3][1] - 0.05f;
		}
		//only need to do this once I believe
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	
		glUniformMatrix4fv(transformloc, 1, GL_FALSE, &transform[0][0]);	
		//glBindTexture(GL_TEXTURE_2D, texture);
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		FanDraw(texture, VAO, 4);
		glfwSwapBuffers(window);
	}
	return 0;
}

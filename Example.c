#include "GWrapper/GWrapper.h"
#include "MWrapper/MWrapper.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "GL/gl.h"
#include "GLFW/glfw3.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "ExternLib/Bin/stb-master/stb_image.h"

#include "/usr/include/GL/glx.h"
#include "/usr/include/GL/glxext.h"
#include <math.h>

const GLchar *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"	gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"	TexCoord = aTexCoord;\n"
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

//in future, simplify to local variables with pointers passed, etc, for now global variables will be tolerated until better method found for key_callback, such as perhaps setting up custom one

struct Object ob1;

#define GRAVITY 0.0009700f;

int key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	//activated only when a new change in key input has occured
	if (key == GLFW_KEY_E && action == GLFW_PRESS && ob1.colground){
		//apply vertical acceleration
		ob1.velocity.y = 0.1f;
		ob1.colground = 0;
		//when jumping, start with base velocity from start of jump, similar to throwing an object, has starting velocity, with gravity acting as the only real change in acceleration
	}
}

int main(){
	GWrapperInit();
	if(!glfwInit()){
		printf("FAILED TO INITIALIZE GLFW\n");
		return -1;
	}
	ob1.colground = 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//CALL TO TELL OPENGL TO DO MSAA SAMPLING WITH THE GIVEN NUMBER
	//glfwWindowHint(GLFW_SAMPLES, 4);
	//^ do not use sampling with pixel art, bad idea
	GLFWwindow * window = glfwCreateWindow(800, 800, "A", NULL, NULL);
	if(window == NULL){
		printf("FAILED TO CREATE WINDOW\n");
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
	//adding very small value to texcoord x position to ensure that it really does get the sampling within range to count a pixel, ensures no vibrating of texture

	float *transform = MatIdent(4);
	transform [13] = -0.5f; 
	//ansi c uses row major, therefore this will be stored in row major order instead of column major order (RIP the common practice of column major order in computer graphics)
	//13, 14, 15 are the translation parts of the matrix	
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	




	int fragmentShader, vertexShader;
	
	vertexShadR(&vertexShader, vertexShaderSource);

	int success;
	char infoLog[512];
	/*
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("%s", infoLog);
	}
*/
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
	glUniformMatrix4fv(transformloc, 1, GL_FALSE, transform);
	
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

	
	unsigned int texture;
	_LOADIMAGE(&texture, "Atlas.png");



	ob1.velocity.x = 0;
	ob1.velocity.y = 0;
	int second = 0;
	float TexAtlas1 = 0;
	clock_t lastframe = 0, currentframe = 0;
	int xmov = 0, negxmov = 0;
	float speed = 1.0f;

	//if button is released, set currentframe to 0
	//if button is pushed, continue using clock() to update the current frame, then compare the two, then update lastframe with
	//current frame value
	//check if key is continued to be held by having a variable that is set at the end of the action of the key, and if it is seen as pressed before then, then it is held, use else statement to set it to zero
	while(!glfwWindowShouldClose(window)){
		//update acceleration, then velocity, then player position
		ob1.accel.y -= GRAVITY;
		if(ob1.colground){
			ob1.accel.y = 0;
		}
		ob1.velocity.y += ob1.accel.y;
		transform [13] += ob1.velocity.y;
		if(transform [13] < -0.5){
			ob1.colground = 1;
			transform [13] = -speed;
		}
		printf("%f\n", transform [13]);
		glfwPollEvents();
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
			currentframe = clock();
			if(xmov == 1){
				//currentframe = clock();
				transform [12] += speed * calcDeltaTime(lastframe, currentframe);
				printf("movement: %f\n", (speed * (float)(speed * (float)((float)(currentframe - lastframe) / CLOCKS_PER_SEC))));
				printf("frame time: %f\n", (float)((float)(currentframe - lastframe) / CLOCKS_PER_SEC));
			}
			else{
				//currentframe = clock();
			}
			lastframe = currentframe;
			xmov = 1;
			printf("TransfrmMat: %f\n", transform[12]);
		}
		else{
			xmov = 0;
		}
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
			transform [12] = transform [12] - speed;
			//update texture
		}
		if(glfwGetKey(window, GLFW_KEY_W)){
			if(second == 1){
				//to apply it as twos
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
			//usleep(41000);
			for(int i = 0; i < 40000; i++){
			
			}
			//implement clock readings to get about how many micro seconds have passed
			//since last frame
		}
		else{
			TexAtlas1 = 0.0f;
			glUniform2f(TexCoordShiftLoc, TexAtlas1, 0);
			//set animation movement to zero
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
	
		glUniformMatrix4fv(transformloc, 1, GL_FALSE, transform);	
		//glBindTexture(GL_TEXTURE_2D, texture);
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		FanDraw(texture, VAO, 4);
		glfwSwapBuffers(window);
	}
	return 0;
}

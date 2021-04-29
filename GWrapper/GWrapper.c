//#include "GL/glew.h"
#include "GWrapper.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include "GL/gl.h"
#include "/usr/include/GL/glx.h"
#include "/usr/include/GL/glxext.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../ExternLib/Bin/stb-master/stb_image.h"



jmp_buf errhandl;
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

void segfaultHandler(int seg_num){
	//signal(SIGSEGV, segfaultHandler);
	longjmp(errhandl, 1);
}

int _BaseShadR(int * Shader, const char *shaderSource, int LINE, const char *FUNC, const char *cFILE, int Type){
	if(setjmp(errhandl)){
		printf("SEGMENTATION FAULT at shader creation and compilation: %s : %s : %d :: Ignoring function call\n", cFILE, FUNC, LINE);
		return -1;
	}

	int err = 0;
	int Statuse = 0;
	char infoLog[512];
	
	
	*Shader = glCreateShader(Type);
	glShaderSource(*Shader, 1, &shaderSource, NULL);
	glCompileShader(*Shader);
	
	
	glGetShaderiv(*Shader, GL_COMPILE_STATUS, &Statuse);
	
	if(Statuse == GL_FALSE){
		glGetShaderInfoLog(*Shader, 512, NULL, infoLog);
		printf("SHADER SOURCE ERROR: %s : %s : %d :: %s", cFILE, FUNC, LINE, infoLog);
		glDeleteShader(*Shader);
		return -1;
	}
}
int _BaseProgram(unsigned int * shaderProgram, int FragmentShader, int VertexShader, const char *cFILE, const char *FUNC, int LINE){
	if(setjmp(errhandl)){
		printf("SEGMENTATION FAULT at program creation and linking: %s : %s : %d :: Ignoring function call\n", cFILE, FUNC, LINE);
	}
	
	*shaderProgram = glCreateProgram();
	glAttachShader(*shaderProgram, FragmentShader);
	glAttachShader(*shaderProgram, VertexShader);
	glLinkProgram(*shaderProgram);
	glUseProgram(*shaderProgram);
	return 0;
}
int _LOADIMAGE(unsigned int *texture, char name[]){
	//for now, will use STB for ease of decoding image files, hopefully in the future I'll either be taught how or figure it out
	glGenTextures(1, texture);
	
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi_set_flip_vertically_on_load(True);
	int width, height, nrChannels;
	unsigned char *data = stbi_load(name, &width, &height, &nrChannels, 0);
	if(!data){
		printf("failed to load texture");
		return -1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}


int _DRAW(unsigned int texture, unsigned int VAO, int Vertices, int TYPE){
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawArrays(TYPE, 0, Vertices);
}


 int _CLEARCOLOR(float R, float G, float B, float H){
 	glClearColor(R, G, B, H);
 	glClear(GL_COLOR_BUFFER_BIT);
 }
int GWrapperInit(){
	signal(SIGSEGV, segfaultHandler);
	glCreateShader = (PFNGLCREATESHADERPROC)glXGetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)glXGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)glXGetProcAddress("glCompileShader");
	glGenBuffers = (PFNGLGENBUFFERSPROC)glXGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddress("glBufferData");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)glXGetProcAddress("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)glXGetProcAddress("glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)glXGetProcAddress("glLinkProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)glXGetProcAddress("glUseProgram");
	glDeleteShader = (PFNGLDELETESHADERPROC)glXGetProcAddress("glDeleteShader");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddress("glVertexAttribPointer");
	
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress("glEnableVertexAttribArray");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress("glGenVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glXGetProcAddress("glBindVertexArray");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)glXGetProcAddress("glGetShaderiv");
	
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glXGetProcAddress("glGetUniformLocation");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glXGetProcAddress("glUniformMatrix4fv");
	
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glXGetProcAddress("glGetShaderInfoLog");
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)glXGetProcAddress("glGenerateMipmap");
	glUniform2f = (PFNGLUNIFORM2FPROC)glXGetProcAddress("glUniform2f");
}
//Basic shader construction
//Vertex Shader Initialization and compilation


//#include "GL/glew.h"
#include "GWrapper.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include "GL/gl.h"
#include "/usr/include/GL/glx.h"
#include "/usr/include/GL/glxext.h"

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

void segfaultHandler(int seg_num){
	//signal(SIGSEGV, segfaultHandler);
	longjmp(errhandl, 1);
}

int _BaseShadR(int * Shader, const char *shaderSource, int LINE, const char *FUNC, const char *cFILE, int Type){
	if(setjmp(errhandl)){
		printf("SEGMENTATION FAULT at fragment creation and compilation: %s : %s : %d :: Ignoring function call\n", cFILE, FUNC, LINE);
		return -1;
	}

	signal(SIGSEGV, segfaultHandler);
	int err = 0;
	while((err = glGetError()) != GL_NO_ERROR){
		//clear out opengl errors before starting
	}
	*Shader = glCreateShader(Type);
	glShaderSource(*Shader, 1, &shaderSource, NULL);
	glCompileShader(*Shader);
	//NVM, DON'T FOLLOW BELOWS METHOD
	//use glGetShaderInfoLog to get info on if correctly compiled
	fragshadererr:
	while((err = glGetError()) != GL_NO_ERROR){
		printf("ERR: compilation unsucessful during creation of Fragment Shader: %s : %s : %d\nReason due to ERR: %d\n", cFILE, FUNC, LINE, err);
	}
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
}
//Basic shader construction
//Vertex Shader Initialization and compilation


#define fragmentShadR(VertexShader, input_Vertex_Shader) _BaseShadR(VertexShader, input_Vertex_Shader, __LINE__, __func__, __FILE__, GL_FRAGMENT_SHADER);

#define vertexShadR(VertexShader, input_Vertex_Shader) _BaseShadR(VertexShader, input_Vertex_Shader, __LINE__, __func__, __FILE__, GL_VERTEX_SHADER);

#define ProgCrtLnk(shaderProgram, FragmentShader, VertexShader) _BaseProgram(shaderProgram, FragmentShader, VertexShader, __FILE__, __func__, __LINE__)

#define FanDraw(texture, VAO, vertices) _DRAW(texture, VAO, vertices, GL_TRIANGLE_FAN);
int GWrapperInit();
int _BaseShadR(int * Shader, const char *shaderSource, int LINE, const char *FUNC, const char *_FILE, int Type);
int _BaseProgram(unsigned int * shaderProgram, int FragmentShader, int VertexShader, const char *cFILE, const char *FUNC, int LINE);
int _DRAW(unsigned int texture, unsigned int VAO, int Vertices, int TYPE);
int _LOADIMAGE(unsigned int *texture, char name[]);

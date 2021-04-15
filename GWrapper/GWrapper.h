#define fragmentShadR(VertexShader, input_Vertex_Shader) _BaseShadR(VertexShader, input_Vertex_Shader, __LINE__, __func__, __FILE__, GL_FRAGMENT_SHADER);

#define vertexShadR(VertexShader, input_Vertex_Shader) _BaseShadR(VertexShader, input_Vertex_Shader, __LINE__, __func__, __FILE__, GL_VERTEX_SHADER);
int GWrapperInit();
int _BaseShadR(int * Shader, const char *shaderSource, int LINE, const char *FUNC, const char *_FILE, int Type);

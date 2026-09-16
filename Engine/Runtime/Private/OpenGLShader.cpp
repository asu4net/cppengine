// @Note: This translation unit gets compiled inside "Runtime/GraphicsStorage.cpp"

OpenGLShader::OpenGLShader(const ShaderParams& params)
{
  LOG_INFO("OpenGL Shader created.");
}

OpenGLShader::~OpenGLShader()
{
  LOG_INFO("OpenGL Shader destroyed.");
}

void OpenGLShader::Use()
{
  LOG_INFO("OpenGL Shader used.");
}

#ifdef ENGINE_OPENGL
GRAPHICS_STORAGE_REGISTER(GraphicsInstance, OpenGLGraphicsInstance, GraphicsInstanceParams)
GRAPHICS_STORAGE_REGISTER(GraphicsDevice,   OpenGLGraphicsDevice,   GraphicsDeviceParams)
GRAPHICS_STORAGE_REGISTER(GraphicsContext,  OpenGLGraphicsContext,  GraphicsContextParams)
GRAPHICS_STORAGE_REGISTER(VertexBuffer,     OpenGLVertexBuffer,     VertexBufferParams)
GRAPHICS_STORAGE_REGISTER(Shader,           OpenGLShader,           ShaderParams)
#elif  ENGINE_D3D11
GRAPHICS_STORAGE_REGISTER(GraphicsInstance, D3D11GraphicsInstance,  GraphicsInstanceParams)
GRAPHICS_STORAGE_REGISTER(GraphicsDevice,   D3D11GraphicsDevice,    GraphicsDeviceParams)
GRAPHICS_STORAGE_REGISTER(GraphicsContext,  D3D11GraphicsContext,   GraphicsContextParams)
GRAPHICS_STORAGE_REGISTER(VertexBuffer,     D3D11VertexBuffer,      VertexBufferParams)
GRAPHICS_STORAGE_REGISTER(Shader,           D3D11Shader,            ShaderParams)
#else
#error "Missing Graphics API implementation."
#endif

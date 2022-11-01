/* copied from wglext.h */
typedef BOOL(WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef HGLRC(WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
#define WGL_SAMPLE_BUFFERS_ARB            0x2041
#define WGL_SAMPLES_ARB                   0x2042
#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_ALPHA_BITS_ARB                0x201B
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_SWAP_EXCHANGE_ARB             0x2028
#define WGL_SWAP_METHOD_ARB               0x2007
#define WGL_SWAP_COPY_ARB                 0x2029

/* copied from glcorearb.h */
#define APIENTRYP APIENTRY *
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_ARRAY_BUFFER                   0x8892
#define GL_STATIC_DRAW                    0x88E4
typedef char GLchar;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;
typedef GLuint(APIENTRYP PFNGLCREATESHADERPROC)(GLenum type);
typedef void (APIENTRYP PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (APIENTRYP PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void (APIENTRYP PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef GLuint(APIENTRYP PFNGLCREATEPROGRAMPROC) (void);
typedef void (APIENTRYP PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRYP PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLVALIDATEPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRYP PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRYP PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
typedef GLint(APIENTRYP PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRYP PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRYP PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBPOINTERVPROC) (GLuint index, GLenum pname, GLvoid **pointer);
typedef void (APIENTRYP PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint *arrays);
typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLDELETESHADERPROC) (GLuint shader);
typedef GLint (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void (APIENTRYP PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (APIENTRYP PFNGLUNIFORM2FPROC) (GLint location, GLfloat v0, GLfloat v1);

/* copied from gl_core.h */
PFNGLCREATESHADERPROC                           glCreateShader;
PFNGLSHADERSOURCEPROC                           glShaderSource;
PFNGLCOMPILESHADERPROC                          glCompileShader;
PFNGLGETSHADERIVPROC                            glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC                       glGetShaderInfoLog;
PFNGLCREATEPROGRAMPROC                          glCreateProgram;
PFNGLATTACHSHADERPROC                           glAttachShader;
PFNGLLINKPROGRAMPROC                            glLinkProgram;
PFNGLVALIDATEPROGRAMPROC                        glValidateProgram;
PFNGLGETPROGRAMIVPROC                           glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC                      glGetProgramInfoLog;
PFNGLGENBUFFERSPROC                             glGenBuffers;
PFNGLGENVERTEXARRAYSPROC                        glGenVertexArrays;
PFNGLGETATTRIBLOCATIONPROC                      glGetAttribLocation;
PFNGLBINDVERTEXARRAYPROC                        glBindVertexArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC                glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC                    glVertexAttribPointer;
PFNGLBINDBUFFERPROC                             glBindBuffer;
PFNGLBUFFERDATAPROC                             glBufferData;
PFNGLGETVERTEXATTRIBPOINTERVPROC                glGetVertexAttribPointerv;
PFNGLUSEPROGRAMPROC                             glUseProgram;
PFNGLDELETEVERTEXARRAYSPROC                     glDeleteVertexArrays;
PFNGLDELETEBUFFERSPROC                          glDeleteBuffers;
PFNGLDELETEPROGRAMPROC                          glDeleteProgram;
PFNGLDELETESHADERPROC                           glDeleteShader;
PFNGLGETUNIFORMLOCATIONPROC                     glGetUniformLocation;
PFNGLUNIFORM1FPROC                              glUniform1f;
PFNGLUNIFORM2FPROC                              glUniform2f;
PFNWGLSWAPINTERVALEXTPROC                       wglSwapIntervalEXT = 0;

static void 
initOpenGLFunctions()
{
   glCreateShader             = (PFNGLCREATESHADERPROC)
      (wglGetProcAddress("glCreateShader"));
   glShaderSource             = (PFNGLSHADERSOURCEPROC)
      (wglGetProcAddress("glShaderSource"));
   glCompileShader            = (PFNGLCOMPILESHADERPROC)
      (wglGetProcAddress("glCompileShader"));
   glGetShaderiv              = (PFNGLGETSHADERIVPROC)
      (wglGetProcAddress("glGetShaderiv"));
   glGetShaderInfoLog         = (PFNGLGETSHADERINFOLOGPROC)
      (wglGetProcAddress("glGetShaderInfoLog"));
   glCreateProgram            = (PFNGLCREATEPROGRAMPROC)
      (wglGetProcAddress("glCreateProgram"));
   glAttachShader             = (PFNGLATTACHSHADERPROC)
      (wglGetProcAddress("glAttachShader"));
   glLinkProgram              = (PFNGLLINKPROGRAMPROC)
      (wglGetProcAddress("glLinkProgram"));
   glValidateProgram          = (PFNGLVALIDATEPROGRAMPROC)
      (wglGetProcAddress("glValidateProgram"));
   glGetProgramiv             = (PFNGLGETPROGRAMIVPROC)
      (wglGetProcAddress("glGetProgramiv"));
   glGenBuffers               = (PFNGLGENBUFFERSPROC)
      (wglGetProcAddress("glGenBuffers"));
   glGenVertexArrays          = (PFNGLGENVERTEXARRAYSPROC)
      (wglGetProcAddress("glGenVertexArrays"));
   glGetAttribLocation        = (PFNGLGETATTRIBLOCATIONPROC)
      (wglGetProcAddress("glGetAttribLocation"));
   glBindVertexArray          = (PFNGLBINDVERTEXARRAYPROC)
      (wglGetProcAddress("glBindVertexArray"));
   glEnableVertexAttribArray  = (PFNGLENABLEVERTEXATTRIBARRAYPROC)
      (wglGetProcAddress("glEnableVertexAttribArray"));
   glVertexAttribPointer      = (PFNGLVERTEXATTRIBPOINTERPROC)
      (wglGetProcAddress("glVertexAttribPointer"));
   glBindBuffer               = (PFNGLBINDBUFFERPROC)
      (wglGetProcAddress("glBindBuffer"));
   glBufferData               = (PFNGLBUFFERDATAPROC)
      (wglGetProcAddress("glBufferData"));
   glGetVertexAttribPointerv  = (PFNGLGETVERTEXATTRIBPOINTERVPROC)
      (wglGetProcAddress("glGetVertexAttribPointerv"));
   glUseProgram               = (PFNGLUSEPROGRAMPROC)
      (wglGetProcAddress("glUseProgram"));
   glDeleteVertexArrays       = (PFNGLDELETEVERTEXARRAYSPROC)
      (wglGetProcAddress("glDeleteVertexArrays"));
   glDeleteBuffers            = (PFNGLDELETEBUFFERSPROC)
      (wglGetProcAddress("glDeleteBuffers"));
   glDeleteProgram            = (PFNGLDELETEPROGRAMPROC)
      (wglGetProcAddress("glDeleteProgram"));
   glDeleteShader             = (PFNGLDELETESHADERPROC)
      (wglGetProcAddress("glDeleteShader"));
   glGetUniformLocation       = (PFNGLGETUNIFORMLOCATIONPROC)
      (wglGetProcAddress("glGetUniformLocation"));
   glUniform1f                = (PFNGLUNIFORM1FPROC)
      (wglGetProcAddress("glUniform1f"));
   glUniform2f                = (PFNGLUNIFORM2FPROC)
      (wglGetProcAddress("glUniform2f"));
}

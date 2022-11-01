#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/GL.h>
#include "_gl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define u0  void

#define s8  char
#define s16 short
#define s32 int
#define s64 long long

#define u8  unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long

#define f32 float
#define f64 double

#define INLINE static inline

#include "platform.h"

typedef struct window {
   LPCWSTR className;
   LPCSTR classNameChar;
   LPCWSTR title;
   HWND hndl;
   HDC deviceContext;
   HGLRC renderContext;
   s32 prevX, prevY, width, height, prevWidth, prevHeight, resX, resY;
   u8 fullscreen;
} window;

typedef struct shader {
   const s8 *vertex, *fragment;
   GLuint vertexId, fragmentId;
} shader;

typedef struct program {
   GLuint id, vbo, vao;
} program;

global_running = 1;

GLint uniform_WindowSize;
GLint uniform_Time;

static u0 
draw(HDC deviceContext, u32 width, u32 height)
{
   glUniform1f(uniform_Time, Time());
   glUniform2f(uniform_WindowSize, width, height);
   glViewport(0, 0, width, height);
   glClearColor(1, 1, 1, 0);
   glClear(GL_COLOR_BUFFER_BIT);
   glDrawArrays(GL_TRIANGLES, 0, 6);
   SwapBuffers(deviceContext);
}


static LRESULT CALLBACK
wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   LRESULT result = 0;

   switch (message)
   {
      case WM_SIZE:
         HDC ctx = GetDC(hWnd);
         draw(ctx, LOWORD(lParam), HIWORD(lParam));
         break;
      case WM_PAINT:
         break;
      case WM_KEYDOWN:
         /* ESC */
         if (wParam == 27)
            PostMessage(hWnd, WM_CLOSE, 0, 0);
         break;
      case WM_CLOSE: {
         HDC ctx = GetDC(hWnd);
         wglMakeCurrent(ctx, NULL);
         HGLRC rctx = wglGetCurrentContext();
         wglDeleteContext(rctx);
         ReleaseDC(hWnd, ctx);
         DestroyWindow(hWnd);
         /* stop event queue thread */
         PostQuitMessage(0);
         global_running = 0;
         break;
       }
       default:
            result = DefWindowProc(hWnd, message, wParam, lParam);
   }

   return result;
}

static s32 
registerClass(HINSTANCE instance, window *wind) 
{
   WNDCLASSEXW wcex;

   memset(&wcex, 0, sizeof(wcex));
   wcex.cbSize = sizeof(WNDCLASSEX);
   wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
   wcex.lpfnWndProc = (WNDPROC)wndProc;
   wcex.cbClsExtra = 0;
   wcex.cbWndExtra = 0;
   wcex.hInstance = instance;
   wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
   wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground = NULL;
   wcex.lpszMenuName = NULL;
   wcex.lpszClassName = wind->className;
   wcex.hIconSm = NULL;
   
   if(RegisterClassExW(&wcex) == 0) {
      ERR(L"registering class");
      return -1;
   } 

   return 0;
}

static s32 
createWindow(HINSTANCE instance, window *wind)
{
   DWORD  style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
   RECT rect = { 0, 0, wind->resX, wind->resY };

   if(AdjustWindowRect(&rect, style, 0) == 0) {
      ERR(L"adjusting window rect");
      return -1;
   }

   wind->width = rect.right - rect.left;
   wind->height = rect.bottom - rect.top;

   wind->hndl = CreateWindowW(wind->className, wind->title, style, 
                              0, 0, wind->width, wind->height, 0,
                              0, instance, 0);
   if(wind->hndl == 0) {
      ERR(L"creating window");
      return -1;
   }

   return 0;
}

static u0 
centerWindow(window *wind) 
{
   RECT rect;
   MONITORINFO mi = { sizeof(mi) };

   GetMonitorInfo(MonitorFromWindow(wind->hndl, MONITOR_DEFAULTTONEAREST),
                  &mi);
   s32 x = (mi.rcMonitor.right - mi.rcMonitor.left - wind->width) / 2;
   s32 y = (mi.rcMonitor.bottom - mi.rcMonitor.top - wind->height) / 2;

   SetWindowPos(wind->hndl, 0, x, y, 0, 0, 
                SWP_NOZORDER | SWP_NOSIZE | SWP_SHOWWINDOW);
}

static s32
createFakeWindow(HINSTANCE instance, window *fake)
{
   fake->hndl = CreateWindowW(fake->className, L"Fake Window", 
                              WS_OVERLAPPEDWINDOW, 0, 0, 1, 1, 
                              0, 0, instance, 0);

   if (fake->hndl == 0) {
      ERR(L"creating fake window");
      return -1;
   }

   return 0;
}

static s32
createFakeContext(window *fake)
{
   fake->deviceContext = GetDC(fake->hndl);

   if(fake->deviceContext == 0) {
      return -1;
   }

   s32 pixelFormat;
   PIXELFORMATDESCRIPTOR pixelFormatDesc;
   memset(&pixelFormatDesc, 0, sizeof(PIXELFORMATDESCRIPTOR));
   pixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
   pixelFormatDesc.nVersion = 1;
   pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
   pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
   pixelFormatDesc.cColorBits = 32;
   pixelFormatDesc.cAlphaBits = 8;
   pixelFormatDesc.cDepthBits = 24;
   pixelFormat = ChoosePixelFormat(fake->deviceContext, &pixelFormatDesc);

   if(pixelFormat == 0) {

      ERR(L"choosing pixel format");
      return -1;
   }

   if(SetPixelFormat(fake->deviceContext, pixelFormat, 
                     &pixelFormatDesc) == 0) {

      ERR(L"setting pixel format");
      return -1;
   }

   fake->renderContext = wglCreateContext(fake->deviceContext);

   if(fake->renderContext == 0) {

      ERR(L"creating render context");
      return -1;
   }

   return 0;
}

static u0 
destroyFakeWindow(window *fake)
{
   wglMakeCurrent(fake->deviceContext, NULL);
   wglDeleteContext(fake->renderContext);
   ReleaseDC(fake->hndl, fake->deviceContext);
   DestroyWindow(fake->hndl);
}

static s32 
createContext(window *wind, window *fake) 
{
   // wglGetProcAddress needs an active deviceContext which is
   // the reason for the fake window/context
   wglMakeCurrent(fake->deviceContext, fake->renderContext);

   PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
   PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

   wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)(
      wglGetProcAddress("wglChoosePixelFormatARB")
   );
   if(wglChoosePixelFormatARB == 0) {
      return -1;
   }

   wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)(
      wglGetProcAddress("wglCreateContextAttribsARB")
   );
   if(wglCreateContextAttribsARB == 0) {
      return -1;
   }

   wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)(
      wglGetProcAddress("wglSwapIntervalEXT")
   );
   if(wglSwapIntervalEXT == 0) {
      return -1;
   }

   wind->deviceContext = GetDC(wind->hndl);

   if(wind->deviceContext == 0) {

      ERR(L"getting device context");
      return -1;
   }

   s32 pixelFormat;
   BOOL status;
   UINT numFormats;
   const s32 pixelAttribs[] = {
      WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
      WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
      WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
      /* WGL_SWAP_EXCHANGE_ARB causes problems with window menu in fullscreen */
      WGL_SWAP_METHOD_ARB, WGL_SWAP_COPY_ARB,
      WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
      WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
      WGL_COLOR_BITS_ARB, 32,
      WGL_ALPHA_BITS_ARB, 8,
      WGL_DEPTH_BITS_ARB, 24,
      0
   };

   s32 contextAttributes[] = {
      WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
      WGL_CONTEXT_MINOR_VERSION_ARB, 0,
      WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
      0
   };

   status = wglChoosePixelFormatARB(wind->deviceContext, pixelAttribs, 
                                    0, 1, &pixelFormat, &numFormats);

   if(status == 0 || numFormats == 0) {
      return -1;
   }

   PIXELFORMATDESCRIPTOR pfd;
   memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   DescribePixelFormat(wind->deviceContext, pixelFormat, 
                       sizeof(PIXELFORMATDESCRIPTOR), &pfd);

   if(SetPixelFormat(wind->deviceContext, pixelFormat, 
      &pfd) == 0) {

      ERR(L"setting pixel format");
      return -1;
   }

   wind->renderContext = wglCreateContextAttribsARB(wind->deviceContext, 0,                                                    contextAttributes);

   if(wind->renderContext == 0) {

      ERR(L"creating render context");
      return -1;
   }

   return 0;
}

static s32
checkShader(GLuint shaderId)
{
   GLint result;
   glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

   if(result == TRUE) {
      return 0;
   }

   GLsizei length;
   glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);

   if(length > 0) {
      GLchar *infoLog = (GLchar *)malloc(sizeof(GLchar) * length);
      WCHAR *msg = (WCHAR *)malloc(sizeof(WCHAR) * length);

      glGetShaderInfoLog(shaderId, length, &length, infoLog);
      MultiByteToWideChar(CP_ACP, 0, infoLog, -1, (LPWSTR)msg, length + 1);
      ERR(msg);

      free(infoLog);
      free(msg);

   } else {
      ERR(L"shader failed to compile.");
   }

   return -1;
}

static s32
checkProgram(GLuint shaderId, GLenum param)
{
   GLint result;
   glGetProgramiv(shaderId, param, &result);

   if (result == TRUE) {
      return 0;
   }

   GLsizei length;
   glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &length);

   if (length > 0) {
      GLchar *infoLog = (GLchar *)malloc(sizeof(GLchar) * length);
      WCHAR *msg = (WCHAR *)malloc(sizeof(WCHAR) * length);

      glGetShaderInfoLog(shaderId, length, &length, infoLog);
      MultiByteToWideChar(CP_ACP, 0, infoLog, -1, (LPWSTR)msg, length + 1);
      ERR(msg);

      free(infoLog);
      free(msg);
   } else if (param == GL_LINK_STATUS) {
      ERR(L"glLinkProgram failed: Can not link program.");
   } else {
      ERR(L"glValidateProgram() failed: Can not execute shader program.");
   }

   return -1;
}

static s32
initShader(shader *sh) {

   sh->vertexId = glCreateShader(GL_VERTEX_SHADER);
   sh->fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
   
   glShaderSource(sh->vertexId, 1, &sh->vertex, NULL);
   glShaderSource(sh->fragmentId, 1, &sh->fragment, NULL);
   
   glCompileShader(sh->vertexId);
   
   if(checkShader(sh->vertexId) != 0) {   
      return -1;
   }

   glCompileShader(sh->fragmentId);

   if(checkShader(sh->fragmentId) != 0) {
      return -1;
   }

   return 0;
}

static s32
initProgram(program *prgm, shader sh)
{
   prgm->id = glCreateProgram();
   glAttachShader(prgm->id, sh.vertexId);
   glAttachShader(prgm->id, sh.fragmentId);

   glLinkProgram(prgm->id);
   
   if(checkProgram(prgm->id, GL_LINK_STATUS) != 0) {   
      return -1;
   }

   glValidateProgram(prgm->id);

   if(checkProgram(prgm->id, GL_VALIDATE_STATUS) != 0) {
      return -1;
   }

   return 0;
}

static u0 
initVertexObjects(program prgm, window *wind)
{
   uniform_WindowSize = glGetUniformLocation(prgm.id, "WindowSize");
   uniform_Time = glGetUniformLocation(prgm.id, "Time");

   GLint positionLocation = glGetAttribLocation(prgm.id, "positionIn");
   GLint colorLocation = glGetAttribLocation(prgm.id, "colorIn");
   
   glGenBuffers(1, &prgm.vbo);
   glGenVertexArrays(1, &prgm.vao);
   
   // x, y, z, r, g, b (triangle)
   f32 vertices[] = {
   	-1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0,
   	1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0,
   	1.0, -1.0, 0.0, 1.0, 0.0, 1.0, 1.0,

   	1.0, -1.0, 0.0, 1.0, 0.0, 1.0, 1.0,
   	-1.0, -1.0, 0.0, 0.0, 1.0, 1.0, 1.0,
   	-1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0,
   };

   glBindVertexArray(prgm.vao);
   glEnableVertexAttribArray(positionLocation);
   glEnableVertexAttribArray(colorLocation);
   
   glBindBuffer(GL_ARRAY_BUFFER, prgm.vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 
                GL_STATIC_DRAW);
   glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 
                         7 * sizeof(f32), (u0*)0);
   glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 
                         7 * sizeof(f32), (u0*)(3 * sizeof(f32)));
   
   glUseProgram(prgm.id);
}

s32 WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
            PSTR pCmdLine, s32 nCmdShow) 
{
   window wind = {L"OpenGL", "OpenGL", L"OpenGL Example", 0, 
                  0, 0, 0, 0, 0, 0, 0, 0, 640, 480, 0}, 
          fake = {L"OpenGL", "OpenGL", L"OpenGL Example", 0, 
                  0, 0, 0, 0, 0, 0, 0, 0, 640, 480, 0};
   shader sh = {0};

   u32 size;
   sh.vertex = ReadWholeFile("vertex.glsl", &size);
   sh.fragment = ReadWholeFile("fragment.glsl", &size);

   AllocConsole();

   freopen("CONIN$", "r",stdin);
   freopen("CONOUT$", "w",stdout);
   freopen("CONOUT$", "w",stderr);

   program prgm = {0};

   if(registerClass(hInstance, &wind) != 0) {
      return 1;
   }

   if(createFakeWindow(hInstance, &fake) != 0) {
      return 1;
   }

   if(createFakeContext(&fake) != 0) {
      return 1;
   }

   if(createWindow(hInstance, &wind) != 0) {
      return 1;
   }

   if(createContext(&wind, &fake) != 0) {
      return 1;
   }
   
   destroyFakeWindow(&fake);

   wglMakeCurrent(wind.deviceContext, wind.renderContext);

   initOpenGLFunctions();

   if(initShader(&sh) != 0) {
      return 1;
   }
   
   if(initProgram(&prgm, sh) != 0) {
      return 1;
   }

   initVertexObjects(prgm, &wind);

   //wglSwapIntervalEXT(1);
   centerWindow(&wind);
   ShowWindow(wind.hndl, nCmdShow);
   UpdateWindow(wind.hndl);
   
   MSG msg;

   while(global_running == 1) {
      while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
      {
         draw(wind.deviceContext, wind.width, wind.height);
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
   }

   FreeConsole();

   return 0;
}

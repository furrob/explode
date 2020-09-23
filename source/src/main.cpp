#include "inc.h"
#include <Shader.h>

BOOL Create(HWND* hWnd, LPCWSTR ClassName, PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0,
  int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
  HWND hWndParent = 0, HMENU hMenu = 0);

INT_PTR CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR szCmdLine, _In_ int iCmdShow)
{
  HWND hWnd;

  //GLint64 timer;
  //glGetInteger64v(GL_TIMESTAMP, &timer);
  //printf("Miliseconds %f\n", timer / 1000000.0);
  
  if(!Create(&hWnd, L"GameWindow", L"eXplode", WS_OVERLAPPEDWINDOW))
    return 0;

  if(glewInit() != GLEW_OK)
  {
    MessageBoxW(NULL, L"Cannot initialize glew library", L"Error", MB_OK | MB_ICONERROR);
    return 0;
  }
  Shader testShader = Shader("../../source/shaders/vertex.glsl", "../../source/shaders/fragment.glsl");//kompiluje

  ShowWindow(hWnd, iCmdShow);

  MSG msg = {};

  //main loop - every time, when there is no system message, send custom message to refresh game state
  //elapsed time and other things can be passed in message params
  //goal is to maximize "separation" between this code and game class code
  BOOL bExit = FALSE;
  while(!bExit)
  {
    if(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) //check if there are any system generated messages
    {
      if(msg.message == WM_QUIT)
        bExit = TRUE;
      else
      {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
      }
    }
    else //if no, send custom message (maybe add some mechanism to reduce framerate) or call appropriate method from Game class directly
    {
      //SendMessageW(hWnd, WM_APP_UPDATE, NULL, NULL);
    }
  }

  return 0;
}

INT_PTR CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch(uMsg)
  {
    case WM_CREATE:
    {

    }
    case WM_PAINT:
    {

      return TRUE;
    }
    case WM_CLOSE:
    {
      DestroyWindow(hWnd);
      PostQuitMessage(0);

      return TRUE;
    }
    default:
      return DefWindowProcW(hWnd, uMsg, wParam, lParam);
  }
  //return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

BOOL Create(HWND* hWnd, LPCWSTR ClassName, PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle,
  int x, int y, int nWidth, int nHeight,
  HWND hWndParent, HMENU hMenu)
{
  WNDCLASS wc = { 0 };

  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = GetModuleHandleW(NULL);
  wc.lpszClassName = ClassName;

  RegisterClassW(&wc);

  *hWnd = CreateWindowExW(dwExStyle, ClassName, lpWindowName, dwStyle, x, y,
    nWidth, nHeight, hWndParent, hMenu, GetModuleHandleW(NULL), NULL);

  //opengl context creation
  HDC hDC = GetDC(*hWnd);

  PIXELFORMATDESCRIPTOR pfd =
  {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA,
    32,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    24, 8, 0,
    PFD_MAIN_PLANE,
    0,
    0, 0, 0
  };

  GLuint pixelFormat = ChoosePixelFormat(hDC, &pfd);
  SetPixelFormat(hDC, pixelFormat, &pfd);

  HGLRC m_hRC = wglCreateContext(hDC);

  wglMakeCurrent(hDC, m_hRC);

  //game->Initialize(*hWnd, m_hRC, hDC);
  //ReleaseDC(*hWnd, hDC);

  return (hWnd ? TRUE : FALSE);
}
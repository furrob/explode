#include "inc.h"

#include "Game.h"

BOOL Create(HWND* hWnd, LPCWSTR ClassName, PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0,
  int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
  HWND hWndParent = 0, HMENU hMenu = 0);


INT_PTR CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR szCmdLine, _In_ int iCmdShow)
{
  HWND hWnd; //window handle

  Game game;

  if(!Create(&hWnd,L"GameWindow", L"eXplode", WS_POPUP | WS_VISIBLE))//WS_OVERLAPPEDWINDOW))
    return 0;
  
  if(glewInit() != GLEW_OK)
  {
    MessageBoxW(NULL, L"Cannot initialize glew library", L"Error", MB_OK | MB_ICONERROR);
    return 0;
  }
  
  ShowWindow(hWnd, iCmdShow);

  HDC hDC = GetDC(hWnd); //device context
  HGLRC hRC = wglGetCurrentContext(); //rendering context

  MSG msg = {};

  //main loop - every time, when there is no system message, send custom message to refresh game state
  //elapsed time and other things can be passed in message params
  //goal is to maximize "separation" between this code and game class code
  BOOL bExit = FALSE;

  //elapsed time measurement
  LARGE_INTEGER counterFreq;
  QueryPerformanceFrequency(&counterFreq);

  LARGE_INTEGER lastTime;
  QueryPerformanceCounter(&lastTime);

  LARGE_INTEGER actualTime;
  LARGE_INTEGER elapsedTicks;

  double elapsedTime = 0;

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
      QueryPerformanceCounter(&actualTime);

      elapsedTicks.QuadPart = actualTime.QuadPart - lastTime.QuadPart;

      elapsedTime = ((elapsedTicks.QuadPart * 1000000.0) / counterFreq.QuadPart) / 1000000.0;
      //------------------

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glClearColor(1.0f, 0.25f, 0.25f, 0.0f);

      SwapBuffers(hDC);
      //--------------------
      lastTime.QuadPart = actualTime.QuadPart;

    }
  }

  //cleaning
  if(hRC)
  {
    wglMakeCurrent(NULL, NULL); //
    wglDeleteContext(hRC); //rendering context deletion hehe
    hRC = 0;
  }
  if(hDC)
  {
    ReleaseDC(hWnd, hDC);
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
    case WM_KEYDOWN:
    {
      if(wParam == VK_ESCAPE) //change to something with fullscreen
      {
        DestroyWindow(hWnd);
        PostQuitMessage(0);
      }
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

  //making window fullscreen
  POINT point = {0, 0}; //0,0 on main monitor (always)
  HMONITOR hMon = MonitorFromPoint(point, MONITOR_DEFAULTTOPRIMARY); //get handle to primary monitor

  MONITORINFO mi = { sizeof(mi) };
  if(!GetMonitorInfoW(hMon, &mi))
    return FALSE;
  
  *hWnd = CreateWindowExW(dwExStyle, ClassName, lpWindowName, dwStyle, mi.rcMonitor.left, mi.rcMonitor.top,//x, y,
    mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, hWndParent, hMenu, GetModuleHandleW(NULL), NULL);

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
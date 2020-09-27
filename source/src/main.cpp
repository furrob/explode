#include "inc.h"

#include "Game.h"
#include "OBJLoader.h"
#include <string>

BOOL Create(HWND* hWnd, LPCWSTR ClassName, PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0,
  int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
  HWND hWndParent = 0, HMENU hMenu = 0);


INT_PTR CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR szCmdLine, _In_ int iCmdShow)
{
  HWND hWnd; //window handle

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

  //game object creation
  RECT rc;
  GetClientRect(hWnd, &rc);
  Game game(hWnd, rc.right - rc.left, rc.bottom - rc.top, hDC);
  game.Initialize();


  SendMessageW(hWnd, WM_USER, NULL, (LPARAM)&game);

  ///ClipCursor(&rc); //TODO this

  //OpenGL options?
  glEnable(GL_DEPTH_TEST);

  //transparency?
  glDisable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

      //1 - update everything somehow linked to physics
      game.Update(elapsedTime);
      //every input will be processed in different functions, called from window proc
      //
      //2 - 

      //------------------
      
      game.RenderScene(); //teœcik
      
      //--------------------
      lastTime.QuadPart = actualTime.QuadPart;

    }
  }

  DestroyWindow(hWnd);

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
  static Game* game = nullptr;
  static BOOL cursor_hidden = FALSE;

  switch(uMsg)
  {
    case WM_USER:
    {
      game = reinterpret_cast<Game*>(lParam);
      return TRUE;
    }
    //  KEYBOARD - probably will be readed async in game loop
    case WM_KEYDOWN:
    {
      switch(wParam)
      {
        case VK_ESCAPE:
        {
          PostQuitMessage(0);
          return TRUE;
        }
        case 0x31: //1 key
        {
          if(cursor_hidden == FALSE)
          {
            ShowCursor(FALSE);
            cursor_hidden = TRUE;
          }
          return TRUE;
        }
        case 0x32: //2 key
        {
          if(cursor_hidden == TRUE)
          {
            ShowCursor(TRUE);
            cursor_hidden = FALSE;
          }
          return TRUE;
        }
        case 0x33: //3 key
        {
          RECT rc;
          GetClientRect(hWnd, &rc);
          ClipCursor(&rc);
          return TRUE;
        }
        case 0x34: //4 key
        {
          ClipCursor(NULL);
          return TRUE;
        }
      }
      return TRUE;
    }
    case WM_MOUSEMOVE:
    {
      int x_pos = GET_X_LPARAM(lParam);
      int y_pos = GET_Y_LPARAM(lParam);

      if(game != nullptr)
        game->OnMouseMove(x_pos, y_pos);

      return TRUE;
    }
    case WM_CLOSE:
    {
      
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
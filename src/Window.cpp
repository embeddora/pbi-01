/* ������������ ���� ��� NewWindow */
#include "Window.h"

/* ������������ ���� ��� NewRender */
#include "Render.h"

#include "Timer.h"

#include "Console.h"

#include "File.h"

#pragma warning( disable : 4311 )

#pragma warning( disable : 4312 )

NewWindow::NewWindow(void)
{
}

NewWindow::~NewWindow(void)
{
}

/* ���������� ����� �������� ����, ��� ������������ FullScreen/Windowed */
static BOOL g_isProgramLooping;

/* ���� TRUE, ����� Fullscreen */
static BOOL g_createFullScreen;

static BOOL FirstStart;

/* ����������� ���������� */
void TerminateApplication (GL_Window* window)
{
	/* �������� ��������� A WM_QUIT */
	PostMessage (window->hWnd, WM_QUIT, 0, 0);

	/* ��������� ��������� ����� */
	g_isProgramLooping = FALSE;
}

/* ������������ Fullscreen/Windowed */
void ToggleFullscreen (GL_Window* window)
{
	/* �������� ���������A WM_TOGGLEFULLSCREEN  */
	PostMessage (window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);
}

/* ����������� ��������� ����, ����� ���������� ��� ��������� ������� */
void ReshapeGL (int width, int height)
{
	MainRender.Reshape(width, height);
}

/* ������ ���������� ������ */
BOOL ChangeScreenResolution (int width, int height, int bitsPerPixel, int frequency) 
{
DEVMODE devmode;

DEVMODE curmode;

	curmode.dmPelsWidth  = 10000;

    curmode.dmPelsHeight = 10000;
		
    int mod = 0;

    while(EnumDisplaySettings(NULL,mod,&devmode))
	{
		int res = ChangeDisplaySettings(&devmode, CDS_TEST);

		if((res == DISP_CHANGE_SUCCESSFUL)||(res == DISP_CHANGE_RESTART))
		{
			if(((int)devmode.dmBitsPerPel == bitsPerPixel) &&((int)devmode.dmPelsWidth >= width) &&	((int)devmode.dmPelsHeight >= height) &&(devmode.dmPelsWidth <= curmode.dmPelsWidth) &&	(devmode.dmPelsHeight <= curmode.dmPelsHeight) &&(devmode.dmDisplayFrequency <= frequency))
			{
				curmode = devmode;
			}
		} /* if (res == ...*/

		mod++;
    } /* while (...) */

    if(0 == curmode.dmBitsPerPel)

		return FALSE;

	if (DISP_CHANGE_SUCCESSFUL != ChangeDisplaySettings (&curmode, CDS_FULLSCREEN))
	{
		/* ���� �� ������� ���������� ����� ������, �� ���������� False */
		return FALSE;
	}

	MainConsole.Add(0, "���������� ����� �����: %ix%i, bpp=%i, refresh=%i",curmode.dmPelsWidth, curmode.dmPelsHeight, curmode.dmBitsPerPel, curmode.dmDisplayFrequency);

	/* �� ����������, ���������� True */
	return TRUE;
}

/* ���� ��� ������ ���� OpenGL ���� */
BOOL CreateWindowGL (GL_Window* window)	
{
/* ������� ����� ���� */
DWORD windowStyle = WS_OVERLAPPEDWINDOW;

/* ������� ���������� ����� ���� */
DWORD windowExtendedStyle = WS_EX_APPWINDOW;

	/* ������ ���� */
	window->init.width			= MainSettings.SCREEN_WIDTH;

	/* ������ ���� */
	window->init.height			= MainSettings.SCREEN_HEIGHT;

	/* ����� �� ������� */
	window->init.bitsPerPixel	= MainSettings.BPP;

	/* ����������? (���� TRUE) */
	window->init.isFullScreen	= MainSettings.FULLSCREEN;

	/* ������� ���������� ������ */
	window->init.frequency		= MainSettings.SCREEN_frequency;
	
	/* ������� ���������� ���� */
	RECT windowRect = {0, 0, window->init.width, window->init.height};

	/* ������� ������ Windows */
	ShowCursor(FALSE);

	/* �������� Fullscreen, ��������� ������� ����� ����� */
	if (TRUE == MainSettings.FULLSCREEN)
	{
		if (FALSE == ChangeScreenResolution (window->init.width, window->init.height, window->init.bitsPerPixel, window->init.frequency) )
		{
			/* ����� Fullscreen �� ��������.  ����������� � ������ ����� */
			MessageBox (HWND_DESKTOP, "Mode Switch Failed.\nRunning In Windowed Mode.", "Error", MB_OK | MB_ICONEXCLAMATION);

			/* ��������� isFullscreen � False (������� �����) */
			window->init.isFullScreen = FALSE;
		}
		else /* ����� (���� Fullscreen ����� �����������) */
		{
			/* ��������� ������ ���� */
			ShowCursor (FALSE);

			/* ������������� WindowStyle � WS_POPUP (����������� ����) */
			windowStyle = WS_POPUP;

			/* ������������� ���������� ����� ���� � WS_EX_TOPMOST */
			windowExtendedStyle |= WS_EX_TOPMOST;

		}/* (Top ���� ��������� �� ������ �����) */
	}
	else /* ���� Fullscreen ����� ������ */
	{
		/* �������� ���� �� ����� ������ */
		AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);
	}

	/* ������� OpenGL ���� */
	window->hWnd = CreateWindowEx (windowExtendedStyle/* ���������� ����� */,
									/* ��� ������ */
									window->init.application->className,
									/* ��������� ���� */
									window->init.title,
									/* ����� ���� */
									windowStyle,
									/* X,Y ������� ���� */
									0, 0,
									/* ������ ���� */
									windowRect.right - windowRect.left,
									/* ������ ���� */
									windowRect.bottom - windowRect.top,
									/* �������� ���� ������� ���� */
									HWND_DESKTOP,
									/* ��� ���� */
									0,
									/* ������ Window Instance */
									window->init.application->hInstance, window);

	/* ���� ��������� �������? */
	if (0 == window->hWnd)	
	{
		/* ���� ���, �� ��������� False */
		return FALSE;
	}

	/* ��������� �������� ���������� ��� ����� ���� */
	window->hDC = GetDC (window->hWnd);	

	/* �������� ���������� ����������? */
	if (0 == window->hDC)
	{
		/* Jesli net, to ���������� ���� */
		DestroyWindow (window->hWnd);

		/* �������� ���������� ���� */
		window->hWnd = 0;

		/* ���������� False */
		return FALSE;
	}

	/* ����� � ������� ��������� */
	MainConsole.Add(0,"������� ���� %dx%dx%d, ����������: %s.",	MainSettings.SCREEN_WIDTH,	MainSettings.SCREEN_HEIGHT,	MainSettings.BPP,	(MainSettings.FULLSCREEN ? "��" : "���" ));

	/* ������������� OpenGL, ���� ��� �� �������. */
	if (!MainRender.SetupOpenGL(window))
		
		return FALSE;

	/* ������� ���� ������� */
	ShowWindow (window->hWnd, SW_NORMAL);

	/* ��������� isVisible � True */
	window->isVisible = TRUE;

	/* �������� ���� ���� */
	ReshapeGL (window->init.width, window->init.height);

	/* ������� ��� ������� */
	ZeroMemory (window->keys, sizeof (Keys));

	/* �������� ���� ������ �������. A vot i������������ ���������� � WM_CREATE */
	return TRUE;							
}

/* ��������� ��������� */
LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/* �������� �������� ���� */
	GL_Window* window = (GL_Window*)(GetWindowLong (hWnd, GWL_USERDATA));

	/* ������������ ��������� */
	switch (uMsg)
	{
		/* ������������� ��������� ������� */
		case WM_SYSCOMMAND:
		{
			/* ��������� ��������� ������ */
			switch (wParam)	
			{
				/* ��������� ������ �������� �����������? */
				case SC_SCREENSAVE:	

				/* ������� �������� ���� � ������ �����? */
				case SC_MONITORPOWER:

				/* �� ���� ����� ����, ����� ������ �� ���������� */
				return 0;
			}
			break;
		}

		/* ������� �� ������� */
		return 0;

		/* �������� ���� */
		case WM_CREATE:	
		{
			/* ��������� ��������� ���� � ��������� */
			CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);

			window = (GL_Window*)(creation->lpCreateParams);

			SetWindowLong (hWnd, GWL_USERDATA, (LONG)(window));

			/* ������� �� ������� */
			return 0;
		}		

		/* ��������� ���� */
		case WM_CLOSE:

			/* ������������� ���������� */
			TerminateApplication(window);

			/* ������� �� ������� */
			return 0;

		/* ���������� ����� ������� ���� */
		case WM_SIZE:

			/* Opredelim ����� */
			switch (wParam)
			{
				/* ���� ��������������? */
				case SIZE_MINIMIZED:

					/* ������������� isVisible � False */
					window->isVisible = FALSE;

					/* ������� �� ������� */
					return 0;

				/* ���� ���������������? */
				case SIZE_MAXIMIZED:

					/* ������������� isVisible � True */
					window->isVisible = TRUE;

					/* ��������� ���� - LoWord=Width, HiWord=Height */
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));

					/* ������� �� ������� */
					return 0;

				/* ���� ���� ������������? */
				case SIZE_RESTORED:

					/* ������������� isVisible � True */
					window->isVisible = TRUE;

					/* ��������� - LoWord=Width, HiWord=Height */
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));

					/* ������� �� ������� */
					return 0;

			} /* switch (wParam) */
			
			/* end of big switch(), case WM_SIZE */
			break; 

		/* ��������� ������ ����������, ������ ������� */
		case WM_KEYDOWN:

			if ((MainSettings.CONSOLE.STATUS!=NEW_CONSOLE_VISIBLE) && (MainSettings.CONSOLE.STATUS!=NEW_CONSOLE_FADEIN) )

				/* ������� (wParam) � ���������� ���������? */
				if ((wParam >= 0) && (wParam <= 255))
				{
					/* ������������� �������� ������� (wParam) � True */
					window->keys->keyDown [wParam] = TRUE;	
				}

			ProcessKeys(hWnd,uMsg,wParam,lParam);

			/* ����� */
			break;

		/* ������� �������� */
		case WM_KEYUP:

			if ((MainSettings.CONSOLE.STATUS!=NEW_CONSOLE_VISIBLE) && (MainSettings.CONSOLE.STATUS!=NEW_CONSOLE_FADEIN) )

				/* ������� (wParam) � ���������� ���������? */
				if ((wParam >= 0) && (wParam <= 255))	
				{
					/* ������������� �������� ������� (wParam) � False */
					window->keys->keyDown [wParam] = FALSE;	
				}

			/* ������� */
			break;

		/* ���� ������ ������ */
		case WM_CHAR:

			/* ���� ������� ������ */
			if ((MainSettings.CONSOLE.STATUS==NEW_CONSOLE_VISIBLE) || (MainSettings.CONSOLE.STATUS==NEW_CONSOLE_FADEIN) )
			{
				/* ���� ������ ������� */
				if ((wParam>31)&&(wParam!='`'))

					/* ��������� � �������� ������� ������� */
					MainConsole.Current+=(char)wParam;
			}

			/* big switch(...)  */
			break;

		/* ������ ����� �����: FullScreen ����� ���./����. */
		case WM_TOGGLEFULLSCREEN:

			MainConsole.Add(0,"����� ����� ������ (�������/����������)");

			g_createFullScreen = (g_createFullScreen == TRUE) ? FALSE : TRUE;

			MainSettings.FULLSCREEN=g_createFullScreen;

			FirstStart=FALSE;

			PostMessage (hWnd, WM_QUIT, 0, 0);

			/* big switch() */
			break;	

		/* Left mouse button is pressed */
		case WM_LBUTTONDOWN:

			MainInput.MouseButton[0]=TRUE;

			return 0;

		/* Right mouse button is pressed */
		case WM_RBUTTONDOWN:

			MainInput.MouseButton[1]=TRUE;

			return 0;

		/* Middle mouse button is pressed */
		case WM_MBUTTONDOWN:

			MainInput.MouseButton[2]=TRUE;

			return 0;

		/* Left mouse button is pressed */
		case WM_LBUTTONUP:

			MainInput.MouseButton[0]=FALSE;

			return 0;

		/* Right mouse button is pressed */
		case WM_RBUTTONUP:	

			MainInput.MouseButton[1]=FALSE;

			return 0;

		/* Middle mouse button is pressed */
		case WM_MBUTTONUP:

			MainInput.MouseButton[2]=FALSE;

			return 0;

		case WM_MOUSEMOVE:

			POINT cu;

			cu.x=LOWORD(lParam);

			cu.y=HIWORD(lParam);

			ClientToScreen(hWnd,&cu);

			MainInput.MousePos.Set((float)cu.x,(float)cu.y);

			MainInput.MouseMoved=TRUE;

			return 0;
	}

	/* ��������� ��������� �������� � DefWindowProc */
	return DefWindowProc (hWnd, uMsg, wParam, lParam);
}

/* ������������ ����� ���� ��� ������ ����������. Jesli vozvraschajet TRUE ������ �� ��������� */
BOOL RegisterWindowClass (Application* application)			
{																		
/* ����� ����. ������������ ������� ����� */
WNDCLASSEX windowClass;	
	
	/* �������� ��� ������ ������� */
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));	
	
	/* ������ ��������� windowClass */
	windowClass.cbSize			= sizeof (WNDCLASSEX);
	
	/* �������������� ���� ��� ����� ��������� � ��������� ������� */
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	
	/* �e�������� WindowProc ��������� */
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				
	
	/* ������������� Instance */
	windowClass.hInstance		= application->hInstance;				
	
	/* ����� ����� ���� */
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			
	
	/* ��������� ��������� (������ ����) */
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			
	
	/* ������������� ��� ������ ���������� */
	windowClass.lpszClassName	= application->className;				

	/* ����������� �����������? */
	if (RegisterClassEx (&windowClass) == 0)							
	{
		/* ��������: ������ �� ������ ������� ���������� */
		MessageBox (HWND_DESKTOP, "RegisterClassEx Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);

		/* ���������� False (������) */
		return FALSE;
	}

	/* ���������� True (�� ��) */
	return TRUE;
}

/* ��������� ���������� */
Application			NEW_application;

/* ��������� ���� */
GL_Window			NEW_window;
	

/* ����� ����� ��������� (WinMain) */
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
/* ��������� �������? */
BOOL	isMessagePumpActive;

/* ��������� ������� ��������� */
MSG		msg;
	
	/* ����� � ������� ��������� */
	MainConsole.Add(0,"Engine ���������...");

	InitFileSystem("");

	/* �������������� ��������� ��������� and loading scene here */
	InitSettings();

	/* ����� � ������� ��������� */
	MainConsole.Add(0,"������������� �������� ������ �������.");

	/* ��������� ������ ����������: ��� ������ ���������� */
	NEW_application.className = "Engine";

	/* i Instance ���������� */
	NEW_application.hInstance = hInstance;


	/* ��������� ����. Pervim delom: �������� ��� ������ ������� */
	ZeroMemory (&NEW_window, sizeof (GL_Window));

	/* ��������� ������ */
	NEW_window.keys					= &MainInput.keys;

	/* ���������� */
	NEW_window.init.application		= &NEW_application;

	/* ��������� ���� */
	NEW_window.init.title			= (char *)MainSettings.TITLE.c_str();

	/* ������ ���� */
	NEW_window.init.width			= MainSettings.SCREEN_WIDTH;

	/* ������ ���� */
	NEW_window.init.height			= MainSettings.SCREEN_HEIGHT;

	/* ����� �� ������� */
	NEW_window.init.bitsPerPixel	= MainSettings.BPP;

	/* ����������? (���� TRUE) */
	NEW_window.init.isFullScreen	= MainSettings.FULLSCREEN;

	MainInput.Init();

	/* ������������ ����� ��� ������ ����. ���������� �����������? */
	if (FALSE == RegisterWindowClass (&NEW_application))	
	{
		/* ������. ����� � ������� ��������� */
		MainConsole.Add(0,"������ �������� ����! �������.");

		/* ��������� ���������� */
		return -1;
	}
	/* ����� � ������� ��������� */
	MainConsole.Add(0,"�������� ���� ������ �������.");

	/* ����������� ���� ������������� � TRUE */
	g_isProgramLooping = TRUE;

	/* g_createFullScreen ������������� �� ��������� */
	g_createFullScreen = NEW_window.init.isFullScreen;

	/* ������������� �������� �� �������� (�������� � �.�.) */
	newInitTimeOperation();

	/* ��� ������� */
    newTimer myTimer;
	
	FirstStart=TRUE;

	/* ���� ���� WM_QUIT �� �������� */
	while (g_isProgramLooping)	
	{
		/* ������ ����. ������������� ��������� ��������� �������� ���� � Fullscreen? */
		NEW_window.init.isFullScreen = g_createFullScreen;

		/* ���� ��������� �������? */
		if (TRUE == CreateWindowGL (&NEW_window))	
		{
			/* � ��� �� ����� ���� � ��� ����������������� OpenGL. ��������� ������������� ���� */
			if (FALSE == Initialize (&NEW_window, &MainInput.keys, FirstStart))	
			{
				/* ������. ��������� ����, ��� �������� �������� ���������� */
				TerminateApplication (&NEW_window);	
			}
			else /* ����� (�������� ��������� ���������) */
			{
				/* ������������� ������ �������. ������������� isMessagePumpActive � TRUE. */
				isMessagePumpActive = TRUE;

				/* �������� ������ */
				myTimer.newStartTiming();
				
				/* ����� � ������� ��������� */
				MainConsole.Add(0,"����� �������� �����.");

				while (TRUE == isMessagePumpActive)
				{
					/* ������� �������� ����, ��������� ��������� */
					if (PeekMessage (&msg, NEW_window.hWnd, 0, 0, PM_REMOVE) != 0)
					{
						/* ��������� �� ��������� WM_QUIT. ��� ��������� A WM_QUIT? */
						if (msg.message != WM_QUIT)
						{
							/* ���� ��� to ��������� ��������� */
							TranslateMessage(&msg);

							/* ���������� ��������� */
							DispatchMessage (&msg);
						}
						else /* ����� (���� ��������� WM_QUIT) */
						{
							FirstStart=FALSE;

							/* ������������� ��������� ��������� */
							isMessagePumpActive = FALSE;
						}
					}
					else /* ���� ���� ��������� */
					{
						/* ���� ���� �� ������ */
						if (NEW_window.isVisible == FALSE)	
						{
							/* ���� ��������������, ���� ��������� */
							WaitMessage ();	
						}
						else /* ���� ���� ������ */
						{
							/* ������ ��������. ������������� �������, ��������� ����� ��������� � myTimer.ElapsedTime */
							myTimer.newTimeElapsed();   

							/* ���������, ������� � �.�. */
							Update (myTimer.ElapsedTime);

							/* �������� ������� */
							myTimer.newStartTiming();

							/* ������ ����� */
							Draw ();

							/* ������ ������ (������� ������������) */
							SwapBuffers (NEW_window.hDC);

							/* ����������� ����� ����� */
							MainRender.Frame_Num++;
							
							MainRender.Tri_accum += MainRender.Tri_per_frame;

							MainRender.Tri_per_frame = 0;
						}
					}
				}/* while ( isMessagePumpActive == TRUE )*/

				/* ����� � ������� ��������� */
				MainConsole.Add(0,"����� �������� �����.");

			} /* if Initialize (... */

			/* ���������� ����������� ������, ��������������� ������. ������������ ������������� �������. */
			Deinitialize ();

			/* ���������� �������� ���� */
			MainRender.DestroyWindowGL (&NEW_window);									
		}
		else /* e��� �������� ���� ����������� */
		{
			/* ������ �������� ���� */
			MessageBox (HWND_DESKTOP, "Error Creating OpenGL Window", "Error", MB_OK | MB_ICONEXCLAMATION);

			/* ��������� ���� */
			g_isProgramLooping = FALSE;

		}

	}/* while (isProgramLooping) */

	/* ����� � ������� ��������� */
	MainConsole.Add(0,"�������� ���������...�����");

	/* ������� ����� ���� */
	UnregisterClass (NEW_application.className, NEW_application.hInstance);		

	/* ����� � ������� ��������� */
	MainConsole.Add(0,"Engine �������� ���� ������.");

	return 0;

} /* ����� WinMain() */

void newMouseToCenter()
{
RECT rcScreen;

	GetClientRect(NEW_window.hWnd, &rcScreen);

	POINT cen;

	cen.x=rcScreen.right / 2;

	cen.y=rcScreen.bottom / 2;

	ClientToScreen(NEW_window.hWnd,&cen);

	SetCursorPos(cen.x, cen.y); 

	MainInput.MouseCenter.Set((float)cen.x, (float)cen.y);

	MainInput.MousePos = MainInput.MouseCenter;
}
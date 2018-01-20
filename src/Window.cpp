/*
 * Copyright (C) 2018 ARNERI arneri@ukr.net All rights reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Abstract: 
 */

/* ������������ ���� ��� NewWindow */
#include "Window.h"

/* ������������ ���� ��� NewRender */
#include "Render.h"

#include "Timer.h"

#include "Console.h"

#include "File.h"

/* Declaration of memset (. . . ) */
#include <string.h>

/* Forge. Temporary. Etc... */
#define ZeroMemory(x, y)  memset(x, 0, y)

namespace std { using namespace __gnu_cxx; }


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
#if 0
	/* �������� ��������� A WM_QUIT */
	PostMessage (window->hWnd, WM_QUIT, 0, 0);
#else
	printf("%s - TerminateApplication \n", __func__);

#endif /* (0) */

	/* ��������� ��������� ����� */
	g_isProgramLooping = FALSE;

}

/* ������������ Fullscreen/Windowed */
void ToggleFullscreen (GL_Window* window)
{
#if 0
	/* �������� ���������A WM_TOGGLEFULLSCREEN  */
	PostMessage (window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);
#else
	printf("%s - TerminateApplication \n", __func__);

#endif /* (0) */

}

/* ����������� ��������� ����, ����� ���������� ��� ��������� ������� */
void ReshapeGL (int width, int height)
{
	MainRender.Reshape(width, height);
}


typedef char TCHAR;
typedef unsigned short WORD;
typedef unsigned long DWORD;

typedef struct _POINTL {
  long x;
  long y;
} POINTL, *PPOINTL;


typedef struct _devicemode {
  TCHAR dmDeviceName[/* CCHDEVICENAME */260];
  WORD  dmSpecVersion;
  WORD  dmDriverVersion;
  WORD  dmSize;
  WORD  dmDriverExtra;
  DWORD dmFields;
  union {
    struct {
      short dmOrientation;
      short dmPaperSize;
      short dmPaperLength;
      short dmPaperWidth;
      short dmScale;
      short dmCopies;
      short dmDefaultSource;
      short dmPrintQuality;
    };
    struct {
      POINTL dmPosition;
      DWORD  dmDisplayOrientation;
      DWORD  dmDisplayFixedOutput;
    };
  };
  short dmColor;
  short dmDuplex;
  short dmYResolution;
  short dmTTOption;
  short dmCollate;
  TCHAR dmFormName[/*CCHFORMNAME*/260];
  WORD  dmLogPixels;
  DWORD dmBitsPerPel;
  DWORD dmPelsWidth;
  DWORD dmPelsHeight;
  union {
    DWORD dmDisplayFlags;
    DWORD dmNup;
  };
  DWORD dmDisplayFrequency;
/*
#if (WINVER >= 0x0400)
  DWORD dmICMMethod;
  DWORD dmICMIntent;
  DWORD dmMediaType;
  DWORD dmDitherType;
  DWORD dmReserved1;
  DWORD dmReserved2;
#if (WINVER >= 0x0500) || (_WIN32_WINNT >= 0x0400)
  DWORD dmPanningWidth;
  DWORD dmPanningHeight;
#endif 
#endif 
*/
} DEVMODE, *PDEVMODE, *LPDEVMODE;


#if 0

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

#endif /* (0) */

#if 0

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

#endif /* (0) */

#if 0

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

#endif /* (0) */

/* ��������� ���������� */
Application			NEW_application;

/* ��������� ���� */
GL_Window			NEW_window;
	

/* ����� ����� ��������� (WinMain) */
//+++int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int  main (int argc, char ** argv)
{
/* ��������� �������? */
BOOL	isMessagePumpActive;

/* ��������� ������� ��������� */
//+++MSG		msg;
	
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
//+++ f.off	NEW_application.hInstance = hInstance;


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

#if 0
	/* ������������ ����� ��� ������ ����. ���������� �����������? */
	if (FALSE == RegisterWindowClass (&NEW_application))	
	{
		/* ������. ����� � ������� ��������� */
		MainConsole.Add(0,"������ �������� ����! �������.");

		/* ��������� ���������� */
		return -1;
	}
#else
	printf ("[warning]  in %s Registering Window class skipped \n", __func__ ) ;
#endif /* (0) */

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
#if 0
		if (TRUE == CreateWindowGL (&NEW_window))	
#else
		printf ("[error] '%s' creation of window <CreateWindowGL (&NEW_window)> SKIPPED  \n", __func__ );
		if (TRUE)
#endif /* 0 */
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
#if 0
					if (PeekMessage (&msg, NEW_window.hWnd, 0, 0, PM_REMOVE) != 0)
#else
					printf("[warning] in '%s'  simulaitng arrival of new mesage by  (PeekMessage <--PM_REMOVE ) \n", __func__);
					if (TRUE)
#endif /* (0) */					
					{


						/* ��������� �� ��������� WM_QUIT. ��� ��������� A WM_QUIT? */
#if 0
						if (msg.message != WM_QUIT)
#else
						printf("[warning] in '%s'  simulaitng absence of <msg.message<--WM_QUIT> \n", __func__);
						if (FALSE)
#endif /* (0) */

						{
#if 0
							/* ���� ��� to ��������� ��������� */
							TranslateMessage(&msg);

							/* ���������� ��������� */
							DispatchMessage (&msg);
#else
							printf("[error] in '%s'  the  <TranslateMessage(&msg)/DispatchMessage(&msg)> skipped \n", __func__);
#endif /* (0) */
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
#if 0
							/* ���� ��������������, ���� ��������� */
							WaitMessage ();	
#else
							printf("[error] in '%s'  the  <WaitMessage ()> skipped \n", __func__);
#endif /* (0) */
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
#if 0

							/* ������ ������ (������� ������������) */
							SwapBuffers (NEW_window.hDC);
#else
							printf("[error] in '%s' the <SwapBuffers (NEW_window.hDC)> skipped \n", __func__);
#endif /* (0) */

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
			//+++MessageBox (HWND_DESKTOP, "Error Creating OpenGL Window", "Error", MB_OK | MB_ICONEXCLAMATION);
			printf (">>>>>>> [error] '%s' Error Creating OpenGL Window", __func__);

			/* ��������� ���� */
			g_isProgramLooping = FALSE;

		}

	}/* while (isProgramLooping) */



	/* ����� � ������� ��������� */
	MainConsole.Add(0,"�������� ���������...�����");

#if 0
	/* ������� ����� ���� */
	UnregisterClass (NEW_application.className, NEW_application.hInstance);		
#else
	printf ("[warning]  in %s UNregistering Window class skipped \n", __func__ ) ;
#endif /* (0) */

	/* ����� � ������� ��������� */
	MainConsole.Add(0,"Engine �������� ���� ������.");

	return 0;

} /* ����� WinMain() */



void newMouseToCenter()
{
#if 0
RECT rcScreen;

	GetClientRect(NEW_window.hWnd, &rcScreen);

	POINT cen;

	cen.x=rcScreen.right / 2;

	cen.y=rcScreen.bottom / 2;

	ClientToScreen(NEW_window.hWnd,&cen);


	SetCursorPos(cen.x, cen.y); 

	MainInput.MouseCenter.Set((float)cen.x, (float)cen.y);
#endif /* 0 */

	MainInput.MousePos = MainInput.MouseCenter;
}


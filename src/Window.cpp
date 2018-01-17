/* Заголовочный файл для NewWindow */
#include "Window.h"

/* Заголовочный файл для NewRender */
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

/* Переменная цикла создания окна, для переключения FullScreen/Windowed */
static BOOL g_isProgramLooping;

/* Если TRUE, тогда Fullscreen */
static BOOL g_createFullScreen;

static BOOL FirstStart;

/* Заканчиваем приложение */
void TerminateApplication (GL_Window* window)
{
	/* Посылаем сообщение A WM_QUIT */
	PostMessage (window->hWnd, WM_QUIT, 0, 0);

	/* Остановка основного цикла */
	g_isProgramLooping = FALSE;
}

/* Переключение Fullscreen/Windowed */
void ToggleFullscreen (GL_Window* window)
{
	/* Посылаем сообщениеA WM_TOGGLEFULLSCREEN  */
	PostMessage (window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);
}

/* Пересчитать настройки окна, после переещения или изменения размера */
void ReshapeGL (int width, int height)
{
	MainRender.Reshape(width, height);
}

/* Меняет разрешение экрана */
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
		/* Если не удалось установить режим экрана, то возвращаем False */
		return FALSE;
	}

	MainConsole.Add(0, "Установлен видео режим: %ix%i, bpp=%i, refresh=%i",curmode.dmPelsWidth, curmode.dmPelsHeight, curmode.dmBitsPerPel, curmode.dmDisplayFrequency);

	/* Всё получилось, возвращаем True */
	return TRUE;
}

/* Этот код создаёт наше OpenGL окно */
BOOL CreateWindowGL (GL_Window* window)	
{
/* Зададим стиль окна */
DWORD windowStyle = WS_OVERLAPPEDWINDOW;

/* Зададим расширеный стиль окна */
DWORD windowExtendedStyle = WS_EX_APPWINDOW;

	/* Ширина окна */
	window->init.width			= MainSettings.SCREEN_WIDTH;

	/* Высота окна */
	window->init.height			= MainSettings.SCREEN_HEIGHT;

	/* Битов на пиксель */
	window->init.bitsPerPixel	= MainSettings.BPP;

	/* Полноэкран? (Если TRUE) */
	window->init.isFullScreen	= MainSettings.FULLSCREEN;

	/* Частота обновления экрана */
	window->init.frequency		= MainSettings.SCREEN_frequency;
	
	/* Зададим координаты окна */
	RECT windowRect = {0, 0, window->init.width, window->init.height};

	/* Убираем курсор Windows */
	ShowCursor(FALSE);

	/* Запрошен Fullscreen, попробуем сменить видео режим */
	if (TRUE == MainSettings.FULLSCREEN)
	{
		if (FALSE == ChangeScreenResolution (window->init.width, window->init.height, window->init.bitsPerPixel, window->init.frequency) )
		{
			/* Режим Fullscreen не проканал.  Запускаемся в оконно тогда */
			MessageBox (HWND_DESKTOP, "Mode Switch Failed.\nRunning In Windowed Mode.", "Error", MB_OK | MB_ICONEXCLAMATION);

			/* Установим isFullscreen в False (Оконный режим) */
			window->init.isFullScreen = FALSE;
		}
		else /* Иначе (Если Fullscreen режим установился) */
		{
			/* Выключаем курсор мыши */
			ShowCursor (FALSE);

			/* Устанавливаем WindowStyle в WS_POPUP (Всплывающее окно) */
			windowStyle = WS_POPUP;

			/* Устанавливаем расширеный стиль окна в WS_EX_TOPMOST */
			windowExtendedStyle |= WS_EX_TOPMOST;

		}/* (Top окно покрывает всё другое собой) */
	}
	else /* Если Fullscreen небыл выбран */
	{
		/* Выравним окно до самых границ */
		AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);
	}

	/* Сделаем OpenGL окно */
	window->hWnd = CreateWindowEx (windowExtendedStyle/* Расширеный стиль */,
									/* Имя класса */
									window->init.application->className,
									/* Заголовок окна */
									window->init.title,
									/* Стиль окна */
									windowStyle,
									/* X,Y позиция окна */
									0, 0,
									/* Ширина окна */
									windowRect.right - windowRect.left,
									/* Высота окна */
									windowRect.bottom - windowRect.top,
									/* Родитель окна рабочий стол */
									HWND_DESKTOP,
									/* Нет меню */
									0,
									/* Прошел Window Instance */
									window->init.application->hInstance, window);

	/* Окно создалось успешно? */
	if (0 == window->hWnd)	
	{
		/* Если нет, то возращаем False */
		return FALSE;
	}

	/* Созраняем контекст устройства для этого окна */
	window->hDC = GetDC (window->hWnd);	

	/* Контекст устройства сохранился? */
	if (0 == window->hDC)
	{
		/* Jesli net, to Уничтожаем окно */
		DestroyWindow (window->hWnd);

		/* Обнуляем дискриптор окна */
		window->hWnd = 0;

		/* Возвращаем False */
		return FALSE;
	}

	/* Вывод в консоль сообщения */
	MainConsole.Add(0,"Создано окно %dx%dx%d, полноэкран: %s.",	MainSettings.SCREEN_WIDTH,	MainSettings.SCREEN_HEIGHT,	MainSettings.BPP,	(MainSettings.FULLSCREEN ? "Да" : "Нет" ));

	/* Устанавливаем OpenGL, если нет то выходим. */
	if (!MainRender.SetupOpenGL(window))
		
		return FALSE;

	/* Сделаем окно видимым */
	ShowWindow (window->hWnd, SW_NORMAL);

	/* Установим isVisible в True */
	window->isVisible = TRUE;

	/* Подгоним наше окно */
	ReshapeGL (window->init.width, window->init.height);

	/* Очищаем все клавиши */
	ZeroMemory (window->keys, sizeof (Keys));

	/* Создание окна прошло успешно. A vot iнициализация закончится в WM_CREATE */
	return TRUE;							
}

/* Обработка сообщений */
LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/* Получаем контекст окна */
	GL_Window* window = (GL_Window*)(GetWindowLong (hWnd, GWL_USERDATA));

	/* Обрабатываем сообщение */
	switch (uMsg)
	{
		/* Перехватываем системную команду */
		case WM_SYSCOMMAND:
		{
			/* Проверяем системные вызовы */
			switch (wParam)	
			{
				/* Хранитель экрана пытается запуститься? */
				case SC_SCREENSAVE:	

				/* Монитор вытается уйти в спящий режим? */
				case SC_MONITORPOWER:

				/* Не надо этого всёго, пусть ничего не происходит */
				return 0;
			}
			break;
		}

		/* выходим из функции */
		return 0;

		/* Создание окна */
		case WM_CREATE:	
		{
			/* Сохраняем структуру окна в указателе */
			CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);

			window = (GL_Window*)(creation->lpCreateParams);

			SetWindowLong (hWnd, GWL_USERDATA, (LONG)(window));

			/* выходим из функции */
			return 0;
		}		

		/* Закрываем окно */
		case WM_CLOSE:

			/* Останавливаем приложение */
			TerminateApplication(window);

			/* выходим из функции */
			return 0;

		/* Обнаружена смена размера окна */
		case WM_SIZE:

			/* Opredelim какая */
			switch (wParam)
			{
				/* Окно минимизировано? */
				case SIZE_MINIMIZED:

					/* Устанавливаем isVisible в False */
					window->isVisible = FALSE;

					/* выходим из функции */
					return 0;

				/* Окно максимизировано? */
				case SIZE_MAXIMIZED:

					/* Устанавливаем isVisible в True */
					window->isVisible = TRUE;

					/* Подгоняем окно - LoWord=Width, HiWord=Height */
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));

					/* выходим из функции */
					return 0;

				/* Окно было востановлено? */
				case SIZE_RESTORED:

					/* Устанавливаем isVisible в True */
					window->isVisible = TRUE;

					/* Подгоняем - LoWord=Width, HiWord=Height */
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));

					/* выходим из функции */
					return 0;

			} /* switch (wParam) */
			
			/* end of big switch(), case WM_SIZE */
			break; 

		/* Обновляем буффер клавиатуры, нажата клавиша */
		case WM_KEYDOWN:

			if ((MainSettings.CONSOLE.STATUS!=NEW_CONSOLE_VISIBLE) && (MainSettings.CONSOLE.STATUS!=NEW_CONSOLE_FADEIN) )

				/* Клавиша (wParam) в правильном диапазоне? */
				if ((wParam >= 0) && (wParam <= 255))
				{
					/* Устанавливаем выбраную клавишу (wParam) в True */
					window->keys->keyDown [wParam] = TRUE;	
				}

			ProcessKeys(hWnd,uMsg,wParam,lParam);

			/* выход */
			break;

		/* Клавиша отпущена */
		case WM_KEYUP:

			if ((MainSettings.CONSOLE.STATUS!=NEW_CONSOLE_VISIBLE) && (MainSettings.CONSOLE.STATUS!=NEW_CONSOLE_FADEIN) )

				/* Клавиша (wParam) в правильном диапазоне? */
				if ((wParam >= 0) && (wParam <= 255))	
				{
					/* Устанавливаем выбраную клавишу (wParam) в False */
					window->keys->keyDown [wParam] = FALSE;	
				}

			/* выходим */
			break;

		/* Если введен символ */
		case WM_CHAR:

			/* Если консоль видима */
			if ((MainSettings.CONSOLE.STATUS==NEW_CONSOLE_VISIBLE) || (MainSettings.CONSOLE.STATUS==NEW_CONSOLE_FADEIN) )
			{
				/* Если символ видимый */
				if ((wParam>31)&&(wParam!='`'))

					/* Добавляем в командую строчку консоли */
					MainConsole.Current+=(char)wParam;
			}

			/* big switch(...)  */
			break;

		/* Меняем видео режим: FullScreen режим вкл./выкл. */
		case WM_TOGGLEFULLSCREEN:

			MainConsole.Add(0,"Смена видео режима (оконный/полноэкран)");

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

	/* Остальные сообщения отсылаем в DefWindowProc */
	return DefWindowProc (hWnd, uMsg, wParam, lParam);
}

/* Регистрирует класс окна для нашего приложения. Jesli vozvraschajet TRUE значит всё нормально */
BOOL RegisterWindowClass (Application* application)			
{																		
/* Класс окна. Регистрируем оконный класс */
WNDCLASSEX windowClass;	
	
	/* Убедимся что память очищена */
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));	
	
	/* Размер структуры windowClass */
	windowClass.cbSize			= sizeof (WNDCLASSEX);
	
	/* Перерисовываем окно при любых движениях и изменении размера */
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	
	/* дeксриптор WindowProc сообщений */
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				
	
	/* Устанавливаем Instance */
	windowClass.hInstance		= application->hInstance;				
	
	/* Класс цвета фона */
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			
	
	/* Загружаем стрелочку (курсор мыши) */
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			
	
	/* Устанавливаем имя класса приложения */
	windowClass.lpszClassName	= application->className;				

	/* Регистрация провалилась? */
	if (RegisterClassEx (&windowClass) == 0)							
	{
		/* УКАЗАНИЕ: Ошибка не должна никогда появляться */
		MessageBox (HWND_DESKTOP, "RegisterClassEx Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);

		/* Возвращаем False (Ошибка) */
		return FALSE;
	}

	/* Возвращаем True (Всё ок) */
	return TRUE;
}

/* Структура приложения */
Application			NEW_application;

/* Структура окна */
GL_Window			NEW_window;
	

/* Точка входа программы (WinMain) */
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
/* Сообщения активны? */
BOOL	isMessagePumpActive;

/* Структура оконных сообщений */
MSG		msg;
	
	/* Вывод в консоль сообщения */
	MainConsole.Add(0,"Engine стартовал...");

	InitFileSystem("");

	/* Инициализируем настройки программы and loading scene here */
	InitSettings();

	/* Вывод в консоль сообщения */
	MainConsole.Add(0,"Инициализация настроек прошла успешно.");

	/* Заполняем данные приложения: Имя класса приложения */
	NEW_application.className = "Engine";

	/* i Instance приложения */
	NEW_application.hInstance = hInstance;


	/* Заполняем окно. Pervim delom: Убедимся что память очищена */
	ZeroMemory (&NEW_window, sizeof (GL_Window));

	/* Структура клавиш */
	NEW_window.keys					= &MainInput.keys;

	/* Приложение */
	NEW_window.init.application		= &NEW_application;

	/* Заголовок окна */
	NEW_window.init.title			= (char *)MainSettings.TITLE.c_str();

	/* Ширина окна */
	NEW_window.init.width			= MainSettings.SCREEN_WIDTH;

	/* Высота окна */
	NEW_window.init.height			= MainSettings.SCREEN_HEIGHT;

	/* Битов на пиксель */
	NEW_window.init.bitsPerPixel	= MainSettings.BPP;

	/* Полноэкран? (Если TRUE) */
	NEW_window.init.isFullScreen	= MainSettings.FULLSCREEN;

	MainInput.Init();

	/* Регистрируем класс для нашего окна. Регистраци провалилась? */
	if (FALSE == RegisterWindowClass (&NEW_application))	
	{
		/* Ошибка. Вывод в консоль сообщения */
		MainConsole.Add(0,"Ошибка создания окна! Выходим.");

		/* Завершаем приложение */
		return -1;
	}
	/* Вывод в консоль сообщения */
	MainConsole.Add(0,"Создание окна прошло успешно.");

	/* Программный цикл устанавливаем в TRUE */
	g_isProgramLooping = TRUE;

	/* g_createFullScreen устанавливаем по выбраному */
	g_createFullScreen = NEW_window.init.isFullScreen;

	/* Инициализация опериций со временем (счетчики и т.д.) */
	newInitTimeOperation();

	/* Наш счетчик */
    newTimer myTimer;
	
	FirstStart=TRUE;

	/* Цикл пока WM_QUIT не получено */
	while (g_isProgramLooping)	
	{
		/* Создаём окно. Устанавливаем начальные параметры создания окна в Fullscreen? */
		NEW_window.init.isFullScreen = g_createFullScreen;

		/* Окно создалось успешно? */
		if (TRUE == CreateWindowGL (&NEW_window))	
		{
			/* И так мы имеем окно с уже инициализарованым OpenGL. Запускаем инициализацию игры */
			if (FALSE == Initialize (&NEW_window, &MainInput.keys, FirstStart))	
			{
				/* Ошибка. Закрываем окно, это повлечет закрытие приложения */
				TerminateApplication (&NEW_window);	
			}
			else /* Иначе (Стартуем обработку сообщений) */
			{
				/* Инициализация прошла успешно. Устанавливаем isMessagePumpActive в TRUE. */
				isMessagePumpActive = TRUE;

				/* Стартуем счетчи */
				myTimer.newStartTiming();
				
				/* Вывод в консоль сообщения */
				MainConsole.Add(0,"Старт главного цикла.");

				while (TRUE == isMessagePumpActive)
				{
					/* Удачное создание окна, проверяем сообщения */
					if (PeekMessage (&msg, NEW_window.hWnd, 0, 0, PM_REMOVE) != 0)
					{
						/* Проверяем на сообщение WM_QUIT. Это сообщение A WM_QUIT? */
						if (msg.message != WM_QUIT)
						{
							/* Если нет to Переводим сообщение */
							TranslateMessage(&msg);

							/* обработаем сообщение */
							DispatchMessage (&msg);
						}
						else /* Иначе (Если сообщение WM_QUIT) */
						{
							FirstStart=FALSE;

							/* Останавливаем обработку сообщений */
							isMessagePumpActive = FALSE;
						}
					}
					else /* Если нету сообщений */
					{
						/* Если окно не видимо */
						if (NEW_window.isVisible == FALSE)	
						{
							/* Окно минимизировано, ждем сообщений */
							WaitMessage ();	
						}
						else /* Если окно видимо */
						{
							/* Делаем основное. Останавливаем счетчик, прошедшее время запишется в myTimer.ElapsedTime */
							myTimer.newTimeElapsed();   

							/* Обновляем, двигаем и т.д. */
							Update (myTimer.ElapsedTime);

							/* Стартуем счетчик */
							myTimer.newStartTiming();

							/* Рисуем сцену */
							Draw ();

							/* Меняем буферы (Двойная буфферизация) */
							SwapBuffers (NEW_window.hDC);

							/* Увеличиваем номер кадра */
							MainRender.Frame_Num++;
							
							MainRender.Tri_accum += MainRender.Tri_per_frame;

							MainRender.Tri_per_frame = 0;
						}
					}
				}/* while ( isMessagePumpActive == TRUE )*/

				/* Вывод в консоль сообщения */
				MainConsole.Add(0,"Конец главного цикла.");

			} /* if Initialize (... */

			/* Приложение заканчивает работу, деинициализация данных. Определенное пользователем функция. */
			Deinitialize ();

			/* Уничтожаем активное окно */
			MainRender.DestroyWindowGL (&NEW_window);									
		}
		else /* eсли создание окна провалилось */
		{
			/* Ошибка создания окна */
			MessageBox (HWND_DESKTOP, "Error Creating OpenGL Window", "Error", MB_OK | MB_ICONEXCLAMATION);

			/* Завершаем цикл */
			g_isProgramLooping = FALSE;

		}

	}/* while (isProgramLooping) */

	/* Вывод в консоль сообщения */
	MainConsole.Add(0,"Удаление созданого...выход");

	/* Удаляем класс окна */
	UnregisterClass (NEW_application.className, NEW_application.hInstance);		

	/* Вывод в консоль сообщения */
	MainConsole.Add(0,"Engine закончил свою работу.");

	return 0;

} /* Конец WinMain() */

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
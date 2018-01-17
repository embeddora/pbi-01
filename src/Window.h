#ifndef __WINDOWS_H__
#define __WINDOWS_H__

#include "Settings.h"

/* Класс для будущего использования */
class NewWindow
{
public:

	NewWindow(void);

	~NewWindow(void);
};


/* Завершение программы */
void TerminateApplication (GL_Window* window);

/* Переключение Fullscreen / Windowed режим */
void ToggleFullscreen (GL_Window* window);

/* Эту функцию необходимо описать самому */
BOOL Initialize (GL_Window* window, Keys* keys, BOOL first);

/* Деинициализация */
void Deinitialize (void);

/* Расчет сцены в зависимости от прошедшего времени */
void Update (UINT64 &DeltaTime);

/* Отрисовка всей сцены */
void Draw (void);

/* Инициализация настроек */
void InitSettings();

/* Определяем номер сообщения для переключения полноэкрана */
#define WM_TOGGLEFULLSCREEN (WM_USER+1)

/* Обработка клавиш */
BOOL ProcessKeys(HWND hWnd, UINT message, WPARAM wParam = 0,  LPARAM lParam = 0 );

/* Структура приложения */
extern Application NEW_application;

/* Структура окна */
extern GL_Window NEW_window;

void newMouseToCenter();

#endif /* ifndef-define include guard */

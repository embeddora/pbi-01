#ifndef __WINDOWS_H__
#define __WINDOWS_H__

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
BOOL ProcessKeys(HWND hWnd, UINT message, /*WPARAM 15:03 18-01-2018 */ unsigned short  wParam = 0,  /* LPARAM 15:03 18-01-2018 */ unsigned long lParam = 0 );

/* Структура приложения */
extern Application NEW_application;

/* Структура окна */
extern GL_Window NEW_window;

void newMouseToCenter();

#endif /* ifndef-define include guard */

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

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

/* Типы */
#include "Types.h"

#include "Input.h"

typedef int HINSTANCE;

/* Содержит информацию жизненоважную для программы */
typedef struct
{
	/* Application Instance */
	HINSTANCE		hInstance;

	/* Application ClassName */
	const char*		className;

} Application;

/* Информация для создания окна */
typedef struct
{
	/* структура Application */
	Application*		application;

	/* Название окна */
	char*				title;

	/* Ширина */
	int					width;

	/* Высота */
	int					height;

	/* Бит на пиксель */
	int					bitsPerPixel;

	/* Полноэкранное? */
	BOOL				isFullScreen;

	/* Частота обновления монитора */
	int					frequency;

} GL_WindowInit;

typedef int HWND;
typedef int HDC;
typedef int HGLRC;


/* Содержит информацию жизненоважную для Window */
typedef struct
{	/* Структура для клавишей */
	Keys*				keys;

	/* Хэндл для окна  */
	HWND				hWnd;

	/* Контекст устройства */
	HDC					hDC;

	/* Контекст OpenGL */
	HGLRC				hRC;

	/* Инициализация окна */
	GL_WindowInit		init;

	/* Окно видимо? */
	BOOL				isVisible;

	/* Счетчик тиков */
	DWORD				lastTickCount;

} GL_Window;

/* Класс настроек */
class newSettings
{
public:

	newSettings(void);

	~newSettings(void);

	/* Название окна */
	newString TITLE;

	/* Ширина экрана */
	UINT SCREEN_WIDTH;

	/* Высота экрана */
	UINT SCREEN_HEIGHT;

	/* На полный экран? */
	BOOL FULLSCREEN;

	/*Глубина цвета (бит на пиксель) */
	BYTE BPP;

	/* Частота обновление монитора */
	int SCREEN_frequency;

	/* Битность Z-буффера */
	BYTE DEPTH;

	/* Угол обзора;	По умолчанию угол обзора считается по вертикали, но люди, играющие в Quake, привыкли к горизонтальному углу. В том числе и я.*/
	float FOV;

	/* Ближняя плоскость отсечения */
	float ZNEAR;

	/* Дальняя плоскость отсечения */
	float ZFAR;

	/* Консольные настройки */
	struct Con
	{
		/* Имя файла с текстурой */
		newString TEXTURE_FILE;

		/* Цвет */
		newColor4ub COLOR;

		/* Имя файла шрифта */
		newString FONT_FILE;

		/* Текущий статус */
		UINT STATUS;
		/* Скорость */
		float SPEED;

		/* Положение по вертикали */
		float YPOS;

	} CONSOLE;

	/* Настройки мыши */
	struct mouse
	{
		/* Чувсвительность */
		float Sensitivity;

		/* Коэффициенты по осям */
		float Pitch;

		float Yaw;
	} Mouse;

	/* Настройки рендера */
	struct render
	{
		/* Рисовать нормали */
		BOOL DrawNormals;

		/* Выводить информацию о FPS */
		BOOL DrawFPS;

		/* Выводить помощь */
		BOOL DrawHelp;
	} Render;

	/* Скорость камеры */
	float Camera_Speed;

	/* Загрузить настройки из файла */
	void LoadFromFile(const newString &Filename);
};

/* Глобальная переменная */
extern newSettings MainSettings;

#endif /* ifndef-define include guard */

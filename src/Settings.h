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

#include "Types.h"

#include "Input.h"

typedef int HINSTANCE;

typedef struct
{
	/* Application Instance */
	HINSTANCE		hInstance;

	/* Application ClassName */
	const char*		className;

} Application;

typedef struct
{
	Application*		application;

	char*			title;

	int			width;

	int			height;

	int			bitsPerPixel;

	BOOL			isFullScreen;

	int			frequency;

} GL_WindowInit;

typedef int HWND;

typedef int HDC;

typedef int HGLRC;


typedef struct
{
	Keys*			keys;

	HWND			hWnd;

	HDC			hDC;

	HGLRC			hRC;

	GL_WindowInit		init;

	BOOL			isVisible;

	DWORD			lastTickCount;

} GL_Window;

class newSettings
{
public:

	newSettings(void);

	~newSettings(void);

	newString TITLE;

	UINT SCREEN_WIDTH;

	UINT SCREEN_HEIGHT;

	BOOL FULLSCREEN;

	BYTE BPP;

	int SCREEN_frequency;

	BYTE DEPTH;

	float FOV;

	float ZNEAR;

	float ZFAR;

	struct Con
	{
		newString TEXTURE_FILE;

		newColor4ub COLOR;

		newString FONT_FILE;

		UINT STATUS;

		float SPEED;

		float YPOS;

	} CONSOLE;

	struct mouse
	{
		float Sensitivity;

		float Pitch;

		float Yaw;
	} Mouse;

	struct render
	{
		BOOL DrawNormals;

		BOOL DrawFPS;

		BOOL DrawHelp;
	} Render;

	float Camera_Speed;

	void LoadFromFile(const newString &Filename);
};

extern newSettings MainSettings;

#endif /* ifndef-define include guard */

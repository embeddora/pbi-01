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

#include "Input.h"

#include "Settings.h"

#include "Window.h"

#include "Render.h"

newInput MainInput;

newInput::newInput(void)
{
}

newInput::~newInput(void)
{
}

/* Declaration of memset (. . . ) */
#include <string.h>

/* Forge. Temporary. Etc... */
#define ZeroMemory(x, y)  memset(x, 0, y)


/* Initialization */
void newInput::Init()
{
	/* Cleaning keyboard structure */
	ZeroMemory (&keys, sizeof (Keys));

	/* Keys on the keyboard are not pressed */
	MouseButton[0]=MouseButton[1]=MouseButton[2]=FALSE;

	/* Switch key processing on */
	Enabled=TRUE;

	/* Toggle mouse movement flag off */
	MouseMoved=FALSE;
}

/* Обработка клавиатуры и мышки */
void newInput::ProcessInput()
{
	/* Если мышку подвинули за время прошлой обработки */
	if (MouseMoved)
	{
		/* Вычисляем смещение относительно центра */
		MousePos -= MouseCenter;

		/* Если смещение не нулевое */
		if (!MousePos.IsZero())
		{
			/* Умножаем на осевые коэффициенты */
			MousePos.X *= MainSettings.Mouse.Pitch;

			MousePos.Y *= MainSettings.Mouse.Yaw;

			/* Умножаем на чувтсвительность мыши */
			MousePos *= MainSettings.Mouse.Sensitivity;

			/* Увеличиваем углы эйлера у текущей камеры */
			MainRender.CurrentCamera->Pobj->rotEuler.Pitch += MousePos.Y;

			MainRender.CurrentCamera->Pobj->rotEuler.Yaw   += MousePos.X;

			/* Обрезаем, если вышли за PI */
			NEW_CUT_RANGE(0,MainRender.CurrentCamera->Pobj->rotEuler.Yaw,360);

			/* Урезаем, до между "перпендикулярно вверх" и "вниз" */
			NEW_LIMIT_RANGE(-89.9f,MainRender.CurrentCamera->Pobj->rotEuler.Pitch,89.9f);

			MainRender.CurrentCamera->Pobj->Rotation.FromEuler(MainRender.CurrentCamera->Pobj->rotEuler);
		}

		/* Plasing mouse to screen center */
		newMouseToCenter();

		/* Switching mouse movement flag off */
		MouseMoved=FALSE;
	}
}

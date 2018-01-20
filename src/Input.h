#ifndef __INPUT_H__
#define __INPUT_H__

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

#include "Math.h"

/* Структура клавиатуры */
typedef struct
{
	/* Хранит TRUE / FALSE для каждой клавиши */
	BOOL keyDown [256];

} Keys;

/* Класс управлением ввода */
class newInput
{
public:

	newInput(void);

	~newInput(void);

	/* Вкл./Выкл. */
	BOOL Enabled;

	/* Мышь подвинули */
	BOOL MouseMoved;

	/* Структура клавиш */
	Keys  keys;		

	/* Кнопки мыши */
	BOOL MouseButton[3];

	/* Положение мыши */
	newVector2f MousePos;

	/* Координаты центра экрана для мыши */
	newVector2f MouseCenter;

	/* Колесо мыши */
	newVector2f MouseWheel;

	/* Инициализация */
	void Init();

	/* Обработать ввод */
	void ProcessInput();
};

/* Глобальная переменная */
extern newInput MainInput;

#endif /* ifndef-define include guard */

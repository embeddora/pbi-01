#ifndef __GLLIGHT_H__
#define __GLLIGHT_H__

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

#include "Light.h"

class newGLLight: public newLight
{
	/* Индекс света в массиве OpenGL */
	UINT Index;

	/* Тип затухания: Констаннтный, Линейный, Квадратичный */
	UINT Att_type;

public:

	/* Конструктор */
	newGLLight(void);

	/* Диструктор */
	~newGLLight(void);

	/* Включаем свет */
	void Enable();

	/* Выключаем свет */
	void Disable();

	/* Применить */
	void Apply();

	/* Установка затухания */
	void SetAttenuation(newEnum att_type, float att);
};

#endif /* ifndef-define include guard */

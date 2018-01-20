#ifndef __FONT_H__
#define __FONT_H__

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

#include "Texture.h"

// Класс шрифта
class newFont
{
public:
	// Конструктор
	newFont(void)
	{
		Width=
		Height=1;
		Spacing=6;
		Color=newColor4ub(255,255,255,255);
		Spacing=0;
	}
	~newFont(void);
	// Название шрифта
	newString Name;
	// Ширина
	float Width;
	// Высота
	float Height;
	// Текстура
	newTexture *TextureLink;
	// Прозрачность
	newEnum BlendFunc;
	// Базовая
	UINT Base;
	// Растояние между буквами
	UINT Spacing;
	// Растояния до реальных букв
	BYTE FTrim[256][2];
	// Цвет
	newColor4ub Color;
	// Загрузка теустуры и нахождение пробелов между буквами
	// Zero - это пороговое значение. Ниже это ноль, выше есть пиксель.
	BOOL LoadTextureAndCheckSpace(const newString &FileName, const newString &FontName, BYTE Zero);
};

// Глобальный массив шрифтов
typedef std::map<newString,newFont> Fonts;

extern Fonts Fontchen;

// Новый шрифт
newFont *newNewFont(const newString &Name);
newFont *newDeleteFont(const newString &Name);
// Выбор шрифта
BOOL newSelectFont(const newString &Name);

BOOL newLoadFontAndCheckSpace(const newString &FileName, const newString &FontName, BYTE Zero);


#endif /* ifndef-define include guard */

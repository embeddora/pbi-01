#ifndef __TEXTURE_H__
#define __TEXTURE_H__

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

#define DEF_FILTER		10
#define DEF_TWO			2
#define DEF_DEF_NULL	0
#define DEF_MINUS_ONE	-1


#include "Types.h"

#include "Manager.h"

/* Класс текстуры */
class newTexture
{
public:
	newTexture(UINT filter = DEF_FILTER,  UINT generate_mipmap = DEF_TWO)
	{
		Filter		=	DEF_FILTER;

		Width		=	DEF_DEF_NULL;

		Height		=	DEF_DEF_NULL;

		TextureID	=	DEF_MINUS_ONE;

		Bpp			=	DEF_DEF_NULL;

		Border_Type	=	NEW_TEXTURE_BORDER_REPEAT;

		Type		=	NEW_TEXTURE_DIFFUSE;

		isBRGA		=	FALSE;

		isLoaded	=	FALSE;

		GenerateMipMap = generate_mipmap;
	}

	~newTexture(void);

    /* Пиксели текстуры */
	std::vector<BYTE> Data;

	/* Способ фильтрации текстуры */
	BYTE Filter;

	/* Бит на пиксель */
	BYTE Bpp;

	/* Имя текстуры */
	newString Name;

	/* Ширина текстуры */
	UINT Width;

	/* Высота текстуры */
	UINT Height;

	/* Индекс текстуры в видео памяти */
	UINT TextureID;

	/* Тип границ */
	newEnum Border_Type;

	/* Тип текстуры */
	newEnum Type;

	/* Другая последовательность каналов */
	BOOL isBRGA;

	/* Текстура загружена? */
	BOOL isLoaded;

	/* Генерировать мипмапы: (0 - нет, 1 - да, но в руками, 2 - да, автоматическая генерация) */
	UINT GenerateMipMap;	

	/* Создаёт текстуру, заполненную цветом в 3 компоненты */
	void CreateEmpty(int width,int height, newColor3ub &c);

	/* Создаёт текстуру, заполненную цветом в 4 компоненты */
	void CreateEmpty4(int width,int height, newColor4ub &c);

	/* Загружаем из файла данные в зависимости от расширения */
	newResult LoadFromFile(const newString &FileName);

	/* Загружаем TGA файл */
	newResult LoadTGAFile(const newString &FileName);

	/* Загружаем JPG файл */
	newResult LoadJPGFile(const newString &FileName);

	/* Загружает альфа канал из файла */
	newResult LoadAlphaFromFile(const newString &FileName);

	/* Включить текстуру */
	void Enable();

	/* Загрузить текстуру в видеопамять. */
	newResult Upload();

private:
	void* memcpy(void*, const void*, int);
};

/* Менеджер текстур */
extern newManager<newTexture> TextureMan;

/* Определение короткого имени */
typedef newManager<newTexture>::Items Textures;

/* Ссылка на элементы менеджера текстур */
extern Textures &Texture;

/* Запись экрана в файл JPG; Используется для создания ScreenShot'ов */
//BOOL newSaveScreenToJPG();


#endif /* ifndef-define include guard */

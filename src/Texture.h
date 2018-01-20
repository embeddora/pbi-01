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

#define DEF_FILTER	10
#define DEF_TWO		2
#define DEF_DEF_NULL	0
#define DEF_MINUS_ONE	-1


#include "Types.h"

#include "Manager.h"

class newTexture
{
public:
	newTexture(UINT filter = DEF_FILTER,  UINT generate_mipmap = DEF_TWO)
	{
		Filter		=	DEF_FILTER;

		Width		=	DEF_DEF_NULL;

		Height		=	DEF_DEF_NULL;

		TextureID	=	DEF_MINUS_ONE;

		Bpp		=	DEF_DEF_NULL;

		Border_Type	=	NEW_TEXTURE_BORDER_REPEAT;

		Type		=	NEW_TEXTURE_DIFFUSE;

		isBRGA		=	FALSE;

		isLoaded	=	FALSE;

		GenerateMipMap = generate_mipmap;
	}

	~newTexture(void);

	std::vector<BYTE> Data;

	BYTE Filter;

	BYTE Bpp;

	newString Name;

	UINT Width;

	UINT Height;

	UINT TextureID;

	newEnum Border_Type;

	newEnum Type;

	BOOL isBRGA;

	BOOL isLoaded;

	UINT GenerateMipMap;	

	void CreateEmpty(int width,int height, newColor3ub &c);

	void CreateEmpty4(int width,int height, newColor4ub &c);

	newResult LoadFromFile(const newString &FileName);

	newResult LoadTGAFile(const newString &FileName);

	newResult LoadJPGFile(const newString &FileName);

	newResult LoadAlphaFromFile(const newString &FileName);

	void Enable();

	newResult Upload();

private:
	void* memcpy(void*, const void*, int);
};

extern newManager<newTexture> TextureMan;

typedef newManager<newTexture>::Items Textures;

extern Textures &Texture;

//BOOL newSaveScreenToJPG();

#endif /* ifndef-define include guard */

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

class newFont
{
public:
	newFont(void)
	{
		Width =	Height = 1;

		Spacing=6;

		Color=newColor4ub(255,255,255,255);

		Spacing=0;
	}

	~newFont(void);

	newString Name;

	float Width;

	float Height;

	newTexture *TextureLink;

	newEnum BlendFunc;

	UINT Base;

	UINT Spacing;

	BYTE FTrim[256][2];

	newColor4ub Color;

	BOOL LoadTextureAndCheckSpace(const newString &FileName, const newString &FontName, BYTE Zero);
};

typedef std::map<newString,newFont> Fonts;

extern Fonts Fontchen;


newFont *newNewFont(const newString &Name);

newFont *newDeleteFont(const newString &Name);

BOOL newSelectFont(const newString &Name);

BOOL newLoadFontAndCheckSpace(const newString &FileName, const newString &FontName, BYTE Zero);


#endif /* ifndef-define include guard */

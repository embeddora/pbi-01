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

typedef struct
{
	BOOL keyDown [256];

} Keys;

class newInput
{

public:

	newInput(void);

	~newInput(void);

	BOOL Enabled;

	BOOL MouseMoved;

	Keys  keys;		

	BOOL MouseButton[3];

	newVector2f MousePos;

	newVector2f MouseCenter;

	newVector2f MouseWheel;

	void Init();

	void ProcessInput();
};

extern newInput MainInput;

#endif /* ifndef-define include guard */

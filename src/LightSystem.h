#ifndef __LIGHTSYSTEM_H__
#define __LIGHTSYSTEM_H__

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

#include "Light.h"

class newLightSystem
{
public:

	newLightSystem(void);

	~newLightSystem(void);

	void Init();

	newLight * AddLight(newString Name, newEnum ltype = NEW_LIGHT_AUTODETECT);

	newLight * GetLight(newString Name);

	void DeleteLight(newString Name);

	newManager<newLight *> LightMan;
};

extern newLightSystem LightSystem;

#endif /* ifnde-define include guard */

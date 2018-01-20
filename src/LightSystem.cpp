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

#include "LightSystem.h"

#include "Render.h"

#include "Console.h"

newLightSystem LightSystem;

newLightSystem::newLightSystem(void)
{
}

newLightSystem::~newLightSystem(void)
{
}

void newLightSystem::Init()
{
	MainRender.InitLights();

	MainConsole.Add(0,"Система освещения инициализирована.");
}

newLight * newLightSystem::AddLight(newString Name, newEnum ltype)
{
	newLight *result = NULL;

	LightMan.newNewPItem(Name);

	result = MainRender.CreateLight(ltype);

	result->Name = Name;

	LightMan.Item[Name] = result;

	return result;
}

newLight * newLightSystem::GetLight(newString Name)
{
	if (LightMan.Item.find(Name) != LightMan.Item.end())
	{
		return LightMan.Item[Name];
	}
	else return NULL;
}
void newLightSystem::DeleteLight(newString Name)
{
	if ( LightMan.Item.find(Name) != LightMan.Item.end())
	{
		LightMan.Item.erase(Name);
	}
}

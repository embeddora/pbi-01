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

newLight::newLight(void)
{
	PObj = PhysicObjectMan.newNewItem();

	InfiniteLight = FALSE;
}

newLight::~newLight(void)
{
	/* PhysicObjectMan.Release(PObj); */
}

void newLight::SetAmbientColor(newVector4f col)
{
	AmbientColor = col;
}
newVector4f newLight::GetAmbientColor()
{
	return AmbientColor;
}

void newLight::SetDiffuseColor(newVector4f col)
{
	DiffuseColor = col;
}
newVector4f newLight::GetDiffuseColor()
{
	return DiffuseColor;
}

void newLight::SetSpecularColor(newVector4f col)
{
	SpecularColor = col;
}
newVector4f newLight::GetSpecularColor()
{
	return SpecularColor;
}
void newLight::SetInfinite(BOOL inf)
{
	InfiniteLight = inf;
}
BOOL newLight::GetInfinite()
{
	return InfiniteLight;
}

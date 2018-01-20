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

#include "GLLight.h"

#include <GL/gl.h>

const UINT MAX_LIGHT = 8;

const GLenum LightEnum[MAX_LIGHT] = {GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};

BOOL LightIndex[MAX_LIGHT] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

newGLLight::newGLLight(void)
{
	for (int i=0; i<MAX_LIGHT; i++)

		if (!LightIndex[i])
		{
			LightIndex[i] = TRUE;

			Index = i;

			break;
		}

	DiffuseColor = newVector4f(1,1,1,1);

	AmbientColor = newVector4f(0,0,0,1);

	SpecularColor = newVector4f(1,1,1,1);
}

newGLLight::~newGLLight(void)
{
}

void newGLLight::Enable()
{
	glEnable(GL_LIGHTING);

	glEnable(LightEnum[Index]);
}

void newGLLight::Disable()
{
	glDisable(LightEnum[Index]);
}


void newGLLight::Apply()
{
	if (MAX_LIGHT-Index<1)
		
		return;

	newVector4f Pos(PObj->Position);

	Pos.W = InfiniteLight?0.0f:1.0f;

	glLightfv(LightEnum[Index], GL_POSITION, PObj->Position);

	glLightfv(LightEnum[Index], GL_DIFFUSE, DiffuseColor);

	glLightfv(LightEnum[Index], GL_AMBIENT, AmbientColor);

	glLightfv(LightEnum[Index], GL_SPECULAR, SpecularColor);
}

void newGLLight::SetAttenuation(newEnum att_type, float att)
{
	Attenuation = att;

	switch (att_type)
	{
		case NEW_LIGHT_ATT_CONSTANT:

			Att_type = GL_CONSTANT_ATTENUATION;
			
			break;

		case NEW_LIGHT_ATT_LINEAR:
			
			Att_type = GL_LINEAR_ATTENUATION;
			
			break;

		case NEW_LIGHT_ATT_QUADRATIC:
			
			Att_type = GL_QUADRATIC_ATTENUATION ;
			
			break;
	}
	glLightf(LightEnum[Index], Att_type, Attenuation);
}

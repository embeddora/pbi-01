#ifndef __LIGHT_H__
#define __LIGHT_H__

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

#include "PhysicObject.h"

class newLight: public newBaseObject
{
public:
	
	newLight(void);

	virtual ~newLight(void) = 0;

	virtual void Enable() = 0;

	virtual void Disable() = 0;

	virtual void Apply() = 0;

	void SetPosition(newVector3f pos);

	newVector3f GetPosition();

	void SetDiffuseColor(newVector4f col);

	newVector4f GetDiffuseColor();

	void SetAmbientColor(newVector4f col);

	newVector4f GetAmbientColor();

	void SetSpecularColor(newVector4f col);

	newVector4f GetSpecularColor();

	void SetInfinite(BOOL inf);

	BOOL GetInfinite();

	virtual void SetAttenuation(newEnum att_type, float att) = 0;
	
/* private: */
	newPhysicObject *PObj;

	newVector4f DiffuseColor;

	newVector4f AmbientColor;

	newVector4f SpecularColor;

	BOOL InfiniteLight;

	float Attenuation;
};

#endif /* ifndef-define include guard */

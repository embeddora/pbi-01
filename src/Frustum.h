#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

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

class newFrustum
{
protected:

	float FoV;

	float zNear;

	float zFar;

public:

	newFrustum(void);

	~newFrustum(void);

	BOOL Reshape;

	float GetFoV() {return FoV;}

	float GetzNear() {return zNear;}

	float GetzFar() {return zFar;}

	void SetFoV(float fov) {FoV=fov;Reshape=TRUE;}

	void SetzNear(float znear) {zNear=znear;Reshape=TRUE;}

	void SetzFar(float zfar) {zFar=zfar;Reshape=TRUE;}
};

#endif /* ifndef-define include guard */

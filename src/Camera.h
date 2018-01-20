#ifndef __CAMERA_H__
#define __CAMERA_H__

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

#include "Physic.h"

#include "Frustum.h"

class newCamera: public newBaseObject
{
public:
	newCamera(void);

	~newCamera(void);

	newString Name;

	newEnum CamStyle;

	void SetCameraStyle(newEnum style);

	/* newEnum GetCameraStyle() */

	newFrustum Frustum;

	newPhysicObject *Pobj;

	void AttachTo(newPhysicObject *obj) {Pobj=obj;}

	void AttachTo(const newString &PobjName) {Pobj=&PhysicObject[PobjName];}

	newVector3f DeltaSpeed;

	void Move(newEnum dir,float speed);

	void ApplyTransform(UINT64 &DeltaTime);
};

typedef std::map<newString, newCamera> Cameras;

extern Cameras Camera;

newCamera *newNewCamera(const newString &Name);

newString newGenCameraName();

#endif /* ifndef-define include guard */

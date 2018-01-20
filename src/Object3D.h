#ifndef __OBJECT3D_H__
#define __OBJECT3D_H__

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

#include "PhysicObject.h"

#include "Material.h"

class newObject3D: public newBaseObject
{
private:
	
public:
	newObject3D(void);

	~newObject3D(void);

	newMesh *Mesh;

	newPhysicObject *Pobj;

	BOOL MotionBlur;

	BOOL CastShadows;

	BOOL RecvShadows;

	newMaterial *Mtl;
	
	newRenderObject RObj;

	void TransformToStatic(newPhysicObject &po);

	void BornMesh(const newString &Name);

	void BornPhysicObject(const newString &Name);

	void PreCompute();

	void Render();
};

/* Менеджер */
extern newManager<newObject3D> Object3DMan;

/* Определение короткого имени */
typedef newManager<newObject3D>::Items Object3Ds;

/* Ссылка на элементы менеджера */
extern Object3Ds &Object3D;

#endif /* ifdef-define include guard */

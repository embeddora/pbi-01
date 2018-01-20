#ifndef __PHYSICOBJECT_H__
#define __PHYSICOBJECT_H__

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

#include "Mesh.h"

#include "Quaternion.h"

/* Объект физики */
class newPhysicObject: public newBaseObject
{
public:
	newPhysicObject(void);

	~newPhysicObject(void);
	
	/* Позиция */
	newVector3f Position;

	/* Поворот */
	newQuaternionf Rotation;

	/* Скорость */
	newVector3f Velocity;

	/* Угловое положение */
	newEulerAngle rotEuler;

	/* Геометрическое представление объекта */
	newMesh *Mesh;
};

/* Менеджер */
extern newManager<newPhysicObject> PhysicObjectMan;

/* Определение короткого имени */
typedef newManager<newPhysicObject>::Items PhysicObjects;

/* Ссылка на элементы менеджера */
extern PhysicObjects &PhysicObject;

#endif /* ifndef-define include guard */

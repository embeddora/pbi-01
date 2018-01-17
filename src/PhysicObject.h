#ifndef __PHYSICOBJECT_H__
#define __PHYSICOBJECT_H__

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

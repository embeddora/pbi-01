#ifndef __PHYSICOBJECT_H__
#define __PHYSICOBJECT_H__

#include "Types.h"

#include "Math.h"

#include "Mesh.h"

#include "Quaternion.h"

/* ������ ������ */
class newPhysicObject: public newBaseObject
{
public:
	newPhysicObject(void);

	~newPhysicObject(void);
	
	/* ������� */
	newVector3f Position;

	/* ������� */
	newQuaternionf Rotation;

	/* �������� */
	newVector3f Velocity;

	/* ������� ��������� */
	newEulerAngle rotEuler;

	/* �������������� ������������� ������� */
	newMesh *Mesh;
};

/* �������� */
extern newManager<newPhysicObject> PhysicObjectMan;

/* ����������� ��������� ����� */
typedef newManager<newPhysicObject>::Items PhysicObjects;

/* ������ �� �������� ��������� */
extern PhysicObjects &PhysicObject;

#endif /* ifndef-define include guard */

#ifndef __PHYSIC_H__
#define __PHYSIC_H__

#include "PhysicObject.h"

/* ����� ������ */
class newPhysic
{
public:
	newPhysic(void);

	~newPhysic(void);

	/* ���������� */
	newVector3f Gravity;
};

/* ���������� ���������� */
extern newPhysic MainPhysic;

#endif /* ifndef-define include guard */

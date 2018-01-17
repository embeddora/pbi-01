#ifndef __PHYSIC_H__
#define __PHYSIC_H__

#include "PhysicObject.h"

/* Класс физики */
class newPhysic
{
public:
	newPhysic(void);

	~newPhysic(void);

	/* Гравитация */
	newVector3f Gravity;
};

/* Глобальная переменная */
extern newPhysic MainPhysic;

#endif /* ifndef-define include guard */

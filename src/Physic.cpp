#include "Physic.h"

#include "Settings.h"

newPhysic MainPhysic;

/* Physics constructor */
newPhysic::newPhysic(void)
{
	/* Gravitation */
	Gravity.Set(0,-9.8f,0);
}

/* Physics destructor */
newPhysic::~newPhysic(void)
{

}

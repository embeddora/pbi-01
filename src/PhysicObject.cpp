#include "PhysicObject.h"

newManager<newPhysicObject> PhysicObjectMan;

PhysicObjects &PhysicObject = PhysicObjectMan.Item;

/* Physical object constructor */
newPhysicObject::newPhysicObject(void)
{
	/* Setting Euler angler to zero */
	rotEuler.Pitch = rotEuler.Yaw = rotEuler.Roll = 0;
}

/* Physical object destructor */
newPhysicObject::~newPhysicObject(void)
{

}

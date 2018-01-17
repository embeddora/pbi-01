#include "Object3d.h"

#include "Console.h"

newManager<newObject3D> Object3DMan;

Object3Ds &Object3D = Object3DMan.Item;

newObject3D::newObject3D(void)
{

}

newObject3D::~newObject3D(void)
{

}

void newObject3D::BornMesh(const newString &Name)
{
	Mesh = g_MeshMan.newNewItem(Name);
}

void newObject3D::BornPhysicObject(const newString &Name)
{
	Pobj = PhysicObjectMan.newNewItem(Name);
}

void newObject3D::PreCompute()
{
	if (Mesh!=NULL)
	{
		RObj.Construct(*Mesh);

		TransformToStatic(*Pobj);
	}
}

void newObject3D::Render()
{
	RObj.Render();
}

void newObject3D::TransformToStatic(newPhysicObject &po)
{
	RObj.TransformToStatic(po);
}

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

#include "Object3D.h"

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

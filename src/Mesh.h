#ifndef __MESH_H__
#define __MESH_H__

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

#include "Manager.h"

#include "RenderObject.h"

struct newVertex
{
	newVector3f Coord;

	newVector3f Normal;
};

struct newFace
{
	UINT A,B,C;

	UINT TV[3];

	DWORD MeshSmooth;

	UINT SubMtlID;

	int Link[3];

	newVector3f Normal;
};

struct newTVertex
{
	newVector2f UV;
};

struct newTVFace
{
	UINT A,B,C;
};


class newMesh: public newBaseObject
{
public:
	newMesh(void);

	~newMesh(void);
	
	newResult LoadFromFile(const newString &FileName);

	newResult LoadMeshFromAseFile(FILE * fpIn, char * cpNodeName);

	void PreCompute();

	void Render();

	std::vector<newVertex>	    Vertex;

	typedef std::vector<newVertex> Vertex_iter;

	std::vector<newTVertex>		TVertex;

	std::vector<newFace>		Face;

	std::vector<newTVFace>		TVFace;

	typedef std::vector<newFace> Face_iter;

	BOOL isLoaded;

private:

	BOOL PreComputed;
};

extern newManager<newMesh> g_MeshMan;

typedef newManager<newMesh>::Items Meshs;

extern Meshs &g_Mesh;

#endif /* ifndef-define include guard */

#ifndef __SCENE_H__
#define __SCENE_H__

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

#include "Object3D.h"

#include "File.h"

class newScene: public newBaseObject
{
public:
	newScene(void);

	~newScene(void);

	newVector3f StaticBGColor;

	newVector3f StaticAmbient;

	std::vector<newMaterial *> MatList;

	std::list<newObject3D *> ObjList;

	newResult LoadFromFile(newString FileName);
	newResult LoadSceneFromAseFile(newString FileName);
	
	newResult LoadMaterial(newString FileName, UINT mtlRef);
//	newResult LoadMaterialFromAseFile(FILE * fpIn, char * cpFileName, UINT iIdx);
	newResult LoadMaterialFromAseFile(FILE * fpIn);

	newResult LoadObject3D(newString FileName, newFile &sfile);	
//	newResult LoadObject3dObjectFromAseFile(char * cpNodeName, FILE * fpIn, int iMatCountRes);
	newResult LoadObject3dObjectFromAseFile(FILE * fpIn, char * cpNodeName, int iMatCountRes);

	void Render();

	void PreCompute();

	void SortObjects();
};

#endif /* ifndef-define include guard */

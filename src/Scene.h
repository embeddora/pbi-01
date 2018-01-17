#ifndef __SCENE_H__
#define __SCENE_H__

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

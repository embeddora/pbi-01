#ifndef __MESH_H__
#define __MESH_H__

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

/* Менеджер */
extern newManager<newMesh> g_MeshMan;

/* Определение короткого имени */
typedef newManager<newMesh>::Items Meshs;

/* Ссылка на элементы менеджера */
extern Meshs &g_Mesh;

#endif /* ifndef-define include guard */

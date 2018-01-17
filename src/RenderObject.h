#ifndef __RENDEROBJECT_H__
#define __RENDEROBJECT_H__

#include "Types.h"

#include "Math.h"

class newPhysicObject;

class newMesh;

struct newRenderVertex
{
	newVector3f Coord;

	newVector3f Color;

	newVector3f Normal;

	newVector2f UV;
};

class newRenderObject
{
private:
	newPhysicObject *PObj;
public:
	std::vector<newRenderVertex> RVertex;

	std::vector<UINT> RIndex;

	UINT ListID;

	newRenderObject(void);

	~newRenderObject(void);

	void Construct(const newMesh &m);

	void Render();

	void DumpDraw();

	void TransformToStatic(newPhysicObject &po);
};

/*
// �������� 
extern newManager<newRenderObject> g_RenderObjectMan;
// ����������� ��������� �����
typedef newManager<newRenderObject>::Items RenderObjects;
// ������ �� �������� ���������
extern RenderObjects &g_RenderObject;
*/

#endif /* ifndef-define include guard */

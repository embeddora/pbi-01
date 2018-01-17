#ifndef __OBJECT3D_H__
#define __OBJECT3D_H__

#include "Types.h"

#include "Math.h"

#include "PhysicObject.h"

#include "Material.h"

class newObject3D: public newBaseObject
{
private:
	
public:
	newObject3D(void);

	~newObject3D(void);

	newMesh *Mesh;

	newPhysicObject *Pobj;

	BOOL MotionBlur;

	BOOL CastShadows;

	BOOL RecvShadows;

	newMaterial *Mtl;
	
	newRenderObject RObj;

	void TransformToStatic(newPhysicObject &po);

	void BornMesh(const newString &Name);

	void BornPhysicObject(const newString &Name);

	void PreCompute();

	void Render();
};

/* Менеджер */
extern newManager<newObject3D> Object3DMan;

/* Определение короткого имени */
typedef newManager<newObject3D>::Items Object3Ds;

/* Ссылка на элементы менеджера */
extern Object3Ds &Object3D;

#endif /* ifdef-define include guard */

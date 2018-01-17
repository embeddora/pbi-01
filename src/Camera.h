#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Types.h" //newtypes

#include "Math.h" //newmath

#include "Physic.h"

#include "Frustum.h"

// ����� ������
class newCamera: public newBaseObject
{
public:
	newCamera(void);

	~newCamera(void);

	/* ��� ������ */
	newString Name;

	/* ����� ������ */
	newEnum CamStyle;

	/* ������� ��������� ����� ������ */
	void SetCameraStyle(newEnum style);

	/* ������� ������������ ����� ������ !������ �� ��������! */
	/* newEnum GetCameraStyle() */

	/* ������� */
	newFrustum Frustum;

	/* ������ �� ���������� ������ */
	newPhysicObject *Pobj;

	/* ������������ ���������� ������ � ������� ����������� ������� �� ������ */
	void AttachTo(newPhysicObject *obj) {Pobj=obj;}

	/* ������������ ���������� ������ � ������� ����������� ������� �� ����� */
	void AttachTo(const newString &PobjName) {Pobj=&PhysicObject[PobjName];}

	/* ��������, ������� ���������� � ������� ��������� */
	newVector3f DeltaSpeed;

	/* ���������. ������� ���������: ����������� � �������� */
	void Move(newEnum dir,float speed);

	/* ��������� �������� � ������� */
	void ApplyTransform(UINT64 &DeltaTime);
};

/* ��� ����� ����� */
typedef std::map<newString, newCamera> Cameras;

/* ���������� ������ ����� */
extern Cameras Camera;

/* ����� ������ */
newCamera *newNewCamera(const newString &Name);

/* ���������� ��� ������ */
newString newGenCameraName();

#endif /* ifndef-define include guard */

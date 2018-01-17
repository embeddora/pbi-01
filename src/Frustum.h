#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

#include "Types.h"

/* ����� �������� */
class newFrustum
{
protected:

	/* ���� ������ */
	float FoV;

	/* ������� ��������� ��������� */
	float zNear;

	/* ������� ��������� ��������� */
	float zFar;

public:

	newFrustum(void);

	~newFrustum(void);

	/* ���� ���������, ���� TRUE, ��������� ������� � ���� */
	BOOL Reshape;

	/* ���������� ���� ������ */
	float GetFoV() {return FoV;}

	/* ���������� ������� ��������� */
	float GetzNear() {return zNear;}

	/* ���������� ������� ��������� */
	float GetzFar() {return zFar;}

	/* ������������� ���� ������ */
	void SetFoV(float fov) {FoV=fov;Reshape=TRUE;}

	/* ������������� ������� ��������� */
	void SetzNear(float znear) {zNear=znear;Reshape=TRUE;}

	/* ������������� ������� ��������� */
	void SetzFar(float zfar) {zFar=zfar;Reshape=TRUE;}
};

#endif /* ifndef-define include guard */

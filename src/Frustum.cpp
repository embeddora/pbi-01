#include "Frustum.h"

/* ����������� �������� */
newFrustum::newFrustum(void)
{
	/* ���� ������ 120 �������� */
	FoV=120;

	/* ������� ��������� */
	zNear=1;

	/* ������� ��������� */
	zFar=100;

	/* ���������? */
	Reshape=TRUE;
}

newFrustum::~newFrustum(void)
{
}

#ifndef __GLLIGHT_H__
#define __GLLIGHT_H__

#include "Light.h"

class newGLLight: public newLight
{
	/* ������ ����� � ������� OpenGL */
	UINT Index;

	/* ��� ���������: ������������, ��������, ������������ */
	UINT Att_type;

public:

	/* ����������� */
	newGLLight(void);

	/* ���������� */
	~newGLLight(void);

	/* �������� ���� */
	void Enable();

	/* ��������� ���� */
	void Disable();

	/* ��������� */
	void Apply();

	/* ��������� ��������� */
	void SetAttenuation(newEnum att_type, float att);
};

#endif /* ifndef-define include guard */

#ifndef __INPUT_H__
#define __INPUT_H__

#include "Types.h"

#include "Math.h"

/* ��������� ���������� */
typedef struct
{
	/* ������ TRUE / FALSE ��� ������ ������� */
	BOOL keyDown [256];

} Keys;

/* ����� ����������� ����� */
class newInput
{
public:

	newInput(void);

	~newInput(void);

	/* ���./����. */
	BOOL Enabled;

	/* ���� ��������� */
	BOOL MouseMoved;

	/* ��������� ������ */
	Keys  keys;		

	/* ������ ���� */
	BOOL MouseButton[3];

	/* ��������� ���� */
	newVector2f MousePos;

	/* ���������� ������ ������ ��� ���� */
	newVector2f MouseCenter;

	/* ������ ���� */
	newVector2f MouseWheel;

	/* ������������� */
	void Init();

	/* ���������� ���� */
	void ProcessInput();
};

/* ���������� ���������� */
extern newInput MainInput;

#endif /* ifndef-define include guard */

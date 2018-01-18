#include "Input.h"

#include "Settings.h"

#include "Window.h"

#include "Render.h"

newInput MainInput;

newInput::newInput(void)
{
}

newInput::~newInput(void)
{
}

/* Declaration of memset (. . . ) */
#include <string.h>

/* Forge. Temporary. Etc... */
#define ZeroMemory(x, y)  memset(x, 0, y)


/* Initialization */
void newInput::Init()
{
	/* Cleaning keyboard structure */
	ZeroMemory (&keys, sizeof (Keys));

	/* Keys on the keyboard are not pressed */
	MouseButton[0]=MouseButton[1]=MouseButton[2]=FALSE;

	/* Switch key processing on */
	Enabled=TRUE;

	/* Toggle mouse movement flag off */
	MouseMoved=FALSE;
}

/* ��������� ���������� � ����� */
void newInput::ProcessInput()
{
	/* ���� ����� ��������� �� ����� ������� ��������� */
	if (MouseMoved)
	{
		/* ��������� �������� ������������ ������ */
		MousePos -= MouseCenter;

		/* ���� �������� �� ������� */
		if (!MousePos.IsZero())
		{
			/* �������� �� ������ ������������ */
			MousePos.X *= MainSettings.Mouse.Pitch;

			MousePos.Y *= MainSettings.Mouse.Yaw;

			/* �������� �� ���������������� ���� */
			MousePos *= MainSettings.Mouse.Sensitivity;

			/* ����������� ���� ������ � ������� ������ */
			MainRender.CurrentCamera->Pobj->rotEuler.Pitch += MousePos.Y;

			MainRender.CurrentCamera->Pobj->rotEuler.Yaw   += MousePos.X;

			/* ��������, ���� ����� �� PI */
			NEW_CUT_RANGE(0,MainRender.CurrentCamera->Pobj->rotEuler.Yaw,360);

			/* �������, �� ����� "��������������� �����" � "����" */
			NEW_LIMIT_RANGE(-89.9f,MainRender.CurrentCamera->Pobj->rotEuler.Pitch,89.9f);

			MainRender.CurrentCamera->Pobj->Rotation.FromEuler(MainRender.CurrentCamera->Pobj->rotEuler);
		}

		/* Plasing mouse to screen center */
		newMouseToCenter();

		/* Switching mouse movement flag off */
		MouseMoved=FALSE;
	}
}

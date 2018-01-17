#ifndef __SETTINGS_H__
#define __SETTINGS_H__

/* ���� */
#include "Types.h"

#include "Input.h"

typedef int HINSTANCE;

/* �������� ���������� ������������� ��� ��������� */
typedef struct
{
	/* Application Instance */
	HINSTANCE		hInstance;

	/* Application ClassName */
	const char*		className;

} Application;

/* ���������� ��� �������� ���� */
typedef struct
{
	/* ��������� Application */
	Application*		application;

	/* �������� ���� */
	char*				title;

	/* ������ */
	int					width;

	/* ������ */
	int					height;

	/* ��� �� ������� */
	int					bitsPerPixel;

	/* �������������? */
	BOOL				isFullScreen;

	/* ������� ���������� �������� */
	int					frequency;

} GL_WindowInit;

typedef int HWND;
typedef int HDC;
typedef int HGLRC;


/* �������� ���������� ������������� ��� Window */
typedef struct
{	/* ��������� ��� �������� */
	Keys*				keys;

	/* ����� ��� ����  */
	HWND				hWnd;

	/* �������� ���������� */
	HDC					hDC;

	/* �������� OpenGL */
	HGLRC				hRC;

	/* ������������� ���� */
	GL_WindowInit		init;

	/* ���� ������? */
	BOOL				isVisible;

	/* ������� ����� */
	DWORD				lastTickCount;

} GL_Window;

/* ����� �������� */
class newSettings
{
public:

	newSettings(void);

	~newSettings(void);

	/* �������� ���� */
	newString TITLE;

	/* ������ ������ */
	UINT SCREEN_WIDTH;

	/* ������ ������ */
	UINT SCREEN_HEIGHT;

	/* �� ������ �����? */
	BOOL FULLSCREEN;

	/*������� ����� (��� �� �������) */
	BYTE BPP;

	/* ������� ���������� �������� */
	int SCREEN_frequency;

	/* �������� Z-������� */
	BYTE DEPTH;

	/* ���� ������;	�� ��������� ���� ������ ��������� �� ���������, �� ����, �������� � Quake, �������� � ��������������� ����. � ��� ����� � �.*/
	float FOV;

	/* ������� ��������� ��������� */
	float ZNEAR;

	/* ������� ��������� ��������� */
	float ZFAR;

	/* ���������� ��������� */
	struct Con
	{
		/* ��� ����� � ��������� */
		newString TEXTURE_FILE;

		/* ���� */
		newColor4ub COLOR;

		/* ��� ����� ������ */
		newString FONT_FILE;

		/* ������� ������ */
		UINT STATUS;
		/* �������� */
		float SPEED;

		/* ��������� �� ��������� */
		float YPOS;

	} CONSOLE;

	/* ��������� ���� */
	struct mouse
	{
		/* ��������������� */
		float Sensitivity;

		/* ������������ �� ���� */
		float Pitch;

		float Yaw;
	} Mouse;

	/* ��������� ������� */
	struct render
	{
		/* �������� ������� */
		BOOL DrawNormals;

		/* �������� ���������� � FPS */
		BOOL DrawFPS;

		/* �������� ������ */
		BOOL DrawHelp;
	} Render;

	/* �������� ������ */
	float Camera_Speed;

	/* ��������� ��������� �� ����� */
	void LoadFromFile(const newString &Filename);
};

/* ���������� ���������� */
extern newSettings MainSettings;

#endif /* ifndef-define include guard */

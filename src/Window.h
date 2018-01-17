#ifndef __WINDOWS_H__
#define __WINDOWS_H__

#include "Settings.h"

/* ����� ��� �������� ������������� */
class NewWindow
{
public:

	NewWindow(void);

	~NewWindow(void);
};


/* ���������� ��������� */
void TerminateApplication (GL_Window* window);

/* ������������ Fullscreen / Windowed ����� */
void ToggleFullscreen (GL_Window* window);

/* ��� ������� ���������� ������� ������ */
BOOL Initialize (GL_Window* window, Keys* keys, BOOL first);

/* ��������������� */
void Deinitialize (void);

/* ������ ����� � ����������� �� ���������� ������� */
void Update (UINT64 &DeltaTime);

/* ��������� ���� ����� */
void Draw (void);

/* ������������� �������� */
void InitSettings();

/* ���������� ����� ��������� ��� ������������ ����������� */
#define WM_TOGGLEFULLSCREEN (WM_USER+1)

/* ��������� ������ */
BOOL ProcessKeys(HWND hWnd, UINT message, WPARAM wParam = 0,  LPARAM lParam = 0 );

/* ��������� ���������� */
extern Application NEW_application;

/* ��������� ���� */
extern GL_Window NEW_window;

void newMouseToCenter();

#endif /* ifndef-define include guard */

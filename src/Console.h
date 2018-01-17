#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Types.h"

#include "Texture.h"

#include "Font.h"

struct newConCommand
{
	/* ��� ���������� ��� ������� */
	newEnum Type;

	/* ������ �� ���������� */
	void *Link;

	/* ����� ���������, ������� ����������� ����� ��������� �������. ���� NULL, �� �� �����������. */
	void (*Adress)(const newString &Command);
};

/* ���� ��� ������� */
typedef std::list<newString> Consoles;

typedef std::map<newString,newConCommand> CMDS;

/* ����� ������� */
class newConsole
{
public:

	newConsole(void);

	~newConsole(void);

	/* ������� �������? */
	BOOL IsDrawing;

	/* ������� ������� ��������� */
	UINT MessLevel;

	/* ������ ���� ���������� � ������ */
	CMDS CMD;

	/* �������, ������ ���� ��������� */
	Consoles Console;

	/* LOG ���� */
	FILE *conFile;

	/* ������ �� �������� */
	newTexture *Tex;

	/* ���������� ������� ��� ���������� */
	BOOL AddCMD(const newString &_Name, newEnum _Type, void *_Link, void (*_Adress)(const newString &Command) = NULL);

	/* ���������� ������� */
	BOOL ExecCMD(const newString &cmd);

	/* �������� � �������.  Level - ������� ��������� Message - ���� ��������� */
	void Add(UINT Level, const char *Message,...);

	/* ������� ������ � ������� */
	newString Current;

	/* ����� ������ ������ � �������, ������ ����� */
	UINT Line;

	/* ��������� ������ */
	void ProcessKey(DWORD Key);

	/* ������� ������ */
	Consoles CMDHistory;

	/* ������� ��������� � ������� ������ */
	Consoles::iterator HistoryPos;

	/* ��������� ������� �� ���������� ������ ������ */
	void AutoCompleteCMD();

	/* ������ ������� ����� */
	newStringList Cutten;

	/* ��������� ������ � ������ ��������� */
	void UpdateCut(const newString &src);

	/* ��������� ��� ������� ������ */
	void UpdateCutten();

	/* ����� ��� ������� */
	newFont *ConFont;
};

extern newConsole MainConsole;

#endif /* ifndef-define include guard */

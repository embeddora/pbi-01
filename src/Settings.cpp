#include "Settings.h"

#include "Console.h"

#include "File.h"

#include "Render.h"

/* ���������� ���������� */
newSettings MainSettings;

newSettings::newSettings(void)
{
	FOV = 0;

	/* �������� fps? */
	MainRender.ISfps= 0;

	/* ������ ������ */
	SCREEN_WIDTH= 640;

	/* ������ ������ */
	SCREEN_HEIGHT= 480;

	/* ��� �� ������� */
	BPP= 32;

	/* ����� �� ������� */
	DEPTH= 24;

	/* ����������? */
	FULLSCREEN= 0;

	/* ������ ������ */
	SCREEN_frequency= 120;

	/* ������� ��������� ��������� */
	ZNEAR = 1 ;

	/* ������� ��������� ��������� */
	ZFAR = 10000;

	/* ��������� ���� */
	TITLE= "Engine: Empty window";

	/* �������� ��� ������� */
	CONSOLE.TEXTURE_FILE= "";

	/* ���� ������� */
    CONSOLE.COLOR.RGBColor.a= 200 ;

	/* ����� ������� */
	CONSOLE.FONT_FILE= "";

	/* ��������� �� Y ������� */
	CONSOLE.YPOS= 480;

	/* �������� �������� ������� */
	CONSOLE.SPEED= 400;

	/* �������� �������� ������ */
	Camera_Speed= 500;

	/* ������������ ������� */
	Render.DrawNormals= 0;

	/* ������������ ������ */
	Render.DrawHelp= 0;
}

newSettings::~newSettings(void)
{
}

/* �������� �������� �� ����� */
void newSettings::LoadFromFile(const newString &Filename)
{
	MainConsole.Add(0,"�������� ������-�����: %s",Filename.c_str());

	newString cmd="";

	/* ��������� ����; FILE *file=fopen(Filename.c_str(),"r"); */
	newFile sfile;

	newResult res;

	if ( (res = sfile.Open(Filename, NEW_FILE_READ, FALSE)) != NEW_OK)
		
		return;
	/* ������ � �������� ��� ������� � ������� */
	while (sfile.ReadTextString(cmd))
	{
		if (!(cmd.substr(0,2)=="//"))

			MainConsole.ExecCMD(cmd);
	}

	MainConsole.Add(0,"������-���� %s ��������.",Filename.c_str());
}

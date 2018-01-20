/*
 * Copyright (C) 2018 ARNERI arneri@ukr.net All rights reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Abstract: 
 */

#include "Console.h"

#include "Settings.h"

#include "Render.h"

#include "Window.h"

#include <time.h>

// Prototypes va_start(..),  va_end(..)
#include <stdarg.h>

// Prototypes strlen(..)
#include <string.h>

/* ���������� ��������� */

/* Help - ������. ������� ������ ���� ������ � ����������. */
void conHelp(const newString &s)
{
	/* ����� �� ������� */
	MainConsole.Add(NEW_SYSMESS,"Egine ���������� ������:");

	MainConsole.Add(NEW_SYSMESS,"����������:");

	/* ����� �� ���� �������� � ��������� */
	for (CMDS::iterator it=MainConsole.CMD.begin();it!=MainConsole.CMD.end();it++)
	{
		/* � ����������� �� ���� */
		switch ((*it).second.Type)
		{
			case NEW_VARIABLE_BYTE:

				MainConsole.Add(NEW_SYSMESS,"%s <BYTE>",(*it).first.c_str());

				break;

			case NEW_VARIABLE_INT:

				MainConsole.Add(NEW_SYSMESS,"%s <INT>",(*it).first.c_str());

				break;

			case NEW_VARIABLE_UINT:

				MainConsole.Add(NEW_SYSMESS,"%s <UINT>",(*it).first.c_str());

				break;

			case NEW_VARIABLE_FLOAT:

				MainConsole.Add(NEW_SYSMESS,"%s <FLOAT>",(*it).first.c_str());

				break;

			case NEW_VARIABLE_BOOL:

				MainConsole.Add(NEW_SYSMESS,"%s <BOOL>",(*it).first.c_str());

				break;

			case NEW_VARIABLE_STRING:

				MainConsole.Add(NEW_SYSMESS,"%s <STRING>",(*it).first.c_str());

				break;
			
			default:

				break;
		}
	}

	/* ���� ����� � ��������� */
	MainConsole.Add(NEW_SYSMESS,"�������:");

	for (CMDS::iterator it=MainConsole.CMD.begin();it!=MainConsole.CMD.end();it++)
	{
		switch ((*it).second.Type)
		{
			case NEW_CONSOLE_COMMAND:

				MainConsole.Add(NEW_SYSMESS,(*it).first.c_str());

				break;

			default:

				break;
		}
	}
}



/* ��������� ����� �������� */
void conVideoApply(const newString &s)
{
	MainRender.CurrentCamera->Frustum.SetFoV(MainSettings.FOV);

	MainRender.CurrentCamera->Frustum.SetzNear(MainSettings.ZNEAR);

	MainRender.CurrentCamera->Frustum.SetzFar(MainSettings.ZFAR);

	/* ���� ������� ������� � �������, */
	if (MainConsole.IsDrawing)

		/* �� �������� ��������� */
		MainRender.Reshape(MainSettings.SCREEN_WIDTH,MainSettings.SCREEN_HEIGHT);
}

/* ���������� ����� ������ */
void conVideoRestart(const newString &s)
{
	MainConsole.Add(0,"������� ����� ������...");

#if 0
	PostMessage (NEW_window.hWnd, WM_QUIT, 0, 0);
#else
	printf("[error] in %s message 'WM_QUIT' was not sent to 'NEW_window.hWnd' \n", __func__);
#endif /* (0) */
}


/* ���������� ���������� */
newConsole MainConsole;

/* ����������� */
newConsole::newConsole(void)
{
	/* ������� �������� ������� ������ ��������� */
//+++	MessLevel=NEW_ROBUST_MESSAGE_LEVEL;
	MessLevel=NEW_MAX_MESSAGE_LEVEL;

	/* �� �������� */
	IsDrawing=FALSE;

	/* ������ ��� ���� */
	conFile=fopen("new.log","w");

	fclose(conFile);

	/* ��������� �� ������� ������� �����.  ������������ ��� ��������: Page Up, Page Down, End */
	Line=0;

	/* ������� � ������� ������ ������� */
	HistoryPos=(Consoles::iterator)NULL;

	/* ��������� �������� ������� �������:  ���� ������ (Quake ����) */
	AddCMD("FoV",NEW_VARIABLE_FLOAT,&MainSettings.FOV,&conVideoApply);

	/* ������ */
	AddCMD("Help",NEW_CONSOLE_COMMAND,NULL,&conHelp);

	/* ���������� ����� ������ */
	AddCMD("vidrestart",NEW_CONSOLE_COMMAND,NULL,&conVideoRestart);

	/* �������� fps? */
	AddCMD("r_drawfps",NEW_VARIABLE_BOOL,&MainRender.ISfps);

	/* ������ ������ */
	AddCMD("SCREEN_WIDTH",NEW_VARIABLE_UINT,&MainSettings.SCREEN_WIDTH);

	/* ������ ������ */
	AddCMD("SCREEN_HEIGHT",NEW_VARIABLE_UINT,&MainSettings.SCREEN_HEIGHT);

	/* ��� �� ������� */
	AddCMD("BPP",NEW_VARIABLE_BYTE,&MainSettings.BPP);

	/* ����� �� ������� */
	AddCMD("DEPTH",NEW_VARIABLE_BYTE,&MainSettings.DEPTH);

	/* ����������? */
	AddCMD("FULLSCREEN",NEW_VARIABLE_BOOL,&MainSettings.FULLSCREEN);

	/* ������ ������ */
	AddCMD("SCREEN_frequency",NEW_VARIABLE_UINT,&MainSettings.SCREEN_frequency);

	/* ������� ��������� ��������� */
	AddCMD("ZNEAR",NEW_VARIABLE_FLOAT,&MainSettings.ZNEAR,&conVideoApply);

	/* ������� ��������� ��������� */
	AddCMD("ZFAR",NEW_VARIABLE_FLOAT,&MainSettings.ZFAR,&conVideoApply);

	/* ��������� ���� */
	AddCMD("TITLE",NEW_VARIABLE_STRING,&MainSettings.TITLE);

	/* �������� ��� ������� */
	AddCMD("con_TEXTUREFILE",NEW_VARIABLE_STRING,&MainSettings.CONSOLE.TEXTURE_FILE);

	/* ���� ������� */
    AddCMD("con_Alpha",NEW_VARIABLE_BYTE,&MainSettings.CONSOLE.COLOR.RGBColor.a);

	/* ����� ������� */
	AddCMD("con_FontFile",NEW_VARIABLE_STRING,&MainSettings.CONSOLE.FONT_FILE);

	/* ��������� �� Y ������� */
	AddCMD("con_YPOS",NEW_VARIABLE_FLOAT,&MainSettings.CONSOLE.YPOS);

	/* �������� �������� ������� */
	AddCMD("con_Speed",NEW_VARIABLE_FLOAT,&MainSettings.CONSOLE.SPEED);

	/* �������� �������� ������ */
	AddCMD("cam_Speed",NEW_VARIABLE_FLOAT,&MainSettings.Camera_Speed);

	/* ������������ ������� */
	AddCMD("r_DrawNormals",NEW_VARIABLE_BOOL,&MainSettings.Render.DrawNormals);

	/* ������������ ������ */
	AddCMD("r_DrawHelp",NEW_VARIABLE_BOOL,&MainSettings.Render.DrawHelp);
}

newConsole::~newConsole(void)
{
	
}

/* ������� ���������� ������� */
BOOL newConsole::AddCMD(const newString &_Name, newEnum _Type, void *_Link, void (*_Adress)(const newString &Command))
{
#if 0
	/* ��� ������� ������������� � ������� ������� */
	_strupr((char *)_Name.c_str());
#else
	printf("[warning] skipped '_strupr' in %s. Stromg is: %s \n", __func__, (char *)_Name.c_str() );
#endif /* (0) */

	CMDS::iterator it;

	/* ���� ��� ���� � ����� ������ - ������� */
	if ((it=CMD.find(_Name))!=CMD.end())

		return FALSE;

newConCommand k;

	k.Type=_Type;

	k.Link=_Link;

	k.Adress=_Adress;

	/* ��������� ������� */
	CMD.insert(CMDS::value_type(_Name,k));

	return TRUE;
}

/* ��������� ������� */
BOOL newConsole::ExecCMD(const newString &cmd)
{
/* �������, ������� �������� */
newString command, value;

	/* ����� ���������� ���� ( ����������� �������� ��� �����) */
	UINT n=newStrWordCount(cmd);

	/* ���� ���� �� ������, �� ������� */
	if (1 > n)

		return FALSE;

	/* ������ ����� - ��� ������� */
	newGetStrWord(1,cmd,command);

	/* � ������� ������� */
	newStrMakeUp(command);

	/* ���� ��� ����� �������, */
	if (CMD.find(command)==CMD.end())
	{
		/* �� ������� ������ */
		MainConsole.Add(0,"����������� �������: %s",cmd.c_str());

		/* ������� */
		return FALSE;
	}

	/* ������ �� ������� */
	newConCommand &k=CMD[command];

	/* � ����������� �� ���� */
	switch (k.Type)
	{
		case NEW_VARIABLE_BYTE:

			/* ���� ��� ������� ����������, �� ���������� ������� ��������. */
			if (2 > n)
			{
				MainConsole.Add(0,"%s %d",cmd.c_str(),*(BYTE *)k.Link);

				return TRUE;
			}

			newGetStrWord(2,cmd,value);

			/* ����� �������� �������� */
			*(BYTE *)k.Link=newStrToBYTE(value);

			/* ��������� �������, ���� ������ */
			if (k.Adress)

				k.Adress(cmd);

			break;

		/* ���������� ��������� */
		case NEW_VARIABLE_INT:

			if (2 > n)
			{
				MainConsole.Add(0,"%s %d",cmd.c_str(),*(int *)k.Link);

				return TRUE;
			}
			newGetStrWord(2,cmd,value);

			*(int *)k.Link=newStrToInt(value);

			/* ��������� �������, ���� ������ */
			if (k.Adress)
				
				k.Adress(cmd);

			break;

		case NEW_VARIABLE_UINT:

			if (2 > n)
			{
				MainConsole.Add(0,"%s %d",cmd.c_str(),*(UINT *)k.Link);

				return TRUE;
			}
			newGetStrWord(2,cmd,value);

			*(UINT *)k.Link = newStrToUINT(value);

			if (k.Adress)
				
				k.Adress(cmd);

			break;

		case NEW_VARIABLE_FLOAT:

			if (2 > n)
			{
				MainConsole.Add(0,"%s %f",cmd.c_str(),*(float *)k.Link);

				return TRUE;
			}
			newGetStrWord(2,cmd,value);

			*(float *)k.Link = newStrToFloat(value);

			if (k.Adress)
				
				k.Adress(cmd);

			break;

		case NEW_VARIABLE_BOOL:

			if (2 > n)
			{
				MainConsole.Add(0,"%s %d",cmd.c_str(),(*(BOOL *)k.Link));

				return TRUE;
			}
			newGetStrWord(2,cmd,value);

			*(BOOL *)k.Link=((newStrToUINT(value)>0)? TRUE : FALSE);

			if (k.Adress)
				
				k.Adress(cmd);

			break;

		case NEW_VARIABLE_STRING:

			if (2 > n)
			{
				MainConsole.Add(0,"%s %s",cmd.c_str(),(*(newString *)k.Link).c_str());

				return TRUE;
			}
			*(newString *)k.Link=cmd.substr(command.size()+1,cmd.size()-command.size()-1);

			if (k.Adress)
				
				k.Adress(cmd);

			break;

		case NEW_CONSOLE_COMMAND:

			/* ���� ��� �������, �� ������ ��������� ������� ��������� */
			if (k.Adress)
				
				k.Adress(cmd);

			break;
	}

	return TRUE;
}

/* ����� ��� ��������� ��������� */
char mess[16384];

/* ������� ��������� � ������� ��������� */
void newConsole::Add(UINT Level, const char *Message,...)
{
	/* ���� ������� �� ���������, */
	if (Level>MessLevel)
		
		/* �� ������� */
		return;

	/* ���� ���� ������m, */
	if(Message=="")	

		/* to n����� �� ������ */
		return;
	
	/* ������ �� ������ ���������� */
	va_list		va;

	/* �������� ����� � buf, ������� ���������� �� ���������� */
	va_start(va, Message);

	vsprintf(mess, Message, va);

	va_end(va);

	/* ��������� � ������� */
	Console.push_back(mess);

	/* ��������� ������� ������ */
	if (0 < Line)
		
		Line=(UINT)Cutten.size()-Line;

	UpdateCut(mess);

	if (0 < Line)
		
		Line=(UINT)Cutten.size()-Line;

	/* ��������� ������� ���� � ����� � ��������� */
	time_t ltime;

	time( &ltime );

	char *mtime=ctime( &ltime );

	mtime[strlen(mtime)-1]=0;

	/* � ������ � ���� */
	conFile=fopen("new.log","a+t");

//+++	fprintf(conFile,"%s %s\n",mtime,mess);
		fprintf(conFile,"%s\n",mess);

	fclose(conFile);

	/* ������ ������� */
	if (MainSettings.CONSOLE.STATUS==NEW_CONSOLE_FULLSCREEN)

		MainRender.DrawConsole();
}

/* ������������ ������� */
void newConsole::ProcessKey(DWORD Key)
{
	/* �������:  ������. ��������: ������ ��������� ������� */
	if (Key==192)
	{
		if ((MainSettings.CONSOLE.STATUS==NEW_CONSOLE_FADEOUT) || (MainSettings.CONSOLE.STATUS==NEW_CONSOLE_NOTVISIBLE))
		{
			MainSettings.CONSOLE.STATUS=NEW_CONSOLE_FADEIN;

			return;
		}
		if ((MainSettings.CONSOLE.STATUS==NEW_CONSOLE_FADEIN) || (MainSettings.CONSOLE.STATUS==NEW_CONSOLE_VISIBLE))
		{
			MainSettings.CONSOLE.STATUS=NEW_CONSOLE_FADEOUT;

			return;
		}
		return;
	}

	/* �������:  BackSpace. ��������: ������� ��������� ������ */
	if (Key==8)
	{
		if (Current.size())
		{
			Current.erase(Current.end()-1);
		}
		return;
	}

	/* �������:  Enter. ��������: ��������� �������, ��������� ������� � �������, ������� ������ */
	if (Key==13)
	{
		ExecCMD(Current);

		CMDHistory.push_back(Current);

		HistoryPos=CMDHistory.end();

		Current.clear();
	}

	/* �������: Esc. ��������: ������� ������ */
	if (Key==27) Current.clear();

	/* �������: ������� �����. ��������: �������� �� ������� ������� ���� */
	if (Key==38)
	{
		if (HistoryPos!=(Consoles::iterator) NULL)

			if (HistoryPos!=CMDHistory.begin())
			{
				HistoryPos--;

				Current=*(HistoryPos);
			}
	}

	/* �������: ������� ����. ��������: �������� �� ������� ������� ���� */
	if (Key==40)
	{
		if (HistoryPos!=(Consoles::iterator)NULL)

			if (HistoryPos!=CMDHistory.end())
			{
				HistoryPos++;

				if (HistoryPos!=CMDHistory.end())

					Current=*(HistoryPos);

				else
					Current.clear();
			}
			else
				Current.clear();
	}

	/* �������: Tab. ��������: ����������� ������� ��� ������� ������ ������� */
	if (Key==9)

		if (!Current.empty())

			AutoCompleteCMD();

	/* ���������� ���� �� ��������� �������� ������. ��� Page Up, Page Down ��������� ��������� */
	UINT ConsolePage=MainRender.FontDisplayHeight/32-1;

	/* �������: Page Up. ��������: ������� ����� �� ���� �������� */
	if (Key==33)
	{
		if ((Line+ConsolePage)<Cutten.size())

			Line+=ConsolePage;
	}

	/* �������: Page Down. ��������: ������� ���� �� ���� �������� */
	if (Key==34)

		if (Line>0)
		{
			if (((int)Line-(int)ConsolePage)>0)

				Line-=ConsolePage;

			else
				Line=0;
		}

	/* �������: End. ��������: ������� ���� �� ����� */
	if (Key==35)
	{
		Line=0;
	}
}

/* ���������� ��������� ������������� ������� */
void newConsole::AutoCompleteCMD()
{
newString cmdup;

	newStrMakeUp(cmdup,Current);

	newStringList temp;

	/* ����, �� ������ ������� ������ */
	for (CMDS::iterator it=CMD.begin();it!=CMD.end();it++)

		if (newStrStr(cmdup,(*it).first)==0)

			temp.push_back((*it).first);

	/* ���� ����� */
	if (temp.size()>0)

		/* ����� ���� */
		if (1 == temp.size() )
		{
			/* ������ ������� */
			Current=*temp.begin();

			return;
		}
		else
		{
			/* ���� ������ �����, �� ������� ������ */
			MainConsole.Add(0,"������ ���������� ��� ������, ������������ � '%s':",Current.c_str());

			for (newStringList::iterator it=temp.begin();it!=temp.end();it++)

				MainConsole.Add(0,(*it).c_str());

			return;
		}

	/* ������ �� ������� */
	MainConsole.Add(0,"���������� ��� ������, ������������ � '%s', �� �������.",Current.c_str());
}

/* �������� ������� ������ ����� ����������. �������� ����������� ��� ���������� ��������� ������� �����, �� �������� � ����� */
void newConsole::UpdateCut(const newString &src)
{
	if (!IsDrawing)

		return;
	if (ConFont==NULL)

		return;

	newStringList::iterator safepos=(newStringList::iterator) NULL;

	newString t,p;

	t=src;

	p="";

	safepos=Cutten.begin();

	size_t length=0,slen=0,len=t.size();

	for (UINT i=0;i<len;i++)
	{
	unsigned char h=*((unsigned char *)t.c_str()+i);

		if ((length+ConFont->FTrim[h][1])<MainRender.FontDisplayWidth-16)
		{
			length+=ConFont->FTrim[h][1]+ConFont->Spacing;

			p+=h;
		}
		else
		{	/* safepos=Cutten.insert(safepos,p); */
			Cutten.push_front(p);

			length=ConFont->FTrim[h][1]+ConFont->Spacing;

			slen+=length;

			p=h;
		}
	}

	if (!(p=="")) 
	{
		if (slen!=0) 

			Cutten.push_front(p);

		else

			Cutten.insert(safepos,p);
	}

}

/* �������� ��� ������� ������. �������� ����������� ��� ���������� ��������� ������� �����, �� �������� � ����� */
void newConsole::UpdateCutten()
{
	if (!IsDrawing)

		return;

	Consoles::iterator it=MainConsole.Console.begin();

	Cutten.clear();

	while (it!=MainConsole.Console.end())
	{
		UpdateCut(*it);

		it++;
	}
}
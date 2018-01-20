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

/* Консольные процедуры */

/* Help - помощь. Выводит список всех команд и переменных. */
void conHelp(const newString &s)
{
	/* Вывод на консоль */
	MainConsole.Add(NEW_SYSMESS,"Egine консольная помощь:");

	MainConsole.Add(NEW_SYSMESS,"Переменные:");

	/* Бежим по всем командам и переменых */
	for (CMDS::iterator it=MainConsole.CMD.begin();it!=MainConsole.CMD.end();it++)
	{
		/* В зависимости от типа */
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

	/* Тоже самое с командами */
	MainConsole.Add(NEW_SYSMESS,"Команды:");

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



/* Изменение видео настроек */
void conVideoApply(const newString &s)
{
	MainRender.CurrentCamera->Frustum.SetFoV(MainSettings.FOV);

	MainRender.CurrentCamera->Frustum.SetzNear(MainSettings.ZNEAR);

	MainRender.CurrentCamera->Frustum.SetzFar(MainSettings.ZFAR);

	/* Если команда набрана в консоли, */
	if (MainConsole.IsDrawing)

		/* то изменяем настройки */
		MainRender.Reshape(MainSettings.SCREEN_WIDTH,MainSettings.SCREEN_HEIGHT);
}

/* Перезапуск видео режима */
void conVideoRestart(const newString &s)
{
	MainConsole.Add(0,"Рестарт видео режима...");

#if 0
	PostMessage (NEW_window.hWnd, WM_QUIT, 0, 0);
#else
	printf("[error] in %s message 'WM_QUIT' was not sent to 'NEW_window.hWnd' \n", __func__);
#endif /* (0) */
}


/* Глобальная переменная */
newConsole MainConsole;

/* Конструктор */
newConsole::newConsole(void)
{
	/* Текущий возмоный уровень вывода сообщений */
//+++	MessLevel=NEW_ROBUST_MESSAGE_LEVEL;
	MessLevel=NEW_MAX_MESSAGE_LEVEL;

	/* Не рисуется */
	IsDrawing=FALSE;

	/* Создаём лог файл */
	conFile=fopen("new.log","w");

	fclose(conFile);

	/* Находимся на нулевой строчки снизу.  Используется при клавишах: Page Up, Page Down, End */
	Line=0;

	/* Позиция в истории команд нулевая */
	HistoryPos=(Consoles::iterator)NULL;

	/* Добавляем основные команды консоли:  Угол обзора (Quake вида) */
	AddCMD("FoV",NEW_VARIABLE_FLOAT,&MainSettings.FOV,&conVideoApply);

	/* Помощь */
	AddCMD("Help",NEW_CONSOLE_COMMAND,NULL,&conHelp);

	/* Перезапуск видео режима */
	AddCMD("vidrestart",NEW_CONSOLE_COMMAND,NULL,&conVideoRestart);

	/* Рисовать fps? */
	AddCMD("r_drawfps",NEW_VARIABLE_BOOL,&MainRender.ISfps);

	/* Ширина экрана */
	AddCMD("SCREEN_WIDTH",NEW_VARIABLE_UINT,&MainSettings.SCREEN_WIDTH);

	/* Высота экрана */
	AddCMD("SCREEN_HEIGHT",NEW_VARIABLE_UINT,&MainSettings.SCREEN_HEIGHT);

	/* Бит на пиксель */
	AddCMD("BPP",NEW_VARIABLE_BYTE,&MainSettings.BPP);

	/* Битов на глубину */
	AddCMD("DEPTH",NEW_VARIABLE_BYTE,&MainSettings.DEPTH);

	/* Полноэкран? */
	AddCMD("FULLSCREEN",NEW_VARIABLE_BOOL,&MainSettings.FULLSCREEN);

	/* Высота экрана */
	AddCMD("SCREEN_frequency",NEW_VARIABLE_UINT,&MainSettings.SCREEN_frequency);

	/* Ближняя плоскость отсечения */
	AddCMD("ZNEAR",NEW_VARIABLE_FLOAT,&MainSettings.ZNEAR,&conVideoApply);

	/* Дальняя плоскость отсечения */
	AddCMD("ZFAR",NEW_VARIABLE_FLOAT,&MainSettings.ZFAR,&conVideoApply);

	/* Заголовок окна */
	AddCMD("TITLE",NEW_VARIABLE_STRING,&MainSettings.TITLE);

	/* Картинка для консоли */
	AddCMD("con_TEXTUREFILE",NEW_VARIABLE_STRING,&MainSettings.CONSOLE.TEXTURE_FILE);

	/* Цвет консоли */
    AddCMD("con_Alpha",NEW_VARIABLE_BYTE,&MainSettings.CONSOLE.COLOR.RGBColor.a);

	/* Шрифт консоли */
	AddCMD("con_FontFile",NEW_VARIABLE_STRING,&MainSettings.CONSOLE.FONT_FILE);

	/* Положение по Y консоли */
	AddCMD("con_YPOS",NEW_VARIABLE_FLOAT,&MainSettings.CONSOLE.YPOS);

	/* Скорость движения консоли */
	AddCMD("con_Speed",NEW_VARIABLE_FLOAT,&MainSettings.CONSOLE.SPEED);

	/* Скорость движения камеры */
	AddCMD("cam_Speed",NEW_VARIABLE_FLOAT,&MainSettings.Camera_Speed);

	/* Отрисовывать нормали */
	AddCMD("r_DrawNormals",NEW_VARIABLE_BOOL,&MainSettings.Render.DrawNormals);

	/* Отрисовывать помощь */
	AddCMD("r_DrawHelp",NEW_VARIABLE_BOOL,&MainSettings.Render.DrawHelp);
}

newConsole::~newConsole(void)
{
	
}

/* Фукнция добавления команды */
BOOL newConsole::AddCMD(const newString &_Name, newEnum _Type, void *_Link, void (*_Adress)(const newString &Command))
{
#if 0
	/* Все команды преобразуются в верхний регистр */
	_strupr((char *)_Name.c_str());
#else
	printf("[warning] skipped '_strupr' in %s. Stromg is: %s \n", __func__, (char *)_Name.c_str() );
#endif /* (0) */

	CMDS::iterator it;

	/* Если уже есть с таким именем - выходим */
	if ((it=CMD.find(_Name))!=CMD.end())

		return FALSE;

newConCommand k;

	k.Type=_Type;

	k.Link=_Link;

	k.Adress=_Adress;

	/* Добавляем команду */
	CMD.insert(CMDS::value_type(_Name,k));

	return TRUE;
}

/* Запускаем команду */
BOOL newConsole::ExecCMD(const newString &cmd)
{
/* команда, входное значение */
newString command, value;

	/* Узнаём количество слов ( разделенных пробелом или табом) */
	UINT n=newStrWordCount(cmd);

	/* Если нету ни одного, то выходим */
	if (1 > n)

		return FALSE;

	/* Первое слово - это команда */
	newGetStrWord(1,cmd,command);

	/* В верхний регистр */
	newStrMakeUp(command);

	/* Если нет такой команды, */
	if (CMD.find(command)==CMD.end())
	{
		/* то выводим ошибку */
		MainConsole.Add(0,"Неизвестная команда: %s",cmd.c_str());

		/* выходим */
		return FALSE;
	}

	/* Ссылка на команду */
	newConCommand &k=CMD[command];

	/* В зависимости от типа */
	switch (k.Type)
	{
		case NEW_VARIABLE_BYTE:

			/* Если нет входных параметров, то показываем текущее значение. */
			if (2 > n)
			{
				MainConsole.Add(0,"%s %d",cmd.c_str(),*(BYTE *)k.Link);

				return TRUE;
			}

			newGetStrWord(2,cmd,value);

			/* Иначе изменяем значение */
			*(BYTE *)k.Link=newStrToBYTE(value);

			/* Запускаем функцию, если задана */
			if (k.Adress)

				k.Adress(cmd);

			break;

		/* Аналогично остальные */
		case NEW_VARIABLE_INT:

			if (2 > n)
			{
				MainConsole.Add(0,"%s %d",cmd.c_str(),*(int *)k.Link);

				return TRUE;
			}
			newGetStrWord(2,cmd,value);

			*(int *)k.Link=newStrToInt(value);

			/* Запускаем функцию, если задана */
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

			/* Если это команда, то просто запускаем функции обработки */
			if (k.Adress)
				
				k.Adress(cmd);

			break;
	}

	return TRUE;
}

/* буфер для обработки сообщений */
char mess[16384];

/* Функция добавляем в консоль сообщение */
void newConsole::Add(UINT Level, const char *Message,...)
{
	/* Если уровень не позволяет, */
	if (Level>MessLevel)
		
		/* то выходим */
		return;

	/* Если нету текстаm, */
	if(Message=="")	

		/* to nичего не делаем */
		return;
	
	/* Ссылка на список аргументов */
	va_list		va;

	/* Копируем текст в buf, заменяя переменные их значениями */
	va_start(va, Message);

	vsprintf(mess, Message, va);

	va_end(va);

	/* Добавляем в консоль */
	Console.push_back(mess);

	/* Обновляем видимые строки */
	if (0 < Line)
		
		Line=(UINT)Cutten.size()-Line;

	UpdateCut(mess);

	if (0 < Line)
		
		Line=(UINT)Cutten.size()-Line;

	/* Добавляем текущую дату и время к сообщению */
	time_t ltime;

	time( &ltime );

	char *mtime=ctime( &ltime );

	mtime[strlen(mtime)-1]=0;

	/* И выводи в файл */
	conFile=fopen("new.log","a+t");

//+++	fprintf(conFile,"%s %s\n",mtime,mess);
		fprintf(conFile,"%s\n",mess);

	fclose(conFile);

	/* Рисуем консоль */
	if (MainSettings.CONSOLE.STATUS==NEW_CONSOLE_FULLSCREEN)

		MainRender.DrawConsole();
}

/* Обрабатываем клавиши */
void newConsole::ProcessKey(DWORD Key)
{
	/* Клавиша:  Тильда. Действие: Меняем состояние консоли */
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

	/* Клавиша:  BackSpace. Действие: Удаляем последний символ */
	if (Key==8)
	{
		if (Current.size())
		{
			Current.erase(Current.end()-1);
		}
		return;
	}

	/* Клавиша:  Enter. Действие: Запускаем команду, сохраняем команду в истории, очищаем строку */
	if (Key==13)
	{
		ExecCMD(Current);

		CMDHistory.push_back(Current);

		HistoryPos=CMDHistory.end();

		Current.clear();
	}

	/* Клавиша: Esc. Действие: Очищаем строку */
	if (Key==27) Current.clear();

	/* Клавиша: Стрелка вверх. Действие: Копируем из истории команду выше */
	if (Key==38)
	{
		if (HistoryPos!=(Consoles::iterator) NULL)

			if (HistoryPos!=CMDHistory.begin())
			{
				HistoryPos--;

				Current=*(HistoryPos);
			}
	}

	/* Клавиша: Стрелка вниз. Действие: Копируем из истории команду ниже */
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

	/* Клавиша: Tab. Действие: Заканчиваем команду или выводим список похожих */
	if (Key==9)

		if (!Current.empty())

			AutoCompleteCMD();

	/* Количество букв по вертикали половины экрана. При Page Up, Page Down считается страницой */
	UINT ConsolePage=MainRender.FontDisplayHeight/32-1;

	/* Клавиша: Page Up. Действие: Листаем вверх на одну страницу */
	if (Key==33)
	{
		if ((Line+ConsolePage)<Cutten.size())

			Line+=ConsolePage;
	}

	/* Клавиша: Page Down. Действие: Листаем вниз на одну страницу */
	if (Key==34)

		if (Line>0)
		{
			if (((int)Line-(int)ConsolePage)>0)

				Line-=ConsolePage;

			else
				Line=0;
		}

	/* Клавиша: End. Действие: Листаем вниз до конца */
	if (Key==35)
	{
		Line=0;
	}
}

/* Завершение буквеного представления команды */
void newConsole::AutoCompleteCMD()
{
newString cmdup;

	newStrMakeUp(cmdup,Current);

	newStringList temp;

	/* Цикл, по поиску похожих команд */
	for (CMDS::iterator it=CMD.begin();it!=CMD.end();it++)

		if (newStrStr(cmdup,(*it).first)==0)

			temp.push_back((*it).first);

	/* Если нашел */
	if (temp.size()>0)

		/* нашел одну */
		if (1 == temp.size() )
		{
			/* Делаем текущей */
			Current=*temp.begin();

			return;
		}
		else
		{
			/* Если больше одной, то выводим список */
			MainConsole.Add(0,"Список переменных или команд, начинающихся с '%s':",Current.c_str());

			for (newStringList::iterator it=temp.begin();it!=temp.end();it++)

				MainConsole.Add(0,(*it).c_str());

			return;
		}

	/* команд не найдено */
	MainConsole.Add(0,"Переменных или команд, начинающихся с '%s', не найдено.",Current.c_str());
}

/* Добавить видимые строки новым сообщением. Операция необходимая для правильной отрисовки длинных строк, не входящих в экран */
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

/* Обновить все видимые строки. Операция необходимая для правильной отрисовки длинных строк, не входящих в экран */
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
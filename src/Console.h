#ifndef __CONSOLE_H__
#define __CONSOLE_H__

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

#include "Types.h"

#include "Texture.h"

#include "Font.h"

struct newConCommand
{
	newEnum Type;

	void *Link;

	void (*Adress)(const newString &Command);
};


typedef std::list<newString> Consoles;

typedef std::map<newString,newConCommand> CMDS;

class newConsole
{
public:

	newConsole(void);

	~newConsole(void);

	BOOL IsDrawing;

	UINT MessLevel;

	CMDS CMD;

	Consoles Console;

	/* LOG file */
	FILE *conFile;

	newTexture *Tex;

	BOOL AddCMD(const newString &_Name, newEnum _Type, void *_Link, void (*_Adress)(const newString &Command) = NULL);

	BOOL ExecCMD(const newString &cmd);

	/* Level -      Message -      */
	void Add(UINT Level, const char *Message,...);

	newString Current;

	UINT Line;

	void ProcessKey(DWORD Key);

	Consoles CMDHistory;

	Consoles::iterator HistoryPos;

	void AutoCompleteCMD();

	newStringList Cutten;

	void UpdateCut(const newString &src);

	void UpdateCutten();

	newFont *ConFont;
};

extern newConsole MainConsole;

#endif /* ifndef-define include guard */

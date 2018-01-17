#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Types.h"

#include "Texture.h"

#include "Font.h"

struct newConCommand
{
	/* Тип переменной или команды */
	newEnum Type;

	/* Ссылка на переменную */
	void *Link;

	/* Адрес процедуры, которая запускается после обработки команды. Если NULL, то не выполняется. */
	void (*Adress)(const newString &Command);
};

/* Типы для консоли */
typedef std::list<newString> Consoles;

typedef std::map<newString,newConCommand> CMDS;

/* Класс консоли */
class newConsole
{
public:

	newConsole(void);

	~newConsole(void);

	/* Консоль создана? */
	BOOL IsDrawing;

	/* Текущий уровень сообщений */
	UINT MessLevel;

	/* Список всех переменных и команд */
	CMDS CMD;

	/* Консоль, список всех сообщений */
	Consoles Console;

	/* LOG Файл */
	FILE *conFile;

	/* Ссылка на текстуру */
	newTexture *Tex;

	/* Добавление команды или переменной */
	BOOL AddCMD(const newString &_Name, newEnum _Type, void *_Link, void (*_Adress)(const newString &Command) = NULL);

	/* Обработать команду */
	BOOL ExecCMD(const newString &cmd);

	/* Добавить в консоль.  Level - уровень сообщения Message - само сообщение */
	void Add(UINT Level, const char *Message,...);

	/* Текущая строка в консоли */
	newString Current;

	/* Номер первой строки в консоли, считая снизу */
	UINT Line;

	/* Обработка клавиш */
	void ProcessKey(DWORD Key);

	/* История команд */
	Consoles CMDHistory;

	/* Текущая положение в истории команд */
	Consoles::iterator HistoryPos;

	/* Закончить команду по нескольким первым буквам */
	void AutoCompleteCMD();

	/* Список видимых строк */
	newStringList Cutten;

	/* Добовляет строки с нового сообщения */
	void UpdateCut(const newString &src);

	/* Обновляет все видимые строки */
	void UpdateCutten();

	/* Шрифт для консоли */
	newFont *ConFont;
};

extern newConsole MainConsole;

#endif /* ifndef-define include guard */

#include "Errors.h"

#include "Console.h"

/* Уровень сообщений для ошибок */
const UINT NEW_ERR_MESSAGE_LEVEL = 9;

/* Генерация ошибки по результату и коментариям */
newResult GenError(newResult R, const char *Comments)
{
	switch (R)
	{
		case NEW_FILE_NOT_FOUND:

			MainConsole.Add(NEW_ERR_MESSAGE_LEVEL,"Файл не найден: %s",Comments);

			break;

		case NEW_EXIST:
			
			MainConsole.Add(NEW_ERR_MESSAGE_LEVEL,"Файл уже загружен: %s",Comments);
			
			break;

		case NEW_UNKNOW:
			
			MainConsole.Add(NEW_ERR_MESSAGE_LEVEL,"%s",Comments);
			
			break;

		case NEW_UNKNOW_FILE_FORMAT:
			
			MainConsole.Add(NEW_ERR_MESSAGE_LEVEL,"Неизвестный формат файла %s",Comments);
			
			break;

		case NEW_INVALIDE_VALUE:

			MainConsole.Add(NEW_ERR_MESSAGE_LEVEL,"Неизвестное значение! %s",Comments);
			
			break;

		case NEW_INVALID_ENUM:

			MainConsole.Add(NEW_ERR_MESSAGE_LEVEL,"Неизвестная константа! %s",Comments);

			break;

		case NEW_OUT_OF_MEMORY:
			
			MainConsole.Add(NEW_ERR_MESSAGE_LEVEL,"Нехватает памяти:  %s",Comments);
			
			break;
	}
	return R;
}
#include "Errors.h"

#include "Console.h"

/* ������� ��������� ��� ������ */
const UINT NEW_ERR_MESSAGE_LEVEL = 9;

/* ��������� ������ �� ���������� � ����������� */
newResult GenError(newResult R, const char *Comments)
{
	switch (R)
	{
		case NEW_FILE_NOT_FOUND:

			MainConsole.Add(NEW_ERR_MESSAGE_LEVEL,"���� �� ������: %s",Comments);

			break;

		case NEW_EXIST:
			
			MainConsole.Add(NEW_ERR_MESSAGE_LEVEL,"���� ��� ��������: %s",Comments);
			
			break;

		case NEW_UNKNOW:
			
			MainConsole.Add(NEW_ERR_MESSAGE_LEVEL,"%s",Comments);
			
			break;

		case NEW_UNKNOW_FILE_FORMAT:
			
			MainConsole.Add(NEW_ERR_MESSAGE_LEVEL,"����������� ������ ����� %s",Comments);
			
			break;

		case NEW_INVALIDE_VALUE:

			MainConsole.Add(NEW_ERR_MESSAGE_LEVEL,"����������� ��������! %s",Comments);
			
			break;

		case NEW_INVALID_ENUM:

			MainConsole.Add(NEW_ERR_MESSAGE_LEVEL,"����������� ���������! %s",Comments);

			break;

		case NEW_OUT_OF_MEMORY:
			
			MainConsole.Add(NEW_ERR_MESSAGE_LEVEL,"��������� ������:  %s",Comments);
			
			break;
	}
	return R;
}
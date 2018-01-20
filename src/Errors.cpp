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

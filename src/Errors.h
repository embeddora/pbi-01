#ifndef __ERROR_H__
#define __ERROR_H__

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

/* Results. Результаты */
enum newResult {

	/* no mistake */
	NEW_OK	= 0,

	/* already exists */
	NEW_EXIST,

	/* file not found */
	NEW_FILE_NOT_FOUND,

	/* unknown file foprmat */
	NEW_UNKNOW_FILE_FORMAT,

	/* unknown result */
	NEW_UNKNOW,

	/* unknown value */
	NEW_INVALIDE_VALUE,

	/* unknown constant */
	NEW_INVALID_ENUM,

	/* unsufficient memory */
	NEW_OUT_OF_MEMORY,

	/* not implemented feature needed */
	NEW_NOT_IMPLEMENTED,
};

/* Function to generate error; input: error code and comment. Функция генерации ошибки: Код ошибки и коментарии */
newResult GenError(newResult R, const char *Comments);

#endif /* ifndef-define include guard */

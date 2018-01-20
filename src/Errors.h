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

enum newResult {

	NEW_OK	= 0,

	NEW_EXIST,

	NEW_FILE_NOT_FOUND,

	NEW_UNKNOW_FILE_FORMAT,

	NEW_UNKNOW,

	NEW_INVALIDE_VALUE,

	NEW_INVALID_ENUM,

	NEW_OUT_OF_MEMORY,

	NEW_NOT_IMPLEMENTED,
};

/* Function to generate error; input: error code and comment  */
newResult GenError(newResult R, const char *Comments);

#endif /* ifndef-define include guard */

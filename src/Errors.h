#ifndef __ERROR_H__
#define __ERROR_H__

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

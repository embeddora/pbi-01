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

/* memcpy () */
#include <memory.h>

/* <boolean> type */
// We don't use C, C99 , so we don't have booleand in #include <stdbool.h>. Have top cinclude it ourselves 
typedef char boolean;


void* memcpy(void* __dest, const void* __src, int __n)
{
int i;
char *d, *s;

	d = (char *)__dest;
	s = (char *)__src;
	
	for (i=0; i<__n; i++)
	    d[i] = s[i];
	    
	return __dest;
}

/* In Linux haben wir keine <itoa(...)> , d.h. muss man selber schaffen . */
char* itoa (unsigned long long  value,  char str[],  int radix)
{
    char        buf [66];
    char*       dest = buf + sizeof(buf);
    boolean     sign = false;

    if (value == 0) {
        memcpy (str, "0", 2);
        return str;
    }

    if (radix < 0) {
        radix = -radix;
        if ( (long long) value < 0) {
            value = -value;
            sign = true;
        }
    }

    *--dest = '\0';

    switch (radix)
    {
    case 16:
        while (value) {
            * --dest = '0' + (value & 0xF);
            if (*dest > '9') *dest += 'A' - '9' - 1;
            value >>= 4;
        }
        break;
    case 10:
        while (value) {
            *--dest = '0' + (value % 10);
            value /= 10;
        }
        break;

    case 8:
        while (value) {
            *--dest = '0' + (value & 7);
            value >>= 3;
        }
        break;

    case 2:
        while (value) {
            *--dest = '0' + (value & 1);
            value >>= 1;
        }
        break;

    default:            // The slow version, but universal
        while (value) {
            *--dest = '0' + (value % radix);
            if (*dest > '9') *dest += 'A' - '9' - 1;
            value /= radix;
        }
        break;
    }

    if (sign) *--dest = '-';

    memcpy (str, dest, buf +sizeof(buf) - dest);
    return str;
}


/* Слолько слов в строке. Слова разделяются пробелами */
UINT newStrWordCount(const newString &src)
{
UINT n=0;

size_t pos=0;

	if (src[pos]==' ')
		
		pos=src.find_first_not_of(' ',pos);

	while (pos<src.length())
	{
		n++;

		pos=src.find_first_of(' ',pos);

		if (pos<src.length()) pos=src.find_first_not_of(' ',pos);
	}

	return n;
}

/* Выдаёт n-тое слово */
BOOL newGetStrWord(UINT n, const newString &src, newString &dst)
{
UINT k=0;

size_t pos=0,last=0;

	if ((src[pos]==' ')||(src[pos]=='\x9'))
		
		pos=dst.find_first_not_of(" \x9",pos);

	while (pos<src.length())
	{
		k++;

		last=pos;

		pos=src.find_first_of(' ',pos);

		if (k==n)
		{
			dst=src.substr(last,pos-last);

			return TRUE;
		}
		if (pos<src.length()) pos=src.find_first_not_of(' ',pos);
	}
	return FALSE;
}

/* Строку в байт */
BYTE newStrToBYTE(const newString &value)
{
	return (BYTE)(atoi(value.c_str()));
}

/* Строку в int */
int newStrToInt(const newString &value)
{
	return (atoi(value.c_str()));
}

/* Строку в uint */
UINT newStrToUINT(const newString &value)
{
	return (UINT)(atoi(value.c_str()));
}

/* Строку в float */
float newStrToFloat(const newString &value)
{
	return (float)(atof(value.c_str()));
}

/* Делает строку в верхний регистр */
newString &newStrMakeUp(newString &res,const newString &src)
{
	res=src;

//+++ bring back!  16:21 18012018	_strupr((char *)res.c_str());

	return res;
}

/* Делает строку в верхний регистр */
void newStrMakeUp(newString &res)
{
//+++ bring back!  16:21 18012018	_strupr((char *)res.c_str());
}

/* Находит подстроку в строке */
int newStrStr(const newString &sub,const newString &main)
{
char *pdest;

	size_t  result,len=main.size();

	pdest = strstr( 
		(char*)main.c_str(), 
		sub.c_str()
	);

	result = pdest - main.c_str();

	if ((result>=0)&&(result<len)) 

		return (int)result;

	else

		return -1;

}

/* Читает строку из файла */
BOOL newReadStr(FILE *file,newString &res)
{
	res="";

	unsigned char h=fgetc(file);

	while (((h==9)||(h==' '))&&!feof(file))
		
		h=fgetc(file);

	while ((h!='\n')&&!feof(file))
	{
		res+=h;

		h=fgetc(file);

		if (h==9)
			
			h=' ';
	}
	if (feof(file))
		
		return FALSE;
	
	else
		
		return TRUE;
}

/* Переводит Int в newString */
newString newIntToStr(int i)
{
char buf[32];

	itoa(i,buf,10);

	return newString(buf);
}
